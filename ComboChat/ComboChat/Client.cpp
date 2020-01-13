#include "pch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include <tchar.h>

using namespace std;

class ClientSetUp {
public:
	SOCKET clientSocket;
	int port = 55555;
	int max = 2;


	int winSockCheck() {
		WSADATA wsaData;
		int winSockPresent;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int err = 1;


		winSockPresent = WSAStartup(wVersionRequested, &wsaData);
		try
		{
			if (winSockPresent != 0) throw err;
			cout << "The Winsock dll found!" << endl;
			cout << "The status: " << wsaData.szSystemStatus << endl;
		}
		catch (int err) {
			cerr << "The Winsock dll not found!" << endl;
			return 0;
		}
	}

	int socketCheck() {
		//bool err = 1;
		clientSocket = INVALID_SOCKET;
		clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
		clientService.sin_port = htons(port);
		int more = max;

		if (clientSocket == INVALID_SOCKET) {
			cerr << "Error at socket(): " << WSAGetLastError() << endl;
			cout << " Re attempting " << more << " more times.";
			for (int i = 0; i < max; i++)
			{
				socketCheck();
				more--;
				if (max == 0)
				{
					cout << "Exiting program failed to find socket";
					WSACleanup();
					return 0;
				}
			}
		}
		else {
			cout << "socket() is OK!" << endl;
		}

		if (connect(clientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
			cout << "Client: connect() - Failed to connect. Aborting program" << endl;
			terminate();
		}
		else {
			cout << "Client: connect() is OK." << endl;
			cout << "Client: Can start sending and receiving data..." << endl;
		}
	}

	void bufferFlow() {
		int byteCount = SOCKET_ERROR;
		char buffer[200];
		char receiveBuffer[200] = "";

		while (1) {
			cout << "Enter your message ";
			cin.getline(buffer, 200);
			byteCount = send(clientSocket, buffer, 200, 0);
			if (byteCount == SOCKET_ERROR) {
				cout << "Client: send() error " << WSAGetLastError() << endl;
			}
			else {
				cout << "Client: sent " << byteCount << " bytes" << endl;
				if (strcmp(receiveBuffer, "QUIT") == 0) {
					//closesocket(clientSocket);
						terminate();
				}
			}
			byteCount = recv(clientSocket, receiveBuffer, 200, 0);

			if (byteCount == 0 || byteCount == WSAECONNRESET) cout << "Client: Connection Closed." << endl;
		
			if (byteCount < 0) cout << "Client: error " << WSAGetLastError() << endl;
			else cout << "From Server :  " << receiveBuffer << endl;
		}
	};
};
