#include <multiplexer/multiplexer.h>

namespace webserver::multiplexer {

	class SelectMultiplexer : public IMultiplexer {
	public:
		SelectMultiplexer();

		virtual webserver::sock::Result<std::vector<int16_t>> Select(
			const std::vector<int16_t>& read_fds,
			const std::vector<int16_t>& write_fds,
			const std::vector<int16_t>& except_fds) const override;
	};
}