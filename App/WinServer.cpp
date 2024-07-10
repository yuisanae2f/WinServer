
#include <winsock2.h>
#include <ws2tcpip.h>

#include "WinServer.h"
#include "ThreadFIle.h"
#pragma comment(lib, "ws2_32.lib")

std::string httpValidHeader(size_t content_length) {
	return
		std::string("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: ")
		+ std::to_string(content_length)
		+ "\r\nAccess-Control-Allow-Origin: *\r\n\r\n";
}

static void Send(SOCKET socket, const char* buf, int len) {
	Trial("[Trial] Sending bytes of " << len << "\n");

	int s = send(socket, buf, len, 0);

	if (s == len) {
		Trial("[Send] Data sent in " << s << " bytes\n");
	}
	else {
		Err("[Error] Data has been lost (" << len << ", " << s << ")\n");
	}
}

void __Buffer::clear() {
	if (s == -1) {
		memset(raw, 0, SizeOnTrial);
	}
	memset(raw, 0, s);
	s = 0;
}

std::unordered_map<std::string, ThreadFIle> WinServer::Files;

static void Main(
	bool* ___Run,
	bool* ___Running
#if !lURL
	,const char* endpoint
#endif // lURL
#if !lPORT
	,const char* port
#endif // !lPORT
)
{
	HWND hWnd = GetForegroundWindow();
	ShowWindow(hWnd, SW_SHOW);

	const char* endpoint = URL;
	Trial("[Trial] Server booting trial, endpoint: " << endpoint << '\n');
	LogDump();

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	struct addrinfo hints;
	struct addrinfo* result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	Trial("[Trial] getting addr info\n");
#if lPORT
	Trial("[Input] Thy port would be: " << PORT << "\n");
	int status = getaddrinfo(endpoint, PORT, &hints, &result);
#else
	int status = getaddrinfo(endpoint, port, &hints, &result);
#endif

	Trial("\n");
	if (status) {
		Err("[Error] get addr has been failed. status: " << status << "\n");
		LogDump();
		WSACleanup();
		___Running[0] = 0;
		return;
	}


	Trial(
		"[Trial] addr has extracted\n"
		<< result->ai_addr->sa_data
		<< "\n";
	);

	Trial("[Trial] implementing result on addr (sin_family)\n");
	LogDump();
	struct sockaddr_in* addr = (struct sockaddr_in*)result->ai_addr;
	addr->sin_family = AF_INET;

	Trial("[Trial] socket call... \n");
	SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (listenSocket == INVALID_SOCKET) {
		Err("[Error] socket creation failed. error: " << WSAGetLastError() << "\n");
		LogDump();
		freeaddrinfo(result);
		WSACleanup();
		___Running[0] = 0;
		return;
	}

	Trial("[Success] Socket call succeed!\n");
	LogDump();

	Trial("[Trial] binding socket...\n");
	if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		Err("[Error] binding socket failed. error: " << WSAGetLastError() << "\n");
		LogDump();
		closesocket(listenSocket);
		freeaddrinfo(result);
		WSACleanup();
		___Running[0] = 0;
		return;
	}
	Trial("[Trial] binding complete!\n");
	Trial("[Trial] listening...\n");
	LogDump();
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		Err("[Error] listening failed. error: " << WSAGetLastError() << "\n");
		LogDump();
		closesocket(listenSocket);
		freeaddrinfo(result);
		WSACleanup();
		___Running[0] = 0;
		return;
	}
	Trial(
			"[Trial] listening complete!\n" 
		<<	"[Init] Server init has done! The loop shall start...\n"
	);
	LogDump();

#if DbgTag(eLog_Console)
	ShowWindow(hWnd, SW_SHOW);
#else
	ShowWindow(hWnd, SW_HIDE);
#endif // _Debug

	// 클라이언트 연결 대기 및 처리 루프
	while (___Run[0]) {
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			Err("[Error] accepting client failed. error: " << WSAGetLastError() << "\n");
			LogDump();
			continue;
		}

		Trial("[Trial] client connected!\n");
		LogDump();

