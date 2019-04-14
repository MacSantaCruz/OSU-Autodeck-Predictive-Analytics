from __future__ import print_function

import numpy as np
import tensorflow as tf
from tensorflow.contrib import rnn
import random
import collections
import time

import sys
from threading import Thread, Lock
import win32pipe, win32file, pywintypes

TENSOR_STREAM = "\\\\.\\pipe\\TENSORFLOW_DATA_STREAM"
ADDIN_STREAM = "\\\\.\\pipe\\ADDIN_DATA_STREAM"

quit = False
iCommand = [] #queue for commands, in the case the come in faster than the lstm can process
oCommand = "" #only need the last output
iMutex = Lock()
oMutex = Lock()

#reads data from addin
def Read():
    global quit
    hPipeR = win32file.INVALID_HANDLE_VALUE
    hPipeR = win32pipe.CreateNamedPipe(
        ADDIN_STREAM,
        win32pipe.PIPE_ACCESS_DUPLEX,
        win32pipe.PIPE_TYPE_BYTE | win32pipe.PIPE_READMODE_BYTE | win32pipe.PIPE_WAIT,
        1,
        1024,
        1024,
        0,
        None)
    while hPipeR != win32file.INVALID_HANDLE_VALUE and not quit:
        try:
            print("waiting for client")
            sys.stdout.flush()
            win32pipe.ConnectNamedPipe(hPipeR, None)
            print("got client")
            sys.stdout.flush()
            while True:
                resp,data = win32file.ReadFile(hPipeR, 1024, None)
                data = data.decode('utf-8')
                data = data.replace('\n','').replace('\x00','')
                #make this available to the prediction
                iMutex.acquire()
                iCommand.append(data)
                print(iCommand)
                sys.stdout.flush()
                iMutex.release()
        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again")
                sys.stdout.flush()
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe")
                sys.stdout.flush()
                quit = True
        finally:
            win32pipe.DisconnectNamedPipe(hPipeR)
    win32file.CloseHandle(hPipeR)
    print("PY - Read Stream Closed")
    sys.stdout.flush()

#Sends data to Inventor Addin, one command at a time
def Write():
    global quit
    hPipeW = win32file.INVALID_HANDLE_VALUE
    while not quit:
        try:
            hPipeW = win32file.CreateFile(
                TENSOR_STREAM,
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                0,
                None)
            while not quit:
                #constantly sends last command, or list of
                #print("sending " + oCommand)
                sys.stdout.flush()
                data = str.encode(oCommand)
                win32file.WriteFile(
				    hPipeW,
				    data
			    )
                #if data = 'q':
                #    quit = True
        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again")
                sys.stdout.flush()
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe")
                sys.stdout.flush()
                quit = True

    win32file.CloseHandle(hPipeW)
    print("PY - Write Stream Closed")
    sys.stdout.flush()

#initalize threads
try:
    w = Thread(name="Writer", target=Write)
    w.start()
except:
    print("Server Thread Error")
    sys.stdout.flush()

try:
    r = Thread(name="Reader", target=Read)
    r.start()
except:
    print("Client Thread Error")
    sys.stdout.flush()

# Target log path
logs_path = '/tmp/tensorflow/rnn_words'
writer = tf.summary.FileWriter(logs_path)

# Text file containing words for training
training_file = 'test2_5000'

def read_data(fname):
    with open(fname) as f:
        content = f.readlines()
    content = [x.strip() for x in content]
    content = [word for i in range(len(content)) for word in content[i].split()]
    content = np.array(content)
    return content

training_data = read_data(training_file)
print("Loaded training data...")
sys.stdout.flush()

def build_dataset(words):
    count = collections.Counter(words).most_common()
    dictionary = dict()
    for word, _ in count:
        dictionary[word] = len(dictionary)
    reverse_dictionary = dict(zip(dictionary.values(), dictionary.keys()))
    return dictionary, reverse_dictionary

dictionary, reverse_dictionary = build_dataset(training_data)
vocab_size = len(dictionary)

# Parameters
learning_rate = 0.01
training_iters = 50000
display_step = 500
n_input = 1

# number of units in RNN cell
n_hidden = 512

# tf Graph input
x = tf.placeholder("float", [None, n_input, 1])
y = tf.placeholder("float", [None, vocab_size])

# RNN output node weights and biases
weights = {
    'out': tf.Variable(tf.random_normal([n_hidden, vocab_size]))
}
biases = {
    'out': tf.Variable(tf.random_normal([vocab_size]))
}

