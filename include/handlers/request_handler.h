#pragma once

namespace webserver::multiplexer {
	class IRequestHandler {
	public:
		virtual ~IRequestHandler() {}
		virtual void HandleRequest(int fd) = 0;
	};
}