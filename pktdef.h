
PKT_DEF.H
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

struct send_packetheader {
	unsigned char PacketID;
	unsigned char CmdListSize;
	unsigned char Parity;
};

struct DriveCmdList{
	unsigned char * Direction;
	unsigned char * Duration;
};

struct Receive_Packet{
	unsigned char PacketId;
	unsigned char CmdListSize;
	unsigned char* StatusData;
	unsigned char Parity;
};

class PktDef {
	
	send_packetheader S_Header;
	DriveCmdList	S_Body;
	Receive_Packet R_Packet;
	

public:
	PktDef();
	void CalcParity(); // Get Parity
	int BinaryCounter(unsigned int); // Count ints for parity (Parity functions calls this)
	void NumCommands();
	void DriveCommand();
	void Display();
	char * TxBuffer;
	char * RxBuffer;
	int cmdsize;
	void Sleep();
	void Status();
	void Drive();
	void getstat();
	void Sendx(SOCKET);
	SOCKET ConnectionSocket, WelcomeSocket;

};

