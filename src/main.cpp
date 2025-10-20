#include "api_server.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    try {
        ApiServer server(8080);
        
        // 添加路由示例
        server.get("/", []([[maybe_unused]] const HttpRequest& req) {
            return HttpResponse::JsonResponse("{\"message\": \"Welcome to API Server\", \"status\": \"running\"}");
        });
        
        server.get("/api/health", []([[maybe_unused]] const HttpRequest& req) {
            return HttpResponse::JsonResponse("{\"status\": \"healthy\", \"timestamp\": \"" + 
                std::to_string(std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count()) + "\"}");
        });
        
        server.get("/api/users/[0-9]+", [](const HttpRequest& req) {
            // 模拟获取用户信息
            std::string user_id = req.path.substr(req.path.find_last_of('/') + 1);
            return HttpResponse::JsonResponse("{\"user_id\": " + user_id + ", \"name\": \"John Doe\", \"email\": \"john@example.com\"}");
        });
        
        server.post("/api/users", [](const HttpRequest& req) {
            // 模拟创建用户
            return HttpResponse::JsonResponse("{\"message\": \"User created\", \"data\": " + req.body + "}", 201);
        });
        
        server.put("/api/users/[0-9]+", [](const HttpRequest& req) {
            // 模拟更新用户
            return HttpResponse::JsonResponse("{\"message\": \"User updated\", \"data\": " + req.body + "}");
        });
        
        server.delete_("/api/users/[0-9]+", []([[maybe_unused]] const HttpRequest& req) {
            // 模拟删除用户
            return HttpResponse::JsonResponse("{\"message\": \"User deleted\"}");
        });
        
        std::cout << "Starting API Server..." << std::endl;
        server.start();
        
        // 保持服务器运行
        std::cout << "Press Enter to stop the server..." << std::endl;
        std::cin.get();
        
        server.stop();
        std::cout << "Server stopped." << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
