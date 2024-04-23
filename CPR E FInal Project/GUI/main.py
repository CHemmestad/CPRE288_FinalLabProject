"""
Last Modified By:
Caleb Hemmestad
4/18/24

Discription: Used to send and recieve data from the CyBot and display the GUI
This program will map the area by using the information that the cybot sends it and then use the information to 
calculate where to move the cybot and then send the instructions to the cybot
This program is also responsible for displaying the information to the user using graphics
"""

import tkinter as tk
import math

def move_roomba(event):
    global cybotAngle, direction, cyBotX, cyBotY, leftSensor, rightSensor, topLeftSensor, topRightSensor, canvas
    key = event.keysym
    dx, dy = 0, 0
    if key == "w":
        dy = -gridSize * math.cos(math.radians(cybotAngle))  # Move forward
        dx = gridSize * math.sin(math.radians(cybotAngle)) 
    elif key == "a":
        cybotAngle = (cybotAngle - 5) # turns left
        if cybotAngle < 0:
            cybotAngle = 360
    elif key == "s":
        dy = gridSize * math.cos(math.radians(cybotAngle))   # Move backward
        dx = -gridSize * math.sin(math.radians(cybotAngle)) 
    elif key == "d":
        cybotAngle = (cybotAngle + 5)%360  # turns right
    elif key == "space":
        canvas_widget.create_oval((cyBotX*gridSize)-10 + 1, (cyBotY*gridSize)-10, (cyBotX*gridSize)+10 + 1, (cyBotY*gridSize)+10, outline="", fill='red')
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
    speed = 3
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
"""
import socket

# WiFi module IP address and port
wifi_module_ip = '192.168.1.1'
wifi_module_port = 288

# Create a socket object
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect to the WiFi module
s.connect((wifi_module_ip, wifi_module_port))

while True:
    # Get user input
    userInput = input('Enter Something: ')
    
    # Send user input to the socket
    s.sendall(userInput.encode())

    # Receive data from the socket
    data = s.recv(1024)
    
    # Check if there is no more data to receive
    if not data:
        break

    # Print the received data
    print("Received:", data.decode())

# Close the socket
s.close()
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

# 0 = empty, 1 = boundary, 2 = object, 3 = center of bot
canvas[10][10] = 1
canvas[11][11] = 1
canvas[12][11] = 2
canvas[13][11] = 2
canvas[166][210] = 2
canvas[146][190] = 2
canvas[146][210] = 2
canvas[166][190] = 2
canvas[1][1] = 2
canvas[cyBotY][cyBotX] = 3 # The center of the bots current location
canvas[rows - 1][cols - 1] = 2
gridSize = 3

# Need to measure the actaul radius of the bot still
botRadius = (36/2) # Each grid represents a 1x1cm block and the radius of the bot is 34/2cm

# Create the GUI window
root = tk.Tk()
root.title("Lawn Bot")

# Create a canvas widget
canvas_widget = tk.Canvas(root, width=cols*gridSize, height=rows*gridSize, bg="green")
canvas_widget.pack()

# Draw the circles on the canvas
for i in range(rows):
    for j in range(cols):
        if canvas[i][j] == 1:
            color = "white"
            canvas_widget.create_oval(j*gridSize, i*gridSize, (j+1)*gridSize, (i+1)*gridSize, outline="", fill=color)
        if canvas[i][j] == 2:
            color = "red"
            canvas_widget.create_oval(j*gridSize, i*gridSize, (j+1)*gridSize, (i+1)*gridSize, outline="", fill=color)
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