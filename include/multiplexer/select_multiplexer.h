#include <multiplexer/multiplexer.h>

namespace webserver::multiplexer {

	class SelectMultiplexer : public IMultiplexer {
	public:
		SelectMultiplexer();

		virtual webserver::sock::Result<std::vector<int16_t>> Select(
			std::vector<int16_t>& read_fds,
			std::vector<int16_t>& write_fds,
			std::vector<int16_t>& except_fds) const override;
	};
}