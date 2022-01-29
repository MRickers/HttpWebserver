#include <util/util.h>
#include <socket/windows_socket.h>

namespace webserver::util {
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

}