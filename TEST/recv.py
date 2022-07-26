from socket import *
from hashlib import sha256 as SHA_256
from os import urandom as randbytes
from random import randint

''' MISC.bin: sha256
a0b7b95dd219016c10a9eb0f7c840a5ea5a701635ede09047a5183e3e03d4e39
'''

def sha256(data :bytes):
    a = SHA_256()
    a.update(data)
    return a.hexdigest()

def server_socket():
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)
    s.setsockopt(SOL_SOCKET, SO_REUSEADDR, 1)
    s.bind(("0.0.0.0", 8880))
    s.listen(5)
    s,addr=s.accept()
    return s

def fread(fileName):
    with open(fileName, "rb") as f:
        data = f.read()
    return data

def recv(s :socket):
    data_length = int(s.recv(20))
    res = b""
    while data_length != 0:
        pass

s = server_socket()
recv(s)
s.close()




