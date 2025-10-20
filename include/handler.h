#ifndef HANDLER_H
#define HANDLER_H

#include "request.h"
#include <functional>
#include <memory>

class RequestHandler {
public:
    virtual ~RequestHandler() = default;
    virtual HttpResponse handle(const HttpRequest& request) = 0;
};

using HandlerFunc = std::function<HttpResponse(const HttpRequest&)>;

class FunctionHandler : public RequestHandler {
private:
    HandlerFunc handler_func_;
    
public:
    FunctionHandler(HandlerFunc func) : handler_func_(std::move(func)) {}
    
    HttpResponse handle(const HttpRequest& request) override {
        return handler_func_(request);
    }
};

#endif
