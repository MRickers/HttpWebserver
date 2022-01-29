#pragma once
#include <vector>
namespace webserver::multiplexer {
	class IRequestDispatcher {
	public:
		virtual ~IRequestDispatcher() {}

		IRequestDispatcher(){}
		virtual void AddClient(int16_t fd) = 0;
		virtual void RemoveClient(int16_t fd) = 0;

		virtual void HandleEvents(uint16_t port) = 0;

	};
}