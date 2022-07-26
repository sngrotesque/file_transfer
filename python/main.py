from transfer import *
from hashlib import sha256 as s256

def sha256(data):
    a = s256()
    a.update(data)
    return a.hexdigest()

with open("./out/bilibili_2233.jpeg", "rb") as f:
    data = f.read()

print(sha256(data))

