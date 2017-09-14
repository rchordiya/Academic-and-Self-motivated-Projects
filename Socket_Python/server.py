
import socket
#socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
print("Socket successfully created")
# reserve a port
host = socket.gethostname()
port = 12345
s.bind(('', port)) #empty string in ip field to make server listen request in network
print("socket binded to",port)
# put the socket into listening mode
s.listen(5)
print("socket is listening")
while True:
    # Establish connection with client.
    c, addr = s.accept()
    print("Got connection from", addr)
    c.sendall(b'Message from server') #send message to client
    c.close() #close connection with client
