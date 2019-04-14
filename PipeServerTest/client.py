import time;
import sys;
import win32pipe, win32file, pywintypes;
import _thread

print("writes")
quit = False

#def aThread():
#    print("a thread")
#
#try:
#    _thread.start_new_thread(aThread,())
#except:
#    print("Failed to build thread")

while not quit:
	try:
		hPipe = win32file.CreateFile(
			"\\\\.\\pipe\\CPP_TO_PY_PIPE",
			win32file.GENERIC_READ | win32file.GENERIC_WRITE,
			0,
			None,
			win32file.OPEN_EXISTING,
			0,
			None
		)
		#res = win32pipe.SetNamedPipeHandleState(hPipe, win32pipe.PIPE_READMODE_MESSAGE, None, None)
		#if res == 0:
		#	print(f"SetNamedPipeHandleState return code: {res}")
		while True:
			strData = input()
			data = str.encode(strData+'\n')
			win32file.WriteFile(
				hPipe,
				data
			)

			#resp = win32file.ReadFile(hPipe, 1024, None)
			#print(resp)

	except pywintypes.error as e:
		if e.args[0] == 2:
			print("no pipe, trying again in a sec")
			time.sleep(1)
		elif e.args[0] == 109:
			print("broken pipe, bye bye")
			quit = True