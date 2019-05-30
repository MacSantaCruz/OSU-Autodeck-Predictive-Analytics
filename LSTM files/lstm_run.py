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



# Target model chkpt path
model_path = '/models/testing.ckpt'
# Text file containing words for training
training_file = 'test2_5000_noapp'

def read_data(fname):
    with open(fname) as f:
        content = f.readlines()
    content = [x.strip() for x in content]
    content = [word for i in range(len(content)) for word in content[i].split()]
    content = np.array(content)
    return content

training_data = read_data(training_file)
print("Loaded training data...")

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



# Initializing the variables
init = tf.global_variables_initializer()

saver = tf.train.Saver()

with tf.Session() as session:
    saver.restore(session, model_path)
    while True:
    
        iMutex.acquire()
        if iCommand == []:
            iMutex.release()
            continue;
        sentence = iCommand[0]
        iCommand.pop(0)
        iMutex.release()
        #prompt = "%s words: " % n_input
        #sentence = input(prompt)
        #sentence = sentence.strip()
        words = sentence.split(' ')
        if len(words) != n_input:
            continue
        try:
            symbols_in_keys = [dictionary[str(words[i])] for i in range(len(words))]
            for i in range(1):
                keys = np.reshape(np.array(symbols_in_keys), [-1, n_input, 1])
                onehot_pred = session.run(pred, feed_dict={x: keys})
                onehot_pred_index = int(tf.argmax(onehot_pred, 1).eval())
                sentence = "%s" % (reverse_dictionary[onehot_pred_index])
                symbols_in_keys = symbols_in_keys[1:]
                symbols_in_keys.append(onehot_pred_index)
                
            oCommand = sentence
            print(sentence)
            sys.stdout.flush()
        except:
            print("Word not in dictionary")