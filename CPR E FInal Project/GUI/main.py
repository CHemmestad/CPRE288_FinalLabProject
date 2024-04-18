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
