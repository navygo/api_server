#include "api_server.h"
#include <iostream>
#include <sstream>
#include <algorithm>

ApiServer::ApiServer(int port) : port_(port), running_(false) {
    initialize_socket();
}

ApiServer::~ApiServer() {
    stop();
    cleanup_socket();
}

void ApiServer::initialize_socket() {
#ifdef _WIN32
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data_) != 0) {
        throw std::runtime_error("WSAStartup failed");
    }
#endif

    server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket_ < 0) {
        throw std::runtime_error("Socket creation failed");
    }

    int opt = 1;
#ifdef _WIN32
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#else
    setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
#endif

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_);

    if (bind(server_socket_, (sockaddr*)&address, sizeof(address)) < 0) {
        throw std::runtime_error("Bind failed");
    }

    if (listen(server_socket_, 10) < 0) {
        throw std::runtime_error("Listen failed");
    }
}

void ApiServer::cleanup_socket() {
#ifdef _WIN32
    closesocket(server_socket_);
    WSACleanup();
#else
    close(server_socket_);
#endif
}

void ApiServer::start() {
    running_ = true;
    server_thread_ = std::make_unique<std::thread>([this]() {
        std::cout << "API Server started on port " << port_ << std::endl;
        
        while (running_) {
            sockaddr_in client_addr{};
            socklen_t client_len = sizeof(client_addr);
            
#ifdef _WIN32
            SOCKET client_socket = accept(server_socket_, (sockaddr*)&client_addr, &client_len);
            if (client_socket == INVALID_SOCKET) continue;
#else
            int client_socket = accept(server_socket_, (sockaddr*)&client_addr, &client_len);
            if (client_socket < 0) continue;
#endif

            handle_client(client_socket);
            
#ifdef _WIN32
            closesocket(client_socket);
#else
            close(client_socket);
#endif
        }
    });
}

void ApiServer::stop() {
    running_ = false;
    if (server_thread_ && server_thread_->joinable()) {
        server_thread_->join();
    }
}

void ApiServer::handle_client(int client_socket) {
    char buffer[4096] = {0};
    
#ifdef _WIN32
    int bytes_read = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
#else
    int bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
#endif

    if (bytes_read > 0) {
        std::string request_str(buffer, bytes_read);
        HttpRequest request = parse_request(request_str);
        
        auto handler = router_.find_handler(request.method, request.path);
        HttpResponse response;
        
        if (handler) {
            response = handler->handle(request);
        } else {
            response = HttpResponse::ErrorResponse("Route not found", 404);
        }
        
        send_response(client_socket, response);
    }
}

HttpRequest ApiServer::parse_request(const std::string& raw_request) {
    HttpRequest request;
    std::istringstream stream(raw_request);
    std::string line;
    
    // 解析请求行
    if (std::getline(stream, line)) {
        std::istringstream line_stream(line);
        line_stream >> request.method >> request.path;
    }
    
    // 解析headers
    while (std::getline(stream, line) && line != "\r") {
        size_t colon_pos = line.find(':');
        if (colon_pos != std::string::npos) {
            std::string key = line.substr(0, colon_pos);
            std::string value = line.substr(colon_pos + 1);
            // 去除首尾空白字符
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            request.headers[key] = value;
        }
    }
    
    // 解析body
    std::string body;
    while (std::getline(stream, line)) {
        body += line + "\n";
    }
    if (!body.empty()) {
        request.body = body.substr(0, body.length() - 1); // 移除最后一个换行
    }
    
    return request;
}

void ApiServer::send_response(int client_socket, const HttpResponse& response) {
    std::string response_str = "HTTP/1.1 " + std::to_string(response.status_code) + " OK\r\n";
    
    for (const auto& header : response.headers) {
        response_str += header.first + ": " + header.second + "\r\n";
    }
    
    response_str += "Content-Length: " + std::to_string(response.body.length()) + "\r\n";
    response_str += "\r\n";
    response_str += response.body;
    
#ifdef _WIN32
    send(client_socket, response_str.c_str(), response_str.length(), 0);
#else
    write(client_socket, response_str.c_str(), response_str.length());
#endif
}

void ApiServer::get(const std::string& pattern, HandlerFunc handler) {
    router_.get(pattern, std::move(handler));
}

void ApiServer::post(const std::string& pattern, HandlerFunc handler) {
    router_.post(pattern, std::move(handler));
}

void ApiServer::put(const std::string& pattern, HandlerFunc handler) {
    router_.put(pattern, std::move(handler));
}

void ApiServer::delete_(const std::string& pattern, HandlerFunc handler) {
    router_.delete_(pattern, std::move(handler));
}
