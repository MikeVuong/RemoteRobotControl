PKT_DEF.cpp
#include <windows.networking.sockets.h>
#include "PKT_DEF.h"
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")

void PktDef::CalcParity(){
	cmdsize = (cmdsize * 2) + 2;
	short unsigned int parity = 0;

	for (int i = 0; i < cmdsize; i++){
		int value = (unsigned int)TxBuffer[i];
		parity = parity + BinaryCounter(value);
	}
	TxBuffer[cmdsize] = parity;
}


int PktDef::BinaryCounter(unsigned int result){
	int i;
	for (i = 0; result != 0; i++, result &= result - 1);
	return i;
}

void PktDef::NumCommands(){
	cout << "Enter number of drive commands to robot (1-10) : ";
	cin >> cmdsize;
	S_Header.CmdListSize = (unsigned char)cmdsize;
	S_Body.Direction = new unsigned char[cmdsize];
	S_Body.Duration = new unsigned char[cmdsize];
}

void PktDef::DriveCommand(){
	char * UserInput = new char[cmdsize * 2] ;
	int Di, Du;
	for (int i = 0; i < cmdsize; i++){
		cout << "Enter in the Command ( 1 Forward , 2 Backwards , 3 Left , 4 Right) : ";
		cin >> Di;
		UserInput++;
		S_Body.Direction[i] = (unsigned int)Di;
		cout << "Enter the duration of the command : ";
		cin >> Du;
		UserInput++;
		S_Body.Duration[i] = (unsigned int)Du;
	}

	TxBuffer[0] = S_Header.PacketID;
	TxBuffer[1] = S_Header.CmdListSize;

	int location = 0;
	for (int l = 0; l < cmdsize * 2; l++){
		TxBuffer[l + 2] = UserInput[location];
		location++;
	}

}
void PktDef::Drive(){
	NumCommands();
	DriveCommand();
	CalcParity();

}

PktDef::PktDef(){
	int userinput;
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

	RxBuffer = new char[128];
	TxBuffer = new char[128];

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
				S_Header.PacketID = (unsigned char)userinput;


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
			closesocket(ConnectionSocket);
		}
	}
	WSACleanup();
}

void PktDef::Sleep(){
	cout << "Putting the robot to sleep..." << endl;
	cmdsize = 0;
	S_Header.CmdListSize = (unsigned char)cmdsize;
	S_Body.Direction = new unsigned char[cmdsize];
	S_Body.Duration = new unsigned char[cmdsize];
	DriveCommand();
	//calculates the parity
	CalcParity();

	//Sends the constructed packet to the robot
	send(ConnectionSocket, TxBuffer, cmdsize + 1, 0);
	closesocket(ConnectionSocket);
	cout << "Connection termiated." << endl;
	WSACleanup();
}

void PktDef::getstat()
{
	cmdsize = 0;
	S_Header.CmdListSize = (unsigned char)cmdsize;
	S_Body.Direction = new unsigned char[cmdsize];
	S_Body.Duration = new unsigned char[cmdsize];
	DriveCommand();
	//calculates the parity
	CalcParity();

	//Sends the constructed packet to the robot
	send(ConnectionSocket, TxBuffer, cmdsize + 1, 0);
	//Gets the status response from the robot
	cmdsize = 0;
	S_Header.CmdListSize = (unsigned char)cmdsize;
	S_Body.Direction = new unsigned char[cmdsize];
	S_Body.Duration = new unsigned char[cmdsize];
	DriveCommand();
	//calculates the parity
	CalcParity();

	//Sends the constructed packet to the robot
	send(ConnectionSocket, TxBuffer, cmdsize + 1, 0);

	//Gets the status response from the robot
	recv(ConnectionSocket, RxBuffer, 128, 0);

	R_Packet.PacketId = RxBuffer[0];
	R_Packet.CmdListSize = RxBuffer[1];
	R_Packet.StatusData = new unsigned char[(int)R_Packet.CmdListSize];
	int index = 0;
	for (int i = 2; i < (int)R_Packet.CmdListSize + 2; i++){
		R_Packet.StatusData[index] = RxBuffer[i];
		index++;
	}
	R_Packet.Parity = RxBuffer[12];
	cout << "*Response Data*" << endl;
	cout << "PacketId: " << (int)R_Packet.PacketId << endl;
	cout << "Size: " << (int)R_Packet.CmdListSize << endl;

	for (int i = 0; i < (int)R_Packet.CmdListSize; i++){
		cout << "Body element " << i + 1 << ": " << (int)R_Packet.StatusData[i] << endl;
	}
	cout << "Parity: " << (int)R_Packet.Parity << endl;

}

void PktDef::Sendx(SOCKET ConnectionSocket){
	send(ConnectionSocket, TxBuffer, cmdsize + 1, 0);
}




