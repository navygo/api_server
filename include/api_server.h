#ifndef API_SERVER_H
#define API_SERVER_H

#include "router.h"
#include <string>
#include <thread>
#include <atomic>
#include <memory>

#ifdef _WIN32
    #include <winsock2.h>
    #pragma comment(lib, "ws2_32.lib")
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
#endif

class ApiServer {
private:
    int port_;
    std::atomic<bool> running_;
    std::unique_ptr<std::thread> server_thread_;
    Router router_;
    
#ifdef _WIN32
    SOCKET server_socket_;
    WSADATA wsa_data_;
#else
    int server_socket_;
#endif

    void initialize_socket();
    void cleanup_socket();
    void handle_client(int client_socket);
    HttpRequest parse_request(const std::string& raw_request);
    void send_response(int client_socket, const HttpResponse& response);
    
public:
    ApiServer(int port = 8080);
    ~ApiServer();
    
    void start();
    void stop();
    Router& get_router() { return router_; }
    
    // 便捷路由方法
    void get(const std::string& pattern, HandlerFunc handler);
    void post(const std::string& pattern, HandlerFunc handler);
    void put(const std::string& pattern, HandlerFunc handler);
    void delete_(const std::string& pattern, HandlerFunc handler);
};

#endif
