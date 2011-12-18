// v8sharp is a community project available under the Microsoft Public License (Ms-PL)
// Code is provided as is and with no warrenty – Use at your own risk
// View the project and the latest code at http://v8sharp.codeplex.com/

#include "v8function.h"
#include "v8external.h"
#include "v8value.h"

v8::Handle<v8::Value> V8FunctionInvoker(const v8::Arguments& info);

namespace v8sharp 
{
    ref class V8FunctionWrapper 
    {
    public:
        V8FunctionWrapper(v8::Function* function) 
        {
            this->function = function;
        }

        Object^ Invoke(cli::array<Object^>^ args) 
        {
            v8::HandleScope handleScope;

            v8::Handle<v8::Function> functionHandle(this->function);

            int len = 0;
            if (args != nullptr) 
            {
                len = args->Length;
            }

            v8::Handle<v8::Value>* v8args = new v8::Handle<v8::Value>[len];
            for (int i = 0; i < len; i++) 
            {
                v8args[i] = V8Interop::ToV8(args[i]);
            }

            try 
            {
                v8::Local<v8::Value> result = functionHandle->Call(functionHandle, len, v8args);
                return v8sharp::V8Interop::FromV8(result);
            } 
            finally 
            {
                delete v8args;
            }
        }

    private:
        v8::Function* function;
    };

    Delegate^ V8FunctionHandler::CreateDelegate(v8::Function* function) 
    {
        return CreateDelegate(function, V8Function::typeid);
    }

    Delegate^ V8FunctionHandler::CreateDelegate(v8::Function* function, Type^ delegateType) 
    {
        V8FunctionWrapper^ functionWrapper = gcnew V8FunctionWrapper(function);
        return gcnew V8Function(functionWrapper, &V8FunctionWrapper::Invoke);
    }

    v8::Handle<v8::Function> V8FunctionHandler::CreateFunction(Delegate^ function) 
    {
        v8::HandleScope handleScope;

        V8ExternalWrapper* functionExternal = V8ExternalWrapper::Create(function);

        v8::Handle<v8::FunctionTemplate> method = v8::FunctionTemplate::New(V8FunctionInvoker, v8::External::New((void*)functionExternal));
        return handleScope.Close(method->GetFunction());
    }
}

v8::Handle<v8::Value> V8FunctionInvoker(const v8::Arguments& info) 
{
    v8::HandleScope handleScope;
    
    V8ExternalWrapper* external = (V8ExternalWrapper*)v8::Handle<v8::External>::Cast(info.Data())->Value();
    Object^ obj = external->Object;

    int len = info.Length();
    cli::array<Object^>^ args = gcnew cli::array<Object^>(len);
    cli::array<Type^>^ argTypes = gcnew cli::array<Type^>(len);
    for (int i = 0; i < len; i++) 
    {
        Object^ arg = v8sharp::V8Interop::FromV8(info[i]);
        args[i] = arg;
    }

    Object^ value = static_cast<Delegate^>(obj)->DynamicInvoke(args);
    return handleScope.Close(v8sharp::V8Interop::ToV8(value));
}