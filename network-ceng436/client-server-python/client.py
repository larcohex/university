import socket, time

HOST = '127.0.0.1'
PORT = 5000


class Client:

	def __init__ (self):
		self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		self.sock.connect((HOST, PORT))


	def _get_count (self, socket):
		buf = ""
		while True:
			c = socket.recv (1)
			if not c:
				# error or just end of connection/
				if buf:
					raise RuntimeError ("underflow")
				else:
					return -1
			if c == "|":
				return int(buf)
			else:
				buf += c


	def communicate (self):
		message = str (time.time())
		length = str (len (message))
		message = length + "|" + message
		self.sock.send (message)
		size = self._get_count (self.sock)
		if (size != -1):
			response = self.sock.recv (size)
		else:
			return "No response"
		return response


if __name__ == "__main__":
	c = Client()
	print c.communicate()