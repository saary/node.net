#pragma comment(lib, "node")

#include <node.h>
#include <v8.h>
#include <string>

#include "SharpLibHelper.h"

using namespace node;
using namespace v8;


class SharpObject: ObjectWrap
{
private:
    SharpLibHelper* _sharpLibHelper;
public:

    static Persistent<FunctionTemplate> s_ct;
    static void NODE_EXTERN Init(Handle<Object> target)
    {
        HandleScope scope;

        // set the constructor function
        Local<FunctionTemplate> t = FunctionTemplate::New(New);

        // set the node.js/v8 class name
        s_ct = Persistent<FunctionTemplate>::New(t);
        s_ct->InstanceTemplate()->SetInternalFieldCount(1);
        s_ct->SetClassName(String::NewSymbol("SharpObject"));

        // registers a class member functions 
        NODE_SET_PROTOTYPE_METHOD(s_ct, "async", Async);
        NODE_SET_PROTOTYPE_METHOD(s_ct, "async2", Async2);
        NODE_SET_PROTOTYPE_METHOD(s_ct, "getSharpValue", GetSharpValue);

        target->Set(String::NewSymbol("SharpObject"),
            s_ct->GetFunction());
    }

    SharpObject() 
    {
        _sharpLibHelper = SharpLibHelper::New();
    }

    ~SharpObject()
    {
        delete _sharpLibHelper;
    }

    static Handle<Value> New(const Arguments& args)
    {
        HandleScope scope;
        SharpObject* pm = new SharpObject();
        pm->Wrap(args.This());
        return args.This();
    }

    static Handle<Value> GetSharpValue(const Arguments& args)
    {
        HandleScope scope;
        SharpObject* so = ObjectWrap::Unwrap<SharpObject>(args.This());
        Local<Integer> result = Integer::New(so->_sharpLibHelper->GetNumber());
        return scope.Close(result);
    }

    static Handle<Value> Async(const Arguments& args)
    {
        HandleScope scope;

        if (!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
                String::New("First argument must be a string")));
        }

        if (!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
                String::New("Second argument must be a callback function")));
        }

        Local<String> url = Local<String>::Cast(args[0]);
        // There's no ToFunction(), use a Cast instead.
        Local<Function> callback = Local<Function>::Cast(args[1]);

        SharpObject* so = ObjectWrap::Unwrap<SharpObject>(args.This());

        // create a state object
        Baton* baton = new Baton();
        baton->request.data = baton;
        baton->sharpLibHelper = so->_sharpLibHelper;
        baton->callback = Persistent<Function>::New(callback);
        baton->url = *v8::String::AsciiValue(url);

        // register a worker thread request
        uv_queue_work(uv_default_loop(), &baton->request,
            StartAsync, AfterAsync);

        return Undefined();
    }


    static Handle<Value> Async2(const Arguments& args)
    {
        HandleScope scope;

        if (!args[0]->IsString()) {
            return ThrowException(Exception::TypeError(
                String::New("First argument must be a string")));
        }

        if (!args[1]->IsFunction()) {
            return ThrowException(Exception::TypeError(
                String::New("Second argument must be a callback function")));
        }

        Local<String> url = Local<String>::Cast(args[0]);
        // There's no ToFunction(), use a Cast instead.
        Local<Function> callback = Local<Function>::Cast(args[1]);

        SharpObject* so = ObjectWrap::Unwrap<SharpObject>(args.This());

        // create a state object
        Baton2* baton = new Baton2();
        baton->asyncHandle.data = baton;
        baton->sharpLibHelper = so->_sharpLibHelper;
        baton->callback = Persistent<Function>::New(callback);
        baton->url = *v8::String::AsciiValue(url);
        baton->error = false;

        uv_async_init(uv_default_loop(), &baton->asyncHandle, AfterAsync2);
        so->_sharpLibHelper->DownloadUrlAsync(baton->url, baton);
        
        return Undefined();
    }

    // this runs on the worker thread and should not callback or interact with node/v8 in any way
    static void StartAsync(uv_work_t* req)
    {
        Baton *baton = static_cast<Baton*>(req->data);
        baton->error = baton->sharpLibHelper->DownloadUrl(baton->url, baton->error_message, baton->result);
    }

    // this runs on the main thread and can call back into the JavaScript
    static void AfterAsync(uv_work_t *req)
    {
        HandleScope scope;
        Baton* baton = static_cast<Baton*>(req->data);

        if (baton->error) 
        {
            Local<Value> err = Exception::Error(
                String::New(baton->error_message.c_str()));
            Local<Value> argv[] = { err };

            TryCatch try_catch;
            baton->callback->Call(
                Context::GetCurrent()->Global(), 1, argv);

            if (try_catch.HasCaught()) {
                node::FatalException(try_catch);
            }        
        } 
        else 
        {
            const unsigned argc = 2;
            Local<Value> argv[argc] = {
                Local<Value>::New(Null()),
                Local<Value>::New(String::New(baton->result.c_str()))
            };

            TryCatch try_catch;
            baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);

            if (try_catch.HasCaught()) {
                FatalException(try_catch);
            }
        }

        baton->callback.Dispose();
        delete baton;
    }

    static void AfterAsync2(uv_async_t *handle, int status)
    {
        HandleScope scope;
        Baton2* baton = static_cast<Baton2*>(handle->data);

        if (baton->error) 
        {
            Local<Value> err = Exception::Error(
                String::New(baton->error_message.c_str()));
            Local<Value> argv[] = { err };

            TryCatch try_catch;
            baton->callback->Call(
                Context::GetCurrent()->Global(), 1, argv);

            if (try_catch.HasCaught()) {
                node::FatalException(try_catch);
            }        
        } 
        else 
        {
            const unsigned argc = 2;
            Local<Value> argv[argc] = {
                Local<Value>::New(Null()),
                Local<Value>::New(String::New(baton->result.c_str()))
            };

            TryCatch try_catch;
            baton->callback->Call(Context::GetCurrent()->Global(), argc, argv);

            if (try_catch.HasCaught()) {
                FatalException(try_catch);
            }
        }

        baton->callback.Dispose();
        delete baton;
        uv_unref(uv_default_loop());
    }
};

Persistent<FunctionTemplate> SharpObject::s_ct;

extern "C" {
    void NODE_EXTERN init (Handle<Object> target)
    {
        SharpObject::Init(target);
        LoadAssembly();
    }
    NODE_MODULE(sharp, init);
}