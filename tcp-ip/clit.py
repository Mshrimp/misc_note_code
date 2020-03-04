#!/usr/bin/python

#client

import socket

address = ("127.0.0.0", 31500)
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(address)

data = s.recv(512)
print("The data recv is: ", data)

s.send("Hi")
s.close()
