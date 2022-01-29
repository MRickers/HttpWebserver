#pragma once
#include <memory>
#include <multiplexer/multiplexer.h>

namespace webserver::multiplexer {
	using MultiplexerPtr = std::unique_ptr<IMultiplexer>;

	class MultiplexerFactory {
	public:
		MultiplexerPtr Create() const;
	};
}