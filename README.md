# API Server

## 项目概述
该项目是一个小型的单进程 HTTP API 服务器，旨在提供简单的请求/响应语义。服务器接受原始 TCP 连接，解析简单的 HTTP 请求，匹配处理程序并返回响应。

## 文件结构
- `src/main.cpp`：应用程序的入口点，构建 `ApiServer` 实例并注册路由。
- `src/api_server.cpp`：实现 `ApiServer` 类，处理服务器的生命周期、套接字管理和请求解析。
- `src/router.cpp`：实现路由逻辑，存储路由并使用正则表达式进行匹配。
- `src/handler.cpp`：用于实现请求处理程序，目前为空，未来可以添加 `RequestHandler` 的实现。
- `include/api_server.h`：声明 `ApiServer` 类及其公共方法和属性。
- `include/router.h`：声明路由相关的类和方法，用于添加和匹配路由。
- `include/handler.h`：声明处理程序相关的类和方法。
- `include/request.h`：声明 `HttpRequest` 和 `HttpResponse` 数据模型，定义请求和响应的格式。
- `CMakeLists.txt`：CMake 的配置文件，定义了构建项目所需的设置和依赖。
- `.github/copilot-instructions.md`：针对 AI 编码代理的指导说明，提供项目的结构和重要代码模式。

## 部署方式
1. **构建项目**
   - 在项目根目录下创建并进入 `build` 目录：
     ```
     mkdir -p build && cd build
     ```
   - 运行 CMake 配置和构建：
     ```
     cmake ..
     cmake --build .
     ```

2. **运行服务器**
   - 在 `build` 目录下运行生成的二进制文件：
     ```
     ./api_server
     ```
   - 服务器默认监听端口 8080。

3. **访问 API**
   - 使用浏览器或 API 客户端访问以下示例路由：
     - 健康检查：`http://localhost:8080/api/health`
     - 用户信息：`http://localhost:8080/api/users/{id}`（替换 `{id}` 为实际用户 ID）

## 贡献
欢迎对项目进行贡献！请遵循代码风格并确保在提交之前进行本地构建和测试。