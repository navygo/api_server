#ifndef ROUTER_H
#define ROUTER_H

#include "handler.h"
#include <unordered_map>
#include <memory>
#include <regex>

class Router {
private:
    struct Route {
        std::string method;
        std::regex pattern;
        std::shared_ptr<RequestHandler> handler;
    };
    
    std::vector<Route> routes_;
    
public:
    void add_route(const std::string& method, const std::string& pattern, 
                   std::shared_ptr<RequestHandler> handler);
    
    void add_route(const std::string& method, const std::string& pattern, 
                   HandlerFunc handler_func);
    
    std::shared_ptr<RequestHandler> find_handler(const std::string& method, 
                                                const std::string& path) const;
    
    // 便捷方法
    void get(const std::string& pattern, HandlerFunc handler);
    void post(const std::string& pattern, HandlerFunc handler);
    void put(const std::string& pattern, HandlerFunc handler);
    void delete_(const std::string& pattern, HandlerFunc handler);
};

#endif
