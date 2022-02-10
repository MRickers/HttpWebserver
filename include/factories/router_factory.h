#pragma once
#include <server/router.h>

namespace webserver {
    class RouterFactory {
    public:
        RouterPtr Create() const;
    };
}