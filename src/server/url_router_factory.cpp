#include <factories/router_factory.h>
#include <server/url_router.h>

namespace webserver {
    RouterPtr RouterFactory::Create() const {
        return std::make_unique<UrlRouter>();
    }
}