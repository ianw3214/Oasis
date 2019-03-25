#include "network.hpp"

#include "util/util.hpp"

bool Network::init() {
	// intialize winsock library
	WSADATA wsaData;
	int initResult;
	// the WSAStartup function initiates use of the winsock DLL by a process
	// first argument specifies highest version of sockets specification the caller can use
	// second argument is a data structure to recieve details of sockets implementation
	initResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	// check result = 0 for success
	if (initResult != 0) {
		ERR("WSAstartup failed. ERROR CODE: " << initResult);
		return false;
	}
	return true;
}

void Network::cleanup() {
	WSACleanup();
}

SOCKET Network::newSocket(const char* address, const char* port) {

	// addrinfo contains a sockaddr struct
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	// initialize the addrInfo structs
	ZeroMemory(&hints, sizeof(hints));  // fills a block of memory with 0s, similar to memset
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// resolve the server address and port
	int success = getaddrinfo(address, port, &hints, &result);
	if (success != 0) {
		ERR("getaddrinfo failed, ERROR CODE: " << success);
		return INVALID_SOCKET;
	}

	// then create the actual socket that will connect to the server
	SOCKET connectSocket = INVALID_SOCKET;
	// attempt to connect to the first address returned by the call to getaddrinfo
	ptr = result;
	// create a SOCKET for the connecting server
	connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	// error check to make sure it is a valid socket
	if (connectSocket == INVALID_SOCKET) {
		ERR("Error at socket(): " << WSAGetLastError());
		freeaddrinfo(result);
		return INVALID_SOCKET;
	}

	// actually connect to the server
	int connectResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
	if (connectResult == SOCKET_ERROR) {
		ERR("Couldn't connect to server...");
		closesocket(connectSocket);
		connectSocket = INVALID_SOCKET;
		return INVALID_SOCKET;
	}

	freeaddrinfo(result);

	// check if the socket is valid
	if (connectSocket == INVALID_SOCKET) {
		ERR("Unable to connect to server...");
		return INVALID_SOCKET;
	}

	return connectSocket;

}