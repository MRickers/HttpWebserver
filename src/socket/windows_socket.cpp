#include <socket/windows_socket.h>
#include <logging/logging.h>
#include <util/util.h>

static WSADATA wsadata = { 0x00 };

namespace webserver::sock {
	static std::string printWindowsErrorMessage(int errorcode) {
		char* error_message = { 0x00 };

		if (!FormatMessage(
			FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
			NULL,
			errorcode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&error_message,
			0,
			NULL)) {
			throw SocketException{ "FormatMessage failed.", (int16_t)GetLastError() };
		}
		const auto string_len = strlen(error_message);
		std::string temp;
		temp.resize(string_len, ' ');
		temp.insert(0, error_message);
		return temp;
	}

    webserver::sock::SocketResult convertToResult(int wsa_error) {
        switch (wsa_error) {
        case 0:
            return webserver::sock::SocketResult::OK;
        case WSANOTINITIALISED:
            return webserver::sock::SocketResult::NOTINITIALISED;
        case WSAENETDOWN:
            return webserver::sock::SocketResult::NETDOWN;
        case WSAENOTCONN:
            return webserver::sock::SocketResult::NOTCONNECTED;
        case WSAENOTSOCK:
            return webserver::sock::SocketResult::NOSOCKET;
        case WSAESHUTDOWN:
            return webserver::sock::SocketResult::SHUTDOWN;
        case WSAECONNABORTED:
            return webserver::sock::SocketResult::CONNABORTED;
        case WSAETIMEDOUT:
            return webserver::sock::SocketResult::TIMEDOUT;
        case WSAECONNRESET:
            return webserver::sock::SocketResult::CONNRESET;
        default:
            return webserver::sock::SocketResult::DEFAULTERROR;
        }
    }

	void ISocketTcp::Init() {
		if (WSAStartup(WINSOCK_VERSION, &wsadata) != NO_ERROR) {
			lLog(lError) << "WSAStartup failed: " << printWindowsErrorMessage(WSAGetLastError());
			throw SocketException{ "Init socket failed.", -1 };
		}
	}

	void ISocketTcp::CleanUp() {
		if (WSACleanup() == SOCKET_ERROR) {
			lLog(lError) << "WSACleanup failed: " << printWindowsErrorMessage(WSAGetLastError());
			throw SocketException{ "WSACleanup failed.", -1 };
		}
	}

	WindowsSocketTcp::WindowsSocketTcp() : WindowsSocketTcp("", 0) {
		
	}

    WindowsSocketTcp::WindowsSocketTcp(socket_t sock, const std::string& host, uint32_t port) : 
        socket_(sock),
        port_(port),
        host_(host)    {
        
    }

