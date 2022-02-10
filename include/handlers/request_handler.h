#pragma once
#include <memory>

namespace webserver::multiplexer {
	class IRequestHandler {
	public:
		virtual ~IRequestHandler() {}
		virtual void HandleRequest(int fd) = 0;
	};

	using RequestHandlerPtr = std::unique_ptr<IRequestHandler>;
}