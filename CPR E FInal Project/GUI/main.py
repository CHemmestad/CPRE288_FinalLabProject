"""
Last Modified By:
Caleb Hemmestad
4/18/24

Discription: Used to send and recieve data from the CyBot and display the GUI
This program will map the area by using the information that the cybot sends it and then use the information to 
calculate where to move the cybot and then send the instructions to the cybot
This program is also responsible for displaying the information to the user using graphics
"""
import socket
import tkinter as tk
import math

def move_roomba(event):
    global cybotAngle, direction, cyBotX, cyBotY, leftSensor, rightSensor, topLeftSensor, topRightSensor, canvas
    key = event.keysym
    dx, dy = 0, 0
    if key == "m":
        s.sendall(key.encode())
        angle = int(s.recv(3))
        distance = int(s.recv(2)) - 3
        print(angle)
        print(distance)
        while int(angle) < 180:
            offset = 11
            angle = int(s.recv(3))
            distance = int(s.recv(2)) - 3
            angleAdj = angle*-1 + 90
            newAngle = (cybotAngle + 360 + angleAdj)%360
            if distance < 30:
                canvas_widget.create_oval(
                (cyBotX + offset*math.sin(math.radians(cybotAngle)) + 4*math.sin(math.radians(newAngle)) + distance*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - offset*math.cos(math.radians(cybotAngle)) - 4*math.cos(math.radians(newAngle)) - distance*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + offset*math.sin(math.radians(cybotAngle)) + 4*math.sin(math.radians(newAngle)) + distance*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - offset*math.cos(math.radians(cybotAngle)) - 4*math.cos(math.radians(newAngle)) - distance*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='red')
            print(angle)
            print(distance)
    elif key == "p":
        s.sendall(key.encode())
    elif key == "c":
        s.sendall(key.encode())
    elif key == "w":
        s.sendall(key.encode())
        hit = s.recv(1).decode()
        cybotAngle = int(s.recv(3))
        distance = int(s.recv(3))/10
        dy = -distance * gridSize * math.cos(math.radians(cybotAngle))  # Move forward
        dx = distance * gridSize * math.sin(math.radians(cybotAngle)) 
        if hit == '0':
            adj = 15
            angleAdj = -68
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='white')
        elif hit == '1':
            adj = 15
            angleAdj = -20
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='white')
        elif hit == '2':
            adj = 15
            angleAdj = 20
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='white')
        elif hit == '3':
            adj = 15
            angleAdj = 68
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='white')
        elif hit == '4':
            adj = 15
            angleAdj = -68
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='black')
        elif hit == '5':
            adj = 15
            angleAdj = -20
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='black')
        elif hit == '6':
            adj = 15
            angleAdj = 20
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='black')
        elif hit == '7':
            adj = 15
            angleAdj = 68
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='black')
        elif hit == '8':
            adj = 17
            angleAdj = 0
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='red')
        elif hit == '9':
            adj = 17
            angleAdj = -45
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='red')
        elif hit == 'a':
            adj = 17
            angleAdj = 45
            newAngle = (cybotAngle + 360 + angleAdj)%360
            canvas_widget.create_oval(
                (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
                (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
                outline="", fill='red')

    elif key == "a":
        s.sendall(key.encode())
        cybotAngle = int(s.recv(3))
    elif key == "s":
        s.sendall(key.encode())
        cybotAngle = int(s.recv(3))
        distance = int(s.recv(3))/-10
        dy = distance * gridSize * math.cos(math.radians(cybotAngle))   # Move backward
        dx = -distance * gridSize * math.sin(math.radians(cybotAngle)) 
    elif key == "d":
        s.sendall(key.encode())
        cybotAngle = int(s.recv(3))
        #cybotAngle = (cybotAngle - int(s.recv(3)))%360  # turns right
    print(cybotAngle)
    canvas_widget.delete(direction)
    canvas_widget.delete(leftSensor)
    canvas_widget.delete(rightSensor)
    canvas_widget.delete(topLeftSensor)
    canvas_widget.delete(topRightSensor)
    direction = canvas_widget.create_polygon(
        (cyBotX + 1 - math.cos(math.radians(cybotAngle))*5)*gridSize, (cyBotY - math.sin(math.radians(cybotAngle))*5)*gridSize, 
        (cyBotX + 1 + math.sin(math.radians(cybotAngle))*10)*gridSize , (cyBotY - math.cos(math.radians(cybotAngle))*10)*gridSize, 
        (cyBotX + 1 + math.cos(math.radians(cybotAngle))*5)*gridSize, (cyBotY + math.sin(math.radians(cybotAngle))*5)*gridSize, 
        fill='blue')
    adj = 15
    angleAdj = -68
    newAngle = (cybotAngle + 360 + angleAdj)%360
    leftSensor = canvas_widget.create_oval(
        (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
        (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
        outline="", fill='yellow')
    adj = 15
    angleAdj = 68
    newAngle = (cybotAngle + 360 + angleAdj)%360
    rightSensor = canvas_widget.create_oval(
        (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
        (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
        outline="", fill='yellow')
    adj = 15
    angleAdj = -20
    newAngle = (cybotAngle + 360 + angleAdj)%360
    topLeftSensor = canvas_widget.create_oval(
        (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
        (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
        outline="", fill='yellow')
    adj = 15
    angleAdj = 20
    newAngle = (cybotAngle + 360 + angleAdj)%360
    topRightSensor = canvas_widget.create_oval(
        (cyBotX + adj*math.sin(math.radians(newAngle)))*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)))*gridSize, 
        (cyBotX + adj*math.sin(math.radians(newAngle)) + 1)*gridSize, (cyBotY - adj*math.cos(math.radians(newAngle)) + 1)*gridSize, 
        outline="", fill='yellow')
    speed = 1
    dy *= speed
    dx *= speed
    canvas[int(cyBotY)][int(cyBotX)] = 0
    cyBotY += dy/gridSize
    cyBotX += dx/gridSize
    if cyBotX >= cols:
        cyBotX = cols-1
        dx = 0
    elif cyBotX < 0:
        cyBotX = 0
        dx = 0
    if cyBotY >= rows:
        cyBotY = rows-1
        dy = 0
    elif cyBotY < 0:
        cyBotY = 0
        dy = 0
    canvas[int(cyBotY)][int(cyBotX)] = 3
    canvas_widget.move(roomba, dx, dy)
    canvas_widget.move(direction, dx, dy)
    canvas_widget.move(leftSensor, dx, dy)
    canvas_widget.move(rightSensor, dx, dy)
    canvas_widget.move(topLeftSensor, dx, dy)
    canvas_widget.move(topRightSensor, dx, dy)

# WiFi module IP address and port
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
"""

rows, cols = 300, 500
canvas = [[0 for _ in range(cols)] for _ in range(rows)]

"""
# Define the radius and center of the circle
radius = 5
center = (50, 50)

# Draw the circle in the canvas
for i in range(rows):
    for j in range(cols):
        if (i - center[0])**2 + (j - center[1])**2 <= radius**2:
            canvas[i][j] = 1
"""

# flags for later
moving = 0
scanning = 0

# Variables to keep track of cause this is going to take a lot of trig
cybotAngle = 0.0
sensorAngle = 0.0
cyBotX = 27
cyBotY = 273
cyBotX = 40
cyBotY = 260

# 0 = empty, 1 = boundary, 2 = object, 3 = center of bot
"""
canvas[10][10] = 1
canvas[11][11] = 1
canvas[12][11] = 2
canvas[13][11] = 2
canvas[166][210] = 2
canvas[146][190] = 2
canvas[146][210] = 2
canvas[166][190] = 2
canvas[1][1] = 2
"""
canvas[cyBotY][cyBotX] = 3 # The center of the bots current location
gridSize = 3

# Need to measure the actaul radius of the bot still
botRadius = (34/2) # Each grid represents a 1x1cm block and the radius of the bot is 34/2cm

# Create the GUI window
root = tk.Tk()
root.title("Lawn Bot")

# Create a canvas widget
canvas_widget = tk.Canvas(root, width=cols*gridSize, height=rows*gridSize, bg="green")
canvas_widget.pack()

# Draw the circles on the canvas
for i in range(rows):
    for j in range(cols):
        """
        if canvas[i][j] == 1:
            color = "white"
            canvas_widget.create_oval(j*gridSize, i*gridSize, (j+1)*gridSize, (i+1)*gridSize, outline="", fill=color)
        if canvas[i][j] == 2:
            color = "red"
            canvas_widget.create_oval(j*gridSize, i*gridSize, (j+1)*gridSize, (i+1)*gridSize, outline="", fill=color)
            """
        if canvas[i][j] == 3:
            color = "black"
            roomba = canvas_widget.create_oval((j - botRadius)*gridSize, (i - botRadius)*gridSize, (j + botRadius + 1)*gridSize, (i + botRadius + 1)*gridSize, outline="", fill=color)
            direction = canvas_widget.create_polygon((j - 4 + cybotAngle)*gridSize, i*gridSize, (j + 1)*gridSize , (i - 10)*gridSize, (j + 6)*gridSize, i*gridSize, fill='blue')
            adjX = -15
            adjY = 0
            leftSensor = canvas_widget.create_oval((j + adjX)*gridSize, (i + adjY)*gridSize, (j + adjX + 1)*gridSize, (i + adjY + 1)*gridSize, outline="", fill='yellow')
            adjX = +15
            adjY = 0
            rightSensor = canvas_widget.create_oval((j + adjX)*gridSize, (i + adjY)*gridSize, (j + adjX + 1)*gridSize, (i + adjY + 1)*gridSize, outline="", fill='yellow')
            adjX = -4
            adjY = -14.5
            topLeftSensor = canvas_widget.create_oval((j + adjX)*gridSize, (i + adjY)*gridSize, (j + adjX + 1)*gridSize, (i + adjY + 1)*gridSize, outline="", fill='yellow')
            adjX = 4
            adjY = -14.5
            topRightSensor = canvas_widget.create_oval((j + adjX)*gridSize, (i + adjY)*gridSize, (j + adjX + 1)*gridSize, (i + adjY + 1)*gridSize, outline="", fill='yellow')

root.bind("<KeyPress>", move_roomba)

# Start the GUI event loop
root.mainloop()
s.close()