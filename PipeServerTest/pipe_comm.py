import time
import sys
import threading
import win32pipe, win32file, pywintypes

TENSOR_STREAM = "\\\\.\\pipe\\TENSORFLOW_DATA_STREAM"
ADDIN_STREAM = "\\\\.\\pipe\\ADDIN_DATA_STREAM"

quit = False

def Read():
    global quit

    hPipeR = win32file.INVALID_HANDLE_VALUE
    hPipeR = win32pipe.CreateNamedPipe(
        ADDIN_STREAM,
        win32pipe.PIPE_ACCESS_DUPLEX,
        win32pipe.PIPE_TYPE_BYTE | win32pipe.PIPE_READMODE_BYTE | win32pipe.PIPE_WAIT,
        1,
        65536,
        65536,
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
                resp = win32file.ReadFile(hPipeR, 1024, None)
                print(f"message: {resp}")
                sys.stdout.flush()


        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again in a sec")
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
                strData = input()
                if strData == 'q':
                    quit = True

                data = str.encode(strData+'\n')
                win32file.WriteFile(
				    hPipeW,
				    data
			    )

        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again in a sec")
                sys.stdout.flush()
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe")
                sys.stdout.flush()
                quit = True

    win32file.CloseHandle(hPipeW)
    print("PY - Write Stream Closed")
    sys.stdout.flush()

try:
    w = threading.Thread(name="Writer", target=Write)
    w.start()
except:
    print("Server Thread Error")
    sys.stdout.flush()

try:
    r = threading.Thread(name="Reader", target=Read)
    r.start()
except:
    print("Client Thread Error")
    sys.stdout.flush()

threads = []
threads.append(w)
threads.append(r)
for t in threads:
    t.join()
