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

def client_socket():
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP)
    s.settimeout(3)
    s.connect(("127.0.0.1", 8880))
    return s

def fread(fileName):
    with open(fileName, "rb") as f:
        data = f.read()
    return data

data = fread("./MISC.bin")
data_length = (
    (20 - len(str(len(data)))) * "0" + str(len(data))
).encode()

s = client_socket()
s.sendall(data_length)

s.close()


