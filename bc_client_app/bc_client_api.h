#ifndef BC_CLIENT_API_H
#define BC_CLIENT_API_H

#include <functional>
#include <string>

#ifdef _WIN32
#ifdef BC_CLIENT_LIB_LIBRARY
#define BC_CLIENT_API_EXPORT __declspec(dllexport)
#else
#define BC_CLIENT_API_EXPORT __declspec(dllimport)
#endif
#else
#define BC_CLIENT_API_EXPORT __attribute__((visibility("default")))
#endif

class BC_CLIENT_API_EXPORT bc_client_api
{
public:
    bc_client_api();
    ~bc_client_api();

    // Connection
    bool connect(const std::string& host, int port);
    void disconnect();

    // Send
    bool send(const std::string& data);

    // Callback types
    using CallbackTypeA = std::function<void(const std::string&)>;
    using CallbackTypeB = std::function<void(const std::string&)>;

    void setCallbackTypeA(CallbackTypeA cb);
    void setCallbackTypeB(CallbackTypeB cb);

    // internal pointer to implementation
    struct Impl;
    Impl* impl;
private:

    CallbackTypeA callbackA;
    CallbackTypeB callbackB;

    // static bridge functions (C → C++)
    static void callbackA_bridge(const char* data, void* context);
};

#endif
