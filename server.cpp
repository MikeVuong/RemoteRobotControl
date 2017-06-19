
Server.cpp
#include <windows.networking.sockets.h>
#include <stdio.h>
#include <iostream>
#include "PKT_DEF.h"
using namespace std;

#pragma comment(lib, "ws2_32.lib")

void main()
{
	PktDef* packet = new PktDef();
	/*int userinput;
	cout << "Starting up TCP server" << endl;

	struct sockaddr_in SvrAddr;
	SOCKET WelcomeSocket, ConnectionSocket;
	WORD wVersionRequested;
	WSADATA wsaData;

	wVersionRequested = MAKEWORD(2, 2);
	if (WSAStartup(wVersionRequested, &wsaData) != 0)
		return;

	//create welcoming socket at port and bind local address
	if ((WelcomeSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return;

	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(5000);

	//bind the WelcomeSocket to the server
	if ((bind(WelcomeSocket, (struct sockaddr *)&SvrAddr, sizeof(SvrAddr))) == SOCKET_ERROR)
	{
		closesocket(WelcomeSocket);
		WSACleanup();
		return;
	}

	if (listen(WelcomeSocket, 1) == SOCKET_ERROR)
	{
		closesocket(WelcomeSocket);
		return;
	}

	cout << "Waiting for robot Connection" << endl;
	ConnectionSocket = SOCKET_ERROR;

	packet.RxBuffer = new char[128];
	packet.TxBuffer = new char[128];

	while (1) {
		if ((ConnectionSocket = accept(WelcomeSocket, NULL, NULL)) == SOCKET_ERROR) {
			return;
		}
		else{
			cout << "Connection Established!" << endl;

			while (1)
			{

				//Gets the initial command (Drive, Sleep or Get the Status)

				// GetUserInput(); 
				cout << "Enter a Packet ID ( 15 = Drive, 5 = Status , 0 = Sleep : ";
				cin >> userinput;
				S_Header.PacketId = (unsigned char)userinput;


				if (userinput == 15 || userinput == 5 || userinput == 0){

					if (userinput == 0){

						//Put the robot to sleep
						Sleep();

					}

					if (userinput == 5){
						getstat();
					}

					if (userinput == 15){

						//Move the robot
						//Gets the number of commands the user wants to enter
						NumCommands();

						//Gets each of the commands from the user
						DriveCommand();

						//calculates the parity
						CalcParity();

						//Sends the constructed packet to the robot
						send(ConnectionSocket, TxBuffer, cmdsize + 1, 0);
					}

				}
				else{
					cout << "Invalid command, choose between 0(sleep) 5(status) or 15(drive)." << endl;
				}


			}
			closesocket();
		}
	}
	WSACleanup(); */
}
