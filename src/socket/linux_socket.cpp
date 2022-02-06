#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <ifaddrs.h>

#include <socket/linux_socket.h>
#include <logging/logging.h>
#include <util/util.h>

[[maybe_unused]]static const char* localhost = "127.0.0.1";
static const int default_timeout = 15000;

namespace webserver::sock {
	void ISocketTcp::Init() {
		
	}

	void ISocketTcp::CleanUp() {
		
	}

	LinuxSocketTcp::LinuxSocketTcp() : LinuxSocketTcp("", 0) {
		
	}

    LinuxSocketTcp::LinuxSocketTcp(socket_t sock, const std::string& host, uint32_t port) : 
        socket_(sock),
        port_(port),
        host_(host)    {
        
    }

	LinuxSocketTcp::LinuxSocketTcp(const std::string& host, uint32_t port) : 
        socket_(),
        port_(port),
        host_(host)
    {
        socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socket_ < 0) {
            lLog(lError) << "Create socket failed " << socket_;
            throw SocketException{ "Create socket failed.", (int16_t)socket_ };
        }
	}

    

	void LinuxSocketTcp::Connect(const std::string& host, uint32_t port) {
        ConnectTimeout(host, port, 1000);
	}

	void LinuxSocketTcp::ConnectTimeout(const std::string& host, uint32_t port, uint32_t timeout_ms) {
        struct addrinfo hints;
        struct addrinfo* result = NULL;
        struct addrinfo* rp = NULL;
        char portnumber_string[33];
        int ret, valopt = -1;
        struct timeval tv = {0x00, 0x00};
        long arg;
        fd_set myset;   // menge an file deskriptoren
        socklen_t lon;

        if(timeout_ms > 0) {
            tv.tv_usec += timeout_ms * 1000;
            tv.tv_sec += tv.tv_usec / 1000000;
            tv.tv_usec %= 1000000;
        }

        snprintf(portnumber_string, 33, "%d", port);

        // Struct-Definition siehe https://www.man7.org/linux/man-pages/man3/getaddrinfo.3.html
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET; // Nur IPv4
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = 0;
        hints.ai_protocol = IPPROTO_TCP;

        // Set non-blocking 
        // Get fd status
        if( (arg = fcntl(socket_, F_GETFL , NULL)) < 0) { 
            lLog(lError) << "Error fcntl(..., F_GETFL) " << strerror(errno); 
            Close();
            throw SocketException{ "Error fcntl F_GETFL", (int16_t)errno };
        }
        arg |= O_NONBLOCK; 
        if( fcntl(socket_, F_SETFL, arg) < 0) { 
            lLog(lError) << "Error fcntl(..., F_SETFL) " <<  strerror(errno); 
            Close();
            throw SocketException{ "Error fcntl F_SETFL", (int16_t)errno };
        }

        ret = getaddrinfo(host.c_str(), portnumber_string, &hints, &result);
        if (ret != 0) {
            lLog(lError) << "ConnectTimeoutTcp error: DNS failure with " << host << " " << ret << " " << strerror(errno);
            freeaddrinfo(result);
            Close();
            throw SocketException{ "ConnectTimeoutTcp error: DNS failure with", (int16_t)errno };
        }

        for (rp = result; rp != NULL; rp = rp->ai_next) {
            if ((ret=connect(socket_, rp->ai_addr, rp->ai_addrlen)) != -1)
                break;                  /* Success */

            if(errno == EINPROGRESS) {
                do {
                    FD_ZERO(&myset);
                    FD_SET(socket_, &myset);   // socket zur Menge der beschreibbaren Deskriptoren hinzufügen
                    ret = select(socket_ + 1, NULL, &myset, NULL, (timeout_ms > 0 ? &tv : NULL));// select ist auf 1024 deskriptoren beschränkt, bei mehr = undefined behaviour
                    if(ret < 0 && errno != EINTR) {
                        lLog(lError) << "ConnectTimeoutTcp error: in select " << errno << " " << strerror(errno);
                        freeaddrinfo(result);
                        Close();
                        throw SocketException{ "ConnectTimeoutTcp error: in select", (int16_t)errno };
                    }else if(ret > 0) {
                        lon = sizeof(int);
                        // ist ein Fehler aufgetreten
                        if(getsockopt(socket_, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon) < 0) {    
                            lLog(lError) << "ConnectTimeoutTcp error: in getsockopt() " << errno << " " << strerror(errno);
                            freeaddrinfo(result);
                            Close();
                            throw SocketException{ "ConnectTimeoutTcp error: in getsockopt()", (int16_t)errno };
                        }
                        // check the value returned
                        if(valopt) {
                            lLog(lError) << "ConnectTimeoutTcp error: in delay connect()" <<  valopt << " " << strerror(valopt);
                            freeaddrinfo(result);
                            Close();
                            throw SocketException{ "ConnectTimeoutTcp error: in delay connect()", (int16_t)valopt};
                        }

                        // Set to blocking mode again... 
                        if( (arg = fcntl(socket_, F_GETFL, NULL)) < 0) { 
                            lLog(lError) << "ConnectTimeoutTcp error: fcntl(..., F_GETFL) " << errno << " " << strerror(errno); 
                            freeaddrinfo(result);
                            Close();
                            throw SocketException{ "ConnectTimeoutTcp error: fcntl(..., F_GETFL)", (int16_t)errno};
                        } 
                        arg &= (~O_NONBLOCK); 
                        if( fcntl(socket_, F_SETFL, arg) < 0) { 
                            lLog(lError) << "ConnectTimeoutTcp error: fcntl(..., F_SETFL)" << errno << " " << strerror(errno); 
                            freeaddrinfo(result);
                            Close();
                            throw SocketException{ "ConnectTimeoutTcp error: fcntl(..., F_SETFL)", (int16_t)errno};
                        } 

                        freeaddrinfo(result);
                    }else {
                        lLog(lError) << "ConnectTimeoutTcp error: timeout in select";
                        freeaddrinfo(result);
                        Close();
                        throw SocketException{ "ConnectTimeoutTcp error: timeout in select", 0};
                    }
                } while(1);
            }else {
                lLog(lError) << "ConnectTimeoutTcp error " << errno << " "<< strerror(errno);
            }
        }
	}

	void LinuxSocketTcp::Close() {
        if(close(socket_) == -1) {
            lLog(lError) << "Close error: " << errno << " " << strerror(errno);
            throw SocketException{ "Close error", 0};
        }
	}

    Result<int32_t> LinuxSocketTcp::Send(const std::string& data) const {
        return Send(std::vector<char>{ data.begin(), data.end() });
    }

    Result<int32_t> LinuxSocketTcp::Send(const std::vector<char>& data) const {
        int32_t sent=0;
        if((sent = send(socket_, data.data(), data.size(), 0)) < 0) {
            lLog(lError) << "SendTcp error: " << errno << " " << strerror(errno);
            return Result<int32_t>{SocketResult::DEFAULTERROR, (int16_t)errno};
        }
        return Result<int32_t>{SocketResult::OK, sent};
	}

    Result<std::vector<char>> LinuxSocketTcp::Receive() const {
        const uint16_t receive_size = 0x8000;
        int16_t received = 0;
        std::vector<char> vec;
        vec.resize(receive_size);

        if((received = recv(socket_, vec.data(), receive_size, 0)) <= 0) {
            lLog(lError) << "ReceiveTcp error: " << errno << " " << strerror(errno);
            return Result<std::vector<char>>{SocketResult::DEFAULTERROR, std::vector<char>{}};
        }
        vec.resize(received);
        return Result<std::vector<char>>{SocketResult::OK, vec};
	}

	void LinuxSocketTcp::Bind(uint32_t port) {
        struct sockaddr_in server;
        int ret;

        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = htonl(INADDR_ANY);
        server.sin_port = htons(port);

        if((ret=bind(socket_, (struct sockaddr*)&server, sizeof(server))) < 0) {
            lLog(lError) << "Bind error: Port " << port << " " << errno << " " << strerror(errno);
            Close();
            throw SocketException{ "Bind error.", errno};
        }
        port_ = port;
	}

	void LinuxSocketTcp::Listen() {
        if(listen(socket_, 5) == -1) {
            lLog(lError) << "Listen error:" << errno << " " << strerror(errno);
            throw SocketException{ "Listen error.", errno };
        }
        lLog(lDebug) << "Listening " << host_ << " on port " << port_;
	}

	std::unique_ptr<ISocketTcp> LinuxSocketTcp::Accept() {
        socket_t sock2;
        struct sockaddr_in client;
        socklen_t len;
        char buffer[64];

        len = sizeof(client);
        sock2 = accept(socket_, (struct sockaddr*)&client, &len);
        if(sock2 == -1) {
            lLog(lError) << "Accept error: " << errno << " " << strerror(errno);
            throw SocketException{ "Accept error.", errno };
        }
        if(inet_ntop(AF_INET, &client.sin_addr, buffer, 64) == NULL) {
            lLog(lWarn) << "Accept error: could not determine client address " << errno << " " << strerror(errno);
            throw SocketException{ "Accept error: could not determine client address", errno };
        }
        lLog(lInfo) << "Connected to client with address: " << buffer;
        return std::make_unique<LinuxSocketTcp>(sock2, "", 0);
	}

    int16_t LinuxSocketTcp::GetFd() const {
        return static_cast<int16_t>(socket_);
    }

    void LinuxSocketTcp::SetFd(int16_t fd) {
        socket_ = fd;
    }

    std::string LinuxSocketTcp::GetPeerAddress() const {
        struct sockaddr_in addr;
        socklen_t addr_len = sizeof(addr);
        const size_t address_size = 128;
        char address[address_size] = { 0x00 };

        if (const int rc = getpeername(socket_, (struct sockaddr*)&addr, &addr_len) == 0) {
            snprintf(address, address_size, "%s:%d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
        }
        else {
            lLog(lWarn) << "Could not get peername: " <<strerror(errno);
            return std::string{};
        }
        return std::string{ address };

    }

	// SocketFactory
	ISocketPtr SocketFactory::Create() const {
		return std::make_unique<LinuxSocketTcp>();
	}

	ISocketPtr SocketFactory::Create(const std::string& host, uint32_t port) const {
		return std::make_unique<LinuxSocketTcp>(host, port);
	}

    ISocketPtr SocketFactory::Create(int16_t fd) const {
        return std::make_unique<LinuxSocketTcp>(fd, "", 0);
    }

}