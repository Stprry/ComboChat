// ComboChat.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include "iostream"
#include <tchar.h>
#include "Client.cpp"
using namespace std;
/*Comment / UN Comment Server mode definition to toggle*/

//#define ServerMode
#if defined ServerMode
#define  CliToggle 0
#elif !defined Servermode
#define CliToggle 1
#endif 


DWORD WINAPI ServerThread(LPVOID lpParam) {
	SOCKET socket = (SOCKET)lpParam;
	int bytesSent;
	int bytesRecv = SOCKET_ERROR;
	char buffer[200] = "";

	while (1) {
		try
		{
			int sockErr = 0;
			bytesRecv = recv(socket, buffer, 200, 0);
			if (bytesRecv == SOCKET_ERROR) throw sockErr;
			else {
				cout << "Clients Message: " << buffer << endl;
				if (strcmp(buffer, "QUIT") == 0) {
					terminate();
					break;
				}
			}
		}
		catch (int sockErr)
		{
			cerr << "Server: recv() error " << WSAGetLastError() << endl;
		}

		char sendBuffer[200] = "Message Received";
		cout << "Enter Server Message: ";
		cin.getline(sendBuffer, 200);
		

		try
		{
			bytesSent = send(socket, sendBuffer, strlen(sendBuffer), 0);
			if (bytesSent == SOCKET_ERROR) cout << "Server: send() error " << WSAGetLastError() << endl;
			else cout << "Please Wait for a response form the Client. " << endl;
		}
		catch (const std::exception& e)
		{
			e.what();
		}
	}
	closesocket(socket);
	return EXIT_SUCCESS;
}


class StartUp {
public:
	int retval, fromlen, socket_type;
	int port = 55555;
	sockaddr_in service;
	SOCKET serverSocket, acceptSocket;
	SOCKADDR_STORAGE from;
	char servstr[NI_MAXSERV], hoststr[NI_MAXHOST];

	int winSockCheck() {
		int winSockFound;
		WSADATA wsaData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		winSockFound = WSAStartup(wVersionRequested, &wsaData);
		try
		{
			if (winSockFound = 1)
			{
				cout << "The Winsock dll found!" << endl;
				cout << "The status: " << wsaData.szSystemStatus << endl;
			}
		}
		catch (const std::exception& e)
		{
			cerr << e.what();
			try
			{
				throw e.what();

			}
			catch (...)
			{
				return-1;
			}
		}
	}

	int socketChecks() {
		service.sin_family = AF_INET;
		InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr.s_addr);
		service.sin_port = htons(port);
		serverSocket = INVALID_SOCKET;
		serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		try
		{
			if (serverSocket == INVALID_SOCKET)cerr << "Error at socket(): " << WSAGetLastError() << endl;
			else cout << "socket() is OK!" << endl;
		}
		catch (const std::exception& WSAGetLastError)
		{
			WSACleanup();
			return 0;
		}

		try
		{
			bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR;
		}
		catch (const std::exception& bind)
		{
			cerr << "bind() failed: " << WSAGetLastError() << endl;
			WSACleanup();
			return -1;
		}
		if (bind(serverSocket, (SOCKADDR*)&service, sizeof(service)) != SOCKET_ERROR)cout << "bind() is OK!" << endl;
		if (listen(serverSocket, 1) == SOCKET_ERROR)cout << "listen(): Error listening on socket " << WSAGetLastError() << endl;
		else cout << "listen() is OK, I'm waiting for connections..." << endl;

	}

	int ThreadID() {
		DWORD threadId;
		while (1) {
			acceptSocket = accept(serverSocket, NULL, NULL);
			try
			{
				bool err = 1;
				if (acceptSocket == INVALID_SOCKET) throw err;
				cout << "Server: Client Connected!" << endl;
				CreateThread(NULL, 0, ServerThread, (LPVOID)acceptSocket, 0, &threadId);
			}
			catch (bool err)
			{
				cerr << stderr << " accept failed: " << WSAGetLastError() << endl;
				return -1;
			}
		}
		system("pause");
		WSACleanup();
		return 0;
	}
};
int _tmain( _TCHAR*) {
	
	try
	{
		if (CliToggle == 1)
		{
			ClientSetUp client;
			client.winSockCheck();
			client.socketCheck();
			client.bufferFlow();
			system("pause");
			WSACleanup();
			return 0;
		}
		else
		{
			StartUp StartUp;
			StartUp.winSockCheck();
			StartUp.socketChecks();
			StartUp.ThreadID();
			WSACleanup();
			return 0;
		}
	}
	catch (const std::exception& e)
	{
		cerr << e.what();
	}
	
	

}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