#pragma region connected
		int len = 1;
		do {
			__Buffer __Reader;

			union Buff {
				char* buff;
				int* v;
			} _Reader = { __Reader.raw };
			len = recv(clientSocket, _Reader.buff, 4, 0);

			Trial("Recv got in " << len << " bytes...\n");

			bool hasGotPath = false;
			std::string path_access;

			switch (_Reader.v[0]) {
			default: { break; }
			case NumGen8<'P', 'O', 'S', 'T'>::val: {
				Trial("[Trial] Server got POST method\n");
#if 0
				POST /path HTTP / 1.1
					Host: example.com
					Content - Type : application / x - www - form - urlencoded
					Content - Length : 23
					;
#endif 
				std::string BodyWithHead, __Path;
				do {
					__Reader.clear();

					if ((__Reader.s = recv(clientSocket, __Reader.raw, SizeOnTrial, 0)) != -1) {
						BodyWithHead += std::string(__Reader.raw, __Reader.s);
						if(BodyWithHead.size() > 2)
						for (size_t i = 2; i < BodyWithHead.size(); i++) {
							if (BodyWithHead[i] == ' ') {
								__Path = std::string(BodyWithHead.c_str() + 2, BodyWithHead.c_str() + i);
								BodyWithHead = std::string(BodyWithHead.c_str() + i, BodyWithHead.size() - i);
								hasGotPath = true;
								break;
							}
						}
					}
				} while (__Reader.s && __Reader.s != -1 && !hasGotPath);

				if (hasGotPath) {
					WinServer::Files[__Path].WriteJSON_Sock(__Path, clientSocket, BodyWithHead);
				}
			} break;

			case NumGen8<'G', 'E', 'T', ' '>::val: {
				Trial("[Trial] Server got GET method\n");

				if (!hasGotPath) {
					std::string BodyWithHead;

					do {
						__Reader.clear();
						__Reader.s = recv(clientSocket, __Reader.raw, SizeOnTrial, 0);
						BodyWithHead += std::string(__Reader.raw, __Reader.s);
					} while (__Reader.full());
					std::istringstream pathReader(BodyWithHead);
					pathReader >> path_access;
					path_access = path_access.c_str() + 1;
				}

				Trial("[Trial] Path accessing trial: " << path_access << "\n");
				std::string Buff = FILE_READ(path_access);
				std::string Header = httpValidHeader(Buff.size());
				Send(clientSocket, Header.c_str(), Header.size());
				for (uint64_t i = 0; i < Buff.size(); i += SizeOnTrial) {
					Send(clientSocket, Buff.c_str() + i, Buff.size() > i + SizeOnTrial ? SizeOnTrial : Buff.size() - i);
				}

				closesocket(clientSocket);
			} break;
			}
			__Reader.clear();
		} while (0);
#pragma endregion
		Trial("[Response] Done. Closing the socket...\n");
		LogDump();
	}


	Trial("[Trial] Freeing resources\n");
	closesocket(listenSocket);
	freeaddrinfo(result);
	WSACleanup();

	LogDump();
	___Running[0] = 0;
	return;
}


void WinServer::Main(
#if !lURL
	const char* endpoint, // ip or domain to input
#endif // lURL
#if !lPORT
	const char* port,
#endif // !lPORT
	bool toDetach
) {
	// states
	this->__Run = 1;
	this->Running = 1;

	// asks if you want to detach the thread after making it
	if (toDetach) {
		std::thread(
			::Main,
			&this->__Run,
			&this->Running
#if !lURL
			, endpoint
#endif // lURL
#if !lPORT
			, port
#endif // !lPORT
		).detach();
	}
	else {
		std::thread(
			::Main,
			&this->__Run,
			&this->Running
#if !lURL
			, endpoint
#endif // lURL
#if !lPORT
			, port
#endif // !lPORT
		).join();
	}
}