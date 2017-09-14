# Import socket module
import socket
# Create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
port = 12345 # port on which you want to connect
host = socket.gethostname()
print(host)
s.connect((host, port)) #connect to the server on local machine
print(s.recv(1024))# receive data from the server
# close the connection
s.close()
