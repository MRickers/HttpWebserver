#include <factories/multiplexer_factory.h>
#include <multiplexer/select_multiplexer.h>

namespace webserver::multiplexer {
	MultiplexerPtr MultiplexerFactory::Create() const {
		return std::make_unique<SelectMultiplexer>();
	}
}