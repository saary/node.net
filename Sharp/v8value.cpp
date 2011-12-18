// v8sharp is a community project available under the Microsoft Public License (Ms-PL)
// Code is provided as is and with no warrenty – Use at your own risk
// View the project and the latest code at http://v8sharp.codeplex.com/

#include "v8value.h"
#include "v8external.h"
#include "v8function.h"

using namespace System::Collections::Generic;

class V8External;

namespace v8sharp 
{
    Object^ V8Interop::FromV8(v8::Handle<v8::Value> value) 
    {
        if (value->IsString()) 
        {
            return FromV8String(value);
        } 
        else if (value->IsBoolean()) 
        {
            return FromV8Boolean(value);
        } 
        else if (value->IsInt32()) 
        {
            return FromV8Int32(value);
        } 
        else if (value->IsArray()) 
        {
            return FromV8Array(value);
        } 
        else if (value->IsDate()) 
        {
            return FromV8Date(value);
        } 
        else if (value->IsNumber()) 
        {
            return FromV8Double(value);
        } 
        else if (value->IsFunction()) 
        {
            return FromV8Function(value);
        } 
        else if (value->IsExternal()) 
        {
            return FromV8External(value);
        } 
        else if (value->IsObject()) 
        {
            return FromV8Object(value);
        } 
        else if (value->IsUndefined() || value->IsNull()) 
        {
            return nullptr;
        }

        return nullptr;
    }

    v8::Handle<v8::Value> V8Interop::ToV8(Object^ value) 
    {
        if (value != nullptr) 
        {
            Type^ type = value->GetType();
            if (type == String::typeid)
            {
                return ToV8String(safe_cast<String^>(value));
            } 
            else if (type == bool::typeid) 
            {
                return ToV8Boolean(safe_cast<bool>(value));
            } 
            else if (type == int::typeid) 
            {
                return ToV8Int32(safe_cast<int>(value));
            } 
            else if (type == double::typeid) 
            {
                return ToV8Double(safe_cast<double>(value));
            } 
            else if (type->IsArray) 
            {
                return ToV8Array(safe_cast<Array^>(value));
            } 
            else if (type == DateTime::typeid) 
            {
                return ToV8Date(safe_cast<DateTime>(value));
            } 
            else if (Delegate::typeid->IsAssignableFrom(type)) 
            {
                return ToV8Function(safe_cast<Delegate^>(value));
            } 
            else 
            {
                return ToV8External(value);
            }
        }

        return v8::Null();
    }

    String^ V8Interop::FromV8String(v8::Handle<v8::Value> value) 
    {
        v8::String::AsciiValue ascii(value);
        return gcnew String((const char*)*ascii);
    }

    v8::Handle<v8::Value> V8Interop::ToV8String(String^ value) 
    {
        pin_ptr<const wchar_t> ptr = PtrToStringChars(value);
        return v8::String::New((const uint16_t*)ptr);
    }

    bool V8Interop::FromV8Boolean(v8::Handle<v8::Value> value) 
    {
        bool val = value->BooleanValue();
        return val;
    }

    v8::Handle<v8::Value> V8Interop::ToV8Boolean(bool value) 
    {
        return value ? v8::True() : v8::False();
    }

    int V8Interop::FromV8Int32(v8::Handle<v8::Value> value) 
    {
        int val = value->Int32Value();
        return val;
    }

    v8::Handle<v8::Value> V8Interop::ToV8Int32(int value) 
    {
        return v8::Int32::New(value);
    }

    double V8Interop::FromV8Double(v8::Handle<v8::Value> value) 
    {
        double val = value->NumberValue();
        return val;
    }

    v8::Handle<v8::Value> V8Interop::ToV8Double(double value) 
    {
        return v8::Number::New(value);
    }

    cli::array<Object^>^ V8Interop::FromV8Array(v8::Handle<v8::Value> value) 
    {
        v8::Array* val = v8::Array::Cast(*value);
        unsigned int len = val->Length();

        cli::array<Object^>^ result = gcnew cli::array<Object^>(len);
        for (unsigned int i = 0; i < len; i++) 
        {
            v8::Handle<v8::Value> key = v8::Uint32::New(i);
            result[i] = FromV8(val->Get(key));
        }

        return result;
    }

