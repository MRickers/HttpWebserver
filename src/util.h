#pragma once
#include <socket/socket.h>

namespace webserver::util {
    /// <summary>
    /// Convert WSA Windows error code to SocketResult
    /// </summary>
    /// <param name="wsa_error"></param>
    /// <returns></returns>
    webserver::sock::SocketResult convertToResult(int wsa_error);
}