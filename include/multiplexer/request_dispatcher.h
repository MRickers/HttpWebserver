#pragma once
#include <vector>
#include <memory>

namespace webserver::multiplexer {
	class IRequestDispatcher {
	public:
		virtual ~IRequestDispatcher() {}

		IRequestDispatcher(){}
		virtual void AddClient(int16_t fd) = 0;
		virtual void RemoveClient(int16_t fd) = 0;

		virtual void Init(uint16_t port) = 0;
		virtual void  WaitForEvents() = 0;

	};

	using DispatcherPtr = std::unique_ptr<IRequestDispatcher>;
}