    v8::Handle<v8::Value> V8Interop::ToV8Array(Array^ value) 
    {
        int len = value->Length;
        v8::Handle<v8::Array> result = v8::Array::New();
        
        for (int i = 0; i < len; i++) 
        {
            v8::Handle<v8::Value> key = v8::Int32::New(i);
            result->Set(key, ToV8(value->GetValue(i)));
        }

        return result;
    }

    DateTime V8Interop::FromV8Date(v8::Handle<v8::Value> value) 
    {
        DateTime startDate = DateTime(1970, 1, 1);

        double milliseconds = value->NumberValue();
        TimeSpan ts = TimeSpan::FromMilliseconds(milliseconds);

        return DateTime(ts.Ticks + startDate.Ticks).ToLocalTime();
    }

    v8::Handle<v8::Value> V8Interop::ToV8Date(DateTime value) 
    {
        DateTime startDate = DateTime(1970, 1, 1);

        TimeSpan ts = TimeSpan::FromTicks(value.Ticks - startDate.Ticks);
        return v8::Date::New(ts.TotalMilliseconds);
    }

    Delegate^ V8Interop::FromV8Function(v8::Handle<v8::Value> value) 
    {
        v8::Function* function = v8::Function::Cast(*value);
        return V8FunctionHandler::CreateDelegate(function);
    }

    v8::Handle<v8::Value> V8Interop::ToV8Function(Delegate^ value) 
    {
        return V8FunctionHandler::CreateFunction(value);
    }

    Object^ V8Interop::FromV8Object(v8::Handle<v8::Value> value) 
    {
        v8::Handle<v8::Object> obj = value->ToObject();
        if (obj->InternalFieldCount() > 0) 
        {
            v8::Handle<v8::Value> internalField = obj->GetInternalField(0);
            if (internalField->IsExternal()) 
            {
                return FromV8External(internalField);
            }
        }

        v8::Local<v8::Array> names = obj->GetPropertyNames();
        
        unsigned int len = names->Length();
        Dictionary<String^, Object^>^ results = gcnew Dictionary<String^, Object^>(len);
        for (unsigned int i = 0; i < len; i++) 
        {
            v8::Handle<v8::Value> nameKey = v8::Uint32::New(i);
            v8::Handle<v8::Value> propName = names->Get(nameKey);
            v8::Handle<v8::Value> propValue = obj->Get(propName);

            String^ key = FromV8String(propName);
            results[key] = FromV8(propValue);
        }

        return results;
    }

    v8::Handle<v8::Value> V8Interop::ToV8Object(Object^ value) 
    {
        v8::Handle<v8::Object> obj = v8::Object::New();

        if (value->GetType() == Dictionary<String^, Object^>::typeid) 
        {
            Dictionary<String^, Object^>^ dict = safe_cast<Dictionary<String^, Object^>^>(value);
            for each(KeyValuePair<String^, Object^> pair in dict) 
            {
                v8::Handle<v8::Value> key = ToV8(pair.Key);
                v8::Handle<v8::Value> val = ToV8(pair.Value);

                obj->Set(key, val);
            }
        } 
        else 
        {
            for each(System::Reflection::PropertyInfo^ prop in value->GetType()->GetProperties()) 
            {
                v8::Handle<v8::Value> key = ToV8(prop->Name);
                v8::Handle<v8::Value> val = ToV8(prop->GetValue(value, nullptr));

                obj->Set(key, val);
            }
        }

        return obj;
    }

    Object^ V8Interop::FromV8External(v8::Handle<v8::Value> value) 
    {
        V8ExternalWrapper* FromV8per = V8ExternalWrapper::Unwrap(value);
        return FromV8per->Object;
    }

    v8::Handle<v8::Value> V8Interop::ToV8External(Object^ value) 
    {
        V8ExternalWrapper* wrapper = V8ExternalWrapper::Create(value);
        return v8::External::New((void*)wrapper);
    }
}