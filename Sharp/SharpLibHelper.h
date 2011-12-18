#include <node.h>
#include <v8.h>
#include <string>

using namespace v8;

void LoadAssembly();

class SharpLibHelper;

struct Baton {
    uv_work_t request;
    SharpLibHelper* sharpLibHelper;
    Persistent<Function> callback;
    // A parameter that will be passed to the .Net library
    std::string url;
    // Tracking errors that happened in the worker function. You can use any
    // variables you want. E.g. in some cases, it might be useful to report
    // an error number.
    bool error;
    std::string error_message;

    // Custom data
    std::string result;
};

struct Baton2 {
    uv_async_t  asyncHandle;
    SharpLibHelper* sharpLibHelper;
    Persistent<Function> callback;
    // A parameter that will be passed to the .Net library
    std::string url;
    // Tracking errors that happened in the worker function. You can use any
    // variables you want. E.g. in some cases, it might be useful to report
    // an error number.
    bool error;
    std::string error_message;

    // Custom data
    std::string result;
};


// this is a shim class which mediates between managed and unmanaged code
class SharpLibHelper
{
protected:
    SharpLibHelper() {};
public:
    virtual bool DownloadUrl(std::string& url, std::string& err, std::string& result) = 0;
    virtual int GetNumber() = 0;
    virtual void DownloadUrlAsync(std::string& url, Baton2* baton) = 0;

    static SharpLibHelper* New();
};
