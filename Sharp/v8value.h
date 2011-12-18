// v8sharp is a community project available under the Microsoft Public License (Ms-PL)
// Code is provided as is and with no warrenty – Use at your own risk
// View the project and the latest code at http://v8sharp.codeplex.com/

#pragma once

#include <v8.h>

using namespace System;

namespace v8sharp 
{
    ref class V8Interop sealed 
    {
    public:
        static System::Object^ FromV8(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8(Object^ value);

    private:
        static System::String^ FromV8String(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8String(System::String^ value);

        static bool FromV8Boolean(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Boolean(bool value);

        static int FromV8Int32(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Int32(int value);

        static double FromV8Double(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Double(double value);

        static cli::array<Object^>^ FromV8Array(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Array(System::Array^ value);

        static System::DateTime FromV8Date(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Date(DateTime value);

        static System::Delegate^ FromV8Function(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Function(Delegate^ value);

        static System::Object^ FromV8Object(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8Object(Object^ value);

        static System::Object^ FromV8External(v8::Handle<v8::Value> value);

        static v8::Handle<v8::Value> ToV8External(Object^ value);
    };
}