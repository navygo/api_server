#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <unordered_map>
#include <vector>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> query_params;
    
    HttpRequest() = default;
};

struct HttpResponse {
    int status_code;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
    
    HttpResponse(int code = 200, const std::string& content = "") 
        : status_code(code), body(content) {
        headers["Content-Type"] = "application/json";
    }
    
    static HttpResponse JsonResponse(const std::string& json, int code = 200) {
        HttpResponse response(code, json);
        response.headers["Content-Type"] = "application/json";
        return response;
    }
    
    static HttpResponse TextResponse(const std::string& text, int code = 200) {
        HttpResponse response(code, text);
        response.headers["Content-Type"] = "text/plain";
        return response;
    }
    
    static HttpResponse ErrorResponse(const std::string& message, int code = 400) {
        std::string error_json = "{\"error\": \"" + message + "\"}";
        return JsonResponse(error_json, code);
    }
};

#endif
