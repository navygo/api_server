#include "router.h"
#include <iostream>

void Router::add_route(const std::string& method, const std::string& pattern, 
                      std::shared_ptr<RequestHandler> handler) {
    Route route;
    route.method = method;
    route.pattern = std::regex(pattern);
    route.handler = handler;
    routes_.push_back(route);
}

void Router::add_route(const std::string& method, const std::string& pattern, 
                      HandlerFunc handler_func) {
    add_route(method, pattern, 
              std::make_shared<FunctionHandler>(std::move(handler_func)));
}

std::shared_ptr<RequestHandler> Router::find_handler(const std::string& method, 
                                                    const std::string& path) const {
    for (const auto& route : routes_) {
        if (route.method == method && std::regex_match(path, route.pattern)) {
            return route.handler;
        }
    }
    return nullptr;
}

void Router::get(const std::string& pattern, HandlerFunc handler) {
    add_route("GET", pattern, std::move(handler));
}

void Router::post(const std::string& pattern, HandlerFunc handler) {
    add_route("POST", pattern, std::move(handler));
}

void Router::put(const std::string& pattern, HandlerFunc handler) {
    add_route("PUT", pattern, std::move(handler));
}

void Router::delete_(const std::string& pattern, HandlerFunc handler) {
    add_route("DELETE", pattern, std::move(handler));
}
