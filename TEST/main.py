from hexdump import hexdump

def fread(filename):
    with open(filename, "rb") as f:
        data = f.read()
    return data

def fwrite(filename, data):
    with open(filename, "wb") as f:
        f.write(data)

import hashlib
a = hashlib.sha256()
a.update(fread("./MISC.bin"))
print(a.hexdigest())


