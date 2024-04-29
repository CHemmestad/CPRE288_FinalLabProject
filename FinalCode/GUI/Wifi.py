import socket

host = '192.168.1.1'
port = 288

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

#s.sendall(b'm')

"""
while True:
    userInput = input('Enter something: ')
    s.sendall(userInput.encode())
    if userInput == "z":
        break
"""
userInput = input('Enter something: ')
s.sendall(userInput.encode())
while True:
    angle = s.recv(3)
    distance = s.recv(2)
    if angle.decode() == "s":
        print('Broke')
        break
    print(angle.decode())
    print(distance.decode())

s.close