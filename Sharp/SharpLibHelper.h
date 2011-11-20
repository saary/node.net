#include <node.h>
#include <v8.h>
#include <string>

using namespace v8;

void LoadAssembly();

// this is a shim class which mediates between managed and unmanaged code
class SharpLibHelper
{
protected:
    SharpLibHelper() {};
public:
    virtual bool DownloadUrl(std::string& url, std::string& err, std::string& result) = 0;
    virtual int GetNumber() = 0;

    static SharpLibHelper* New();
};
