# RemoteRobotControl

This assignment required students to develop code using socket programming to remotely send commands and control a robot.

The following tasks were required of the developer.

The basis of the code is based on a client server interaction, therefore both server and client have to be created independently.

Server
1. Server socket had to be created and binded to an address
2. After binding the server waits for any incoming connections from potentional clients
3. After the connection is established the server gives the following options to the client and waits for further input, Enter a Packet ID ( 15 = Drive, 5 = Status , 0 = Sleep)
4. Upon recieving the client command the server interprets the information and returns the socket and awaits for the movement commands containing the direction and duration buffer from the client
5. After recieving the information the server is put on standby and awaits for further inputs from the user

Client 
1. The client creates a client socket and connects to the server to begin inputting buffered commands through the socket 
2. Depending on the client input they have the option to choose from the following commands ( 15 = Drive, 5 = Status , 0 = Sleep)  
   - 3a. For 0 or sleep it removes all currently buffered commands and puts the machine in a standby state  
   - 3b. For 5 or status it returns the currently inputted commands  
   - 3c. For 15 or drive it returns with a sub menu asking for both the amount of commands the user wishes to input and the type of commmand that the user wishes to use to direct the robot, the following options are available ( 1 Forward , 2 Backwards , 3 Left , 4 Right)   
4. After all the commands are completed and the robot is on standby the user has the option to disconnect the connection or continue to send commands
