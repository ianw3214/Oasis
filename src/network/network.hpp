#pragma once

#include <winsock2.h>
#include <Ws2tcpip.h>

#include <optional>

namespace Network {

	bool init();
	void cleanup();

	SOCKET newSocket(const char* address, const char* port);

	// generic function for sending messages
	template<class T>
	inline bool sendMessage(SOCKET fd, T message) {
		if (send(fd, (char*)&message, sizeof(T), 0) < 0) {
			// error handling here
			return false;
		}
		// handle success here
		return true;
	}

	// generic function for recieving messages
	template<class T>
	inline std::optional<T> getMessage(SOCKET fd) {
		T message;
		if (recv(fd, (char*)&message, sizeof(T), 0) < 0) {
			// error handling here
			return {};
		}
		// handle success here
		return message;
	}

};