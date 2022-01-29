#pragma once
#include <memory>
#include <multiplexer/request_dispatcher.h>

namespace webserver::multiplexer {
	using DispatcherPtr = std::unique_ptr<IRequestDispatcher>;

	class DispatcherFactory {
	public:
		DispatcherPtr Create() const;
	};
}