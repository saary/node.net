// v8sharp is a community project available under the Microsoft Public License (Ms-PL)
// Code is provided as is and with no warrenty – Use at your own risk
// View the project and the latest code at http://v8sharp.codeplex.com/

#pragma once

#include <v8.h>
#include <vcclr.h>

class V8ExternalWrapper 
{
public:
	V8ExternalWrapper() {}

	~V8ExternalWrapper();

	gcroot<System::Object^> Object;

	static V8ExternalWrapper* Create(gcroot<System::Object^> obj);

	static V8ExternalWrapper* Unwrap(v8::Handle<v8::Value> value);
};