import socket, time
from threading import Thread


HOST = '127.0.0.1'
PORT = 5000


class Server:

	def __init__ (self):
		self.sock = socket.socket (socket.AF_INET, socket.SOCK_STREAM)
		self.sock.setsockopt (socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		self.sock.bind ((HOST, PORT))
		self.sock.listen (1)


	def _get_count (self, socket):
		buf = ''
		while True:
			c = socket.recv (1)
			if not c:
				# error or just end of connection/
				if buf:
					raise RuntimeError ("underflow")
				else:
					return -1
			if c == '|':
				return int(buf)
			else:
				buf += c

	def start (self):
		while True:
			connection, addr = self.sock.accept()
			print 'FROM ' + str (addr) + ' - Time: ' + time.strftime ("%H:%M:%S - %d/%m/%Y", time.localtime (time.time()))
			t = Thread (target = self.communicate, args = (connection,))
			t.daemon = True
			t.start()

	def communicate (self, connection):
		size = self._get_count (connection)
		if size != -1:
			command = connection.recv (size)
		else:
			command = "wait"
		while command:
			if command != "wait":
				message = float (command)
				result = time.time()
				response = str (result - message)
				length = str (len (response))
				response = length + "|" + response
				connection.send (response)

			size = self._get_count (connection)
			if size != -1:
				command = connection.recv (size)
			else:
				command = "wait"
		connection.close()

	def finish (self):
		print "\nKilled\n"

		self.sock.close()
		print "bye :(\n"



if __name__ == "__main__":
	try:
		s = Server()
		s.start()
	except KeyboardInterrupt:
		s.finish()