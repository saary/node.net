// v8sharp is a community project available under the Microsoft Public License (Ms-PL)
// Code is provided as is and with no warrenty – Use at your own risk
// View the project and the latest code at http://v8sharp.codeplex.com/

#include "v8external.h"

V8ExternalWrapper::~V8ExternalWrapper() 
{
	/*if (this->Object != nullptr && System::IDisposable::typeid->IsAssignableFrom(this->Object->GetType()) {
		safe_cast<System::IDisposable^>(this->Object)->Dispose();
		this->Object = nullptr;
	}*/
}

V8ExternalWrapper* V8ExternalWrapper::Create(gcroot<System::Object^> obj) 
{
	V8ExternalWrapper* wrapper = new V8ExternalWrapper();
	wrapper->Object = obj;
	return wrapper;
}

V8ExternalWrapper* V8ExternalWrapper::Unwrap(v8::Handle<v8::Value> value) 
{
	v8::External* external = v8::External::Cast(*value);
	V8ExternalWrapper* wrapper = static_cast<V8ExternalWrapper*>(external->Value());
	return wrapper;
}