def RNN(x, weights, biases):

    # reshape to [1, n_input]
    x = tf.reshape(x, [-1, n_input])

    # Generate a n_input-element sequence of inputs
    # (eg. [had] [a] [general] -> [20] [6] [33])
    x = tf.split(x,n_input,1)

    # 2-layer LSTM, each layer has n_hidden units.
    # Average Accuracy= 95.20% at 50k iter
    rnn_cell = rnn.MultiRNNCell([rnn.LSTMCell(n_hidden),rnn.LSTMCell(n_hidden)])
    # rnn_cell = rnn.LSTMCell(n_hidden)

    # generate prediction
    outputs, states = rnn.static_rnn(rnn_cell, x, dtype=tf.float32)

    # there are n_input outputs but
    # we only want the last output
    return tf.matmul(outputs[-1], weights['out']) + biases['out']

pred = RNN(x, weights, biases)

# Loss and optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(logits=pred, labels=y))
optimizer = tf.train.RMSPropOptimizer(learning_rate=learning_rate).minimize(cost)

# Model evaluation
correct_pred = tf.equal(tf.argmax(pred,1), tf.argmax(y,1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

# Initializing the variables
init = tf.global_variables_initializer()

saver = tf.train.Saver()

output = ""
# Launch the graph
with tf.Session() as session:
    session.run(init)
    step = 0
    offset = random.randint(0,n_input+1)
    end_offset = n_input + 1
    acc_total = 0
    loss_total = 0

    writer.add_graph(session.graph)
    try:
        saver.restore(session, "./tmp/model.ckpt")
    except:
        print("No Model Loaded")
        sys.stdout.flush()
    while step < training_iters:
        # Generate a minibatch. Add some randomness on selection process.
        if offset > (len(training_data)-end_offset):
            offset = random.randint(0, n_input+1)

        symbols_in_keys = [ [dictionary[ str(training_data[i])]] for i in range(offset, offset+n_input) ]
        symbols_in_keys = np.reshape(np.array(symbols_in_keys), [-1, n_input, 1])

        symbols_out_onehot = np.zeros([vocab_size], dtype=float)
        symbols_out_onehot[dictionary[str(training_data[offset+n_input])]] = 1.0
        symbols_out_onehot = np.reshape(symbols_out_onehot,[1,-1])

        _, acc, loss, onehot_pred = session.run([optimizer, accuracy, cost, pred], \
                                                feed_dict={x: symbols_in_keys, y: symbols_out_onehot})
        loss_total += loss
        acc_total += acc
        if (step+1) % display_step == 0:
            print("Iter= " + str(step+1) + ", Average Loss= " + \
                  "{:.6f}".format(loss_total/display_step) + ", Average Accuracy= " + \
                  "{:.2f}%".format(100*acc_total/display_step))
            sys.stdout.flush()
            if 100*acc_total/display_step > 5:
                step = training_iters
            acc_total = 0
            loss_total = 0
            symbols_in = [training_data[i] for i in range(offset, offset + n_input)]
            symbols_out = training_data[offset + n_input]
            symbols_out_pred = reverse_dictionary[int(tf.argmax(onehot_pred, 1).eval())]
            #print("%s - [%s] vs [%s]" % (symbols_in,symbols_out,symbols_out_pred))
        step += 1
        offset += (n_input+1)
    #save_path = saver.save(session, "./tmp/model.ckpt")
    while True:
        iMutex.acquire()
        if iCommand == []:
            iMutex.release()
            continue;
        #prompt = "%s words: " % n_input
        #prompt = "Enter Word: "
        #pipe input here
        sentence = iCommand[0]
        iCommand.pop(0)
        #sentence = input(prompt)
        iMutex.release()
        sentence = sentence.strip()
        words = sentence.split(' ')
        if len(words) != n_input:
            continue
        try:
            symbols_in_keys = [dictionary[str(words[i])] for i in range(len(words))]
            for i in range(1):
                keys = np.reshape(np.array(symbols_in_keys), [-1, n_input, 1])
                onehot_pred = session.run(pred, feed_dict={x: keys})
                onehot_pred_index = int(tf.argmax(onehot_pred, 1).eval())
                #sentence = "%s %s" % (sentence,reverse_dictionary[onehot_pred_index])
                output = "%s" % (reverse_dictionary[onehot_pred_index])
                symbols_in_keys = symbols_in_keys[1:]
                symbols_in_keys.append(onehot_pred_index)
            #pipe out here
            #oMutex.acquire()
            oCommand = output
            print(oCommand)
            sys.stdout.flush()
            #oMutex.release()
        except:
            print("Word not in dictionary")
            sys.stdout.flush()
