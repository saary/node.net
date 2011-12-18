// v8sharp is a community project available under the Microsoft Public License (Ms-PL)
// Code is provided as is and with no warrenty – Use at your own risk
// View the project and the latest code at http://v8sharp.codeplex.com/

#pragma once

#include <v8.h>

using namespace System;

namespace v8sharp 
{
	public delegate Object^ V8Function(cli::array<Object^>^ args);

	ref class V8FunctionHandler sealed {
	internal:
		static Delegate^ CreateDelegate(v8::Function* function);

		static Delegate^ CreateDelegate(v8::Function* function, Type^ delegateType);

		static v8::Handle<v8::Function> CreateFunction(Delegate^ function);
	};
}