	WindowsSocketTcp::WindowsSocketTcp(const std::string& host, uint32_t port) : 
        socket_(),
        port_(port),
        host_(host)
    {
        socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ == INVALID_SOCKET) {
            lLog(lError) << "Create socket failed: " << printWindowsErrorMessage(WSAGetLastError());
            throw SocketException{ "Create socket failed.", (int16_t)WSAGetLastError() };
        }
	}

    

	void WindowsSocketTcp::Connect(const std::string& host, uint32_t port) {
        ConnectTimeout(host, port, 1000);
	}

	void WindowsSocketTcp::ConnectTimeout(const std::string& host, uint32_t port, uint32_t timeout_ms) {
        struct addrinfo hints;
        struct addrinfo* result = NULL;
        struct addrinfo* rp = NULL;
        char portnumber_string[33];
        int ret, arg;
        struct timeval tv = { 0x00 };
        unsigned long mode = 1;
        fd_set myset;
        int valopt;
        socklen_t lon;

        if (timeout_ms > 0) {
            tv.tv_usec += timeout_ms * 1000;
            tv.tv_sec += tv.tv_usec / 1000000;
            tv.tv_usec %= 1000000;
        }

        snprintf(portnumber_string, 33, "%d", port);

        // Struct-Definition siehe https://docs.microsoft.com/en-us/windows/win32/api/ws2def/ns-ws2def-addrinfoa 
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET; // Nur IPv4
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = 0;
        hints.ai_protocol = IPPROTO_TCP;

        // Set non-blocking 

        if ((arg = ioctlsocket(socket_, FIONBIO, &mode)) != 0) {
            lLog(lError) << "ConnectTimeout error: " << printWindowsErrorMessage(WSAGetLastError());
            Close();
            throw SocketException{ "ConnectTimeout error.", (int16_t)WSAGetLastError() };
        }

        ret = getaddrinfo(host_.c_str(), portnumber_string, &hints, &result);
        if (ret != 0) {
            Close();
            freeaddrinfo(result);
            lLog(lError) << "ConnectTimeout error, DNS failure for " << host_ << " on port " << port_ <<
                " ," << printWindowsErrorMessage(WSAGetLastError());
            throw SocketException{ "DNS failure.", (int16_t)WSAGetLastError() };
        }

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            if ((ret = connect(socket_, rp->ai_addr, (int)rp->ai_addrlen)) != -1)
                break;                  /* Success */

            if (ret == SOCKET_ERROR) {   // Non-blocking always returns SOCKET_ERROR
                if (WSAEWOULDBLOCK == WSAGetLastError()) {
                    do {
                        FD_ZERO(&myset);
                        FD_SET(socket_, &myset);
                        ret = select(0, NULL, &myset, NULL, (timeout_ms > 0 ? &tv : NULL)); //nfds (1. Parameter) wird unter Windows ignoriert
                        if (ret == SOCKET_ERROR) {
                            Close();
                            freeaddrinfo(result);
                            lLog(lError) << "ConnectTimeout error select: " << printWindowsErrorMessage(WSAGetLastError());
                        }
                        else if (FD_ISSET(socket_, &myset)) {
                            lon = sizeof(int);
                            if (getsockopt(socket_, SOL_SOCKET, SO_ERROR, (char*)(&valopt), &lon) == SOCKET_ERROR) {
                                Close();
                                freeaddrinfo(result);

                                lLog(lError) << "ConnectTimeout error getsockopt failed: " <<
                                    printWindowsErrorMessage(WSAGetLastError());
                            }
                            // check the value returned
                            if (valopt) {
                                Close();
                                freeaddrinfo(result);
                                lLog(lError) << "ConnectTimeout error in delay connect() -" << valopt <<
                                    printWindowsErrorMessage(WSAGetLastError());
                            }
                            // Set to blocking mode again...
                            mode = 0;
                            if ((arg = ioctlsocket(socket_, FIONBIO, &mode)) != 0) {
                                Close();
                                freeaddrinfo(result);
                                lLog(lError) << "ConnectTimeout error ioctlsocket(..., FIONBIO,):"<<
                                    printWindowsErrorMessage(WSAGetLastError());
                            }

                            freeaddrinfo(result);
                            return;
                        }
                        else {
                            freeaddrinfo(result);
                            lLog(lError) << "ConnectTimeout error timeout in select";
                            throw SocketException{ "Connect time out.", -1 };
                        }
                    } while (1);
                }
                else {
                    lLog(lError) << "ConnectTimeout error connecting failed: " <<
                        printWindowsErrorMessage(WSAGetLastError());
                    throw SocketException{ "Connect time out.", -1 };
                }
            }
        }
        Close();
        freeaddrinfo(result);           /* No longer needed */
	}

	void WindowsSocketTcp::Close() {
        if (closesocket(socket_) == SOCKET_ERROR) {
            lLog(lError) << "Close error:" << printWindowsErrorMessage(WSAGetLastError());
            throw SocketException{ "Close error.", (int16_t)WSAGetLastError() };
        }
	}

    Result<int32_t> WindowsSocketTcp::Send(const std::string& data) const {
        //std::vector<char> vec{ data.begin(), data.end() };
        return Send(std::vector<char>{ data.begin(), data.end() });
    }

    Result<int32_t> WindowsSocketTcp::Send(const std::vector<unsigned char>& data) const {
        int32_t sent = 0;

        if ((sent = send(socket_, data.data(), data.size(), 0)) == SOCKET_ERROR) {
            lLog(lError) << "Send error:" << printWindowsErrorMessage(WSAGetLastError());
            return Result<int32_t>{webserver::util::convertToResult(WSAGetLastError()), 0};
        }
        return Result<int32_t>{SocketResult::OK, sent};
	}

    Result<std::vector<char>> WindowsSocketTcp::Receive() const {
        const uint16_t receive_size = 0x8000;
        int16_t received = 0;
        std::vector<char> vec;
        vec.resize(receive_size);

        received = recv(socket_, vec.data(), receive_size, 0);
        if (received == SOCKET_ERROR) {
            lLog(lError) << "Receive error: " << printWindowsErrorMessage(WSAGetLastError());
            return Result<std::vector<char>>{webserver::util::convertToResult(WSAGetLastError()), std::vector<char>{}};
        }
        vec.resize(received);
        return Result<std::vector<char>>{SocketResult::OK, vec};
	}

	void WindowsSocketTcp::Bind(uint32_t port) {
        struct sockaddr_in server;
        int ret;

        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(port);

        if ((ret = bind(socket_, (SOCKADDR*)&server, sizeof(server))) == SOCKET_ERROR) {
            lLog(lError) << "Bind error: " << printWindowsErrorMessage(WSAGetLastError());
            Close();
            throw SocketException{ "Bind error.", (int16_t)webserver::util::convertToResult(WSAGetLastError())};
        }
        port_ = port;
	}

	void WindowsSocketTcp::Listen() {
        int ret;
        if ((ret = listen(socket_, 5)) == SOCKET_ERROR) {
            lLog(lError) << "Listen error: " << printWindowsErrorMessage(WSAGetLastError());
            throw SocketException{ "Listen error.", (int16_t)webserver::util::convertToResult(WSAGetLastError()) };
        }
        lLog(lDebug) << "Listening " << host_ << " on port " << port_;
	}

	std::unique_ptr<ISocketTcp> WindowsSocketTcp::Accept() {
        socket_t sock2;
        struct sockaddr_in client;
        int len;
        char buffer[64] = { 0x00 };

        len = sizeof(client);
        sock2 = accept(socket_, (struct sockaddr*)&client, &len);
        if (sock2 == SOCKET_ERROR) {
            lLog(lError) << "Accept error: " << printWindowsErrorMessage(WSAGetLastError());
            throw SocketException{ "Accept error.", (int16_t)webserver::util::convertToResult(WSAGetLastError()) };
        }
        
        if (inet_ntop(AF_INET, &client.sin_addr, buffer, 64) == NULL) {
            lLog(lWarn) << "Accept error: could not determine client address" << printWindowsErrorMessage(WSAGetLastError());
        }
        lLog(lInfo) << "Connected to client with address: " << buffer;

		return std::make_unique<WindowsSocketTcp>(sock2, "", 0);
	}

    int16_t WindowsSocketTcp::GetFd() const {
        return static_cast<int16_t>(socket_);
    }

    void WindowsSocketTcp::SetFd(int16_t fd) {
        socket_ = fd;
    }

    std::string WindowsSocketTcp::GetPeerAddress() const {
        struct sockaddr_in addr = { 0 };
        socklen_t addr_len = sizeof(addr);
        const size_t address_size = 128;
        char address[address_size] = { 0x00 };
        int rc = -1;

        if (rc = getpeername(socket_, (struct sockaddr*)&addr, &addr_len) == ERROR_SUCCESS) {
            char _addr[256] = { 0 };
            snprintf(address, address_size, "%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        }
        else {
            lLog(lWarn) << "Could not get peername: " << rc;
            return std::string{};
        }
        return std::string{ address };

    }

	// SocketFactory
	ISocketPtr SocketFactory::Create() const {
		return std::make_unique<WindowsSocketTcp>();
	}

	ISocketPtr SocketFactory::Create(const std::string& host, uint32_t port) const {
		return std::make_unique<WindowsSocketTcp>(host, port);
	}

    ISocketPtr SocketFactory::Create(int16_t fd) const {
        return std::make_unique<WindowsSocketTcp>(fd, "", 0);
    }

}