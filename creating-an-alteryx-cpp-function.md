# Creating An Alteryx Custom Function in C++

At [Inspire EU 18](https://www.alteryx.com/inspire-europe-2018/), I presented a talk on [how to create custom functions](https://www.youtube.com/watch?v=vm2H9TXJ0Pc) for Alteryx. As I described, there are two options available - XML Macro functions and C++ Coded functions. In a [previous post](https://jdunkerley.co.uk/2016/08/13/beyond-alteryx-macros-part-2-how-to-create-an-xml-macro-function/), I have gone through creating an XML function. This post will look at creating a very simple function - a simple `SUM` function adding all numerical inputs.

## Why Would I Want To?

The main reasons for wanting to create new functions is to extend functionality. Technically, the XML Macro functions are just a shorthand to existing expressions (although this is still a very powerful abilty) - you cannot make Alteryx do anything fundamentally new using this method. The C++ coded functions however allow you to do anything you like within a function (assuming your C++ is up to it!).

They remove a lot of the limitation that the XML function have. It is straight forward to take a variable amount of inputs and handle appropriately. You can handle invalid inputs gracefully and return errors (with hopefully helpful messages) back to the workflow.

In addition, you can test these with C++ testing frameworks. XML functions need to be tested from within Alteryx and while I am a strong proponent of test workflow the added benefit of addition unit tests are good.

## Some Initial Warnings

As with all SDK functions or tools, any workflow which uses them can only be run on machines which have them installed. If you export a workflow it will only export macros, it does not package custom tools or functions. I would love to see a NuGet style packing system built into Alteryx where workflows will install `yxi` files they need. 

Unlike many of the SDKs within Alteryx, the custom function SDK is not used by Alteryx themselves. This is one of the very few places I have encountered bugs in the engine. In all cases, they have been easy to work around and quickly fixed when reported to Alteryx.

Finally, this SDK is very old - it been there ever since I became a user (but that was only version 9). Unfortunately, there are a few limitations with it. For example, there is no way to detect you are in parse mode as opposed to execution mode.

## Getting Started

For this guide, I will be using the current version of Alteryx Design 2018.4 and [Visual Studio 2017](https://visualstudio.microsoft.com/downloads/). Community edition is perfectly good enough. I think you could also build these in any standard C++ compiler with any editor but I haven't had time to try. 

First, we need to create the new project in Visual Studio and configure as we need. Go to New Project and find C++. In my case, it is in `Other Languages`. You may need to adjust your installation to include the C++ project types if not there. Once you have found it, you need to create a `Dynamic-Link Library (DLL)`.

![New Project](assets/alteryx-cpp-function/create-project.jpg)

This will take a few seconds to create the project. Next, configure the project. Alteryx is 64 bit (since v11 there hasn't been a [32-bit version](https://www.alteryx.com/support-32-bit-windows-systems)) - so first I remove x86 from the list. Goto *Build* and then *Configuration Manager*, select the drop down in Active Solution Platform and go to Edit.

![Configuration Manager](assets/alteryx-cpp-function/configuration-manager.jpg)

The next step is to adjust the project properties so it compiles with the C++ Standard Library embeded inside the resulting DLL. While not strictly necessary for your own PC, this will make it a lot easier to distribute the resulting code. In the Solution Explorer, choose the project and go to properties.

![Project Properties](assets/alteryx-cpp-function/project-properties.jpg)

Inside here, within `C/C++` and then `Code Generation`, there is the innocent sounding `Runtime library`. This is by default on `Multi-threaded DLL (/MD)` for Release mode and `Multi-threaded Debug DLL (/MDd)` for Debug mode. This means it will using the runtime dynamically. If you use this locally or are using the same version as Alteryx uses then it will work fine. However, for general compatibility if you switch to `Multi-threaded (/MT)` for Release mode and `Multi-threaded Debug (/MTd)` for Debug mode, then it will embed the standard library into the compiled DLL. This makes the DLL a little bit bigger but easy to distribute.

I then choose to delete `stdafx.cpp`, `targetver.h` and `dllmain.cpp` files. You also need to remove the line to `#include "targetver.h"` in `stdafx.h`. This leaves a project structure which looks like:

```none
├── AlteryxTotals
│   ├── AlteryxTotals.cpp
│   ├── AlteryxTotals.vcxproj
│   ├── AlteryxTotals.vcxproj.filters
│   ├── AlteryxTotals.vcxproj.user
│   └── stdafx.h
└── AlteryxTotals.sln
```

## Enough with the setup on with the code...

First, we need to define the Alteryx types needed. Add a new header file, say called `AlteryxFunctions.h` and set the content to:

```cpp
#pragma once
#include "stdafx.h"

// Constants For nVarType
const int nVarType_DOUBLE = 1;
const int nVarType_WCHAR = 2;

//  API definition for Formula functions plug ins
struct FormulaAddInData
{
	int nVarType;
	int isNull;

	double dVal; 
	const wchar_t * pVal;

	FormulaAddInData()
	{
		memset(this, 0, sizeof(*this));
	}
};

// Function Defintion
typedef long(_stdcall * FormulaAddInPlugin)(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue);
```

### Too much detail on the header file

For those of us not familiar with header files (pretending for a second I know them inside and out), lets talk about what is in here.

```cpp
#pragma once
#include "stdafx.h"
```

The `#` symbol means these are preprocessor directives. The first line is a "Header Guard" which means the file is only processed once by the compiler no matter how many times it is referenced. The second line includes the `stdafx.h` file, this is needed to define the `memset` function used later. Moving onto the next block:

```cpp
// Constants For nVarType
const int nVarType_DOUBLE = 1;
const int nVarType_WCHAR = 2;
```

This defines constants to identify the variable type Alteryx passes into your function. All custom function variables are passed either as double values (assuming a numberic type in Alteryx) or as unicode character array (assuming a text or date time value in Alteryx). Alteryx tells you what time the variable is by passing one of the two values above. The constants just mean you can use friendly code and not need to remember which one is which!

```cpp
struct FormulaAddInData
{
	int nVarType;
	int isNull;

	double dVal; 
	const wchar_t * pVal;

	FormulaAddInData()
	{
		memset(this, 0, sizeof(*this));
	}
};
```

The above struct is how Alteryx will pass values into and how you are expected to return your results. Looking at the members:

- `nVarType`: defines the value type (double or wchar*)
- `isNull`: indicated if the value is `NULL` - 1 for `NULL`, 0 otherwise
- `dVal`: if the value is not `NULL` and is a double (i.e. if `isNull == 0 && nVarType == nVarType_DOUBLE`) then will be the value
- `pVal`: if the value is not `NULL` and is a wchar* (i.e. if `isNull == 0 && nVarType == nVarType_WCHAR`) then will be the value

The last part defines the constructor for the struct. It is worth noting that the engine of Alteryx is amazingly memory efficient and it will reuse the struct over and over, hence you must check the `isNull` for each execution.

```cpp
typedef long(_stdcall * FormulaAddInPlugin)(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue);
```

This is the definition of a custom C++ function in Alteryx. They all look like this. Lets run over the definition:

- `typedef`: This tells the compiler to create an alias for a function pointer
- `long`: This specifies the return type as a long. If you function succeeds you should return 1 and 0 otherwise
- `_stdcall`: This is a calling convention for Win32 API.
- `* FormulaAddInPlugin`: This specifies that this type definition is a pointer and is called `FormulaAddInPlugin`
- `int nNumArgs`: Alteryx will tell you how many arguments you have been passed. When we get to the XML definition for the function, you can define a variable number of arguments and then this will tell you how many we passed.
- `FormulaAddInData *pArgs`: The input arguments are passed as an array.
- `FormulaAddInData *pReturnValue`: This is struct how you return the value back from your function to Alteryx.

## Onto the C++ Code

Now to write the function. Open the cpp file already present. Add an include for the `AlteryxFunction.h`:

```cpp
#include "AlteryxFunction.h"
```

Next, we define the function:

```cpp
extern "C" long _declspec(dllexport) _stdcall Total(int nNumArgs, FormulaAddInData *pArgs, FormulaAddInData *pReturnValue)
{
	pReturnValue->nVarType = nVarType_DOUBLE;

	double total = 0;
	for (auto argNum = 0; argNum < nNumArgs; argNum++)
	{
		if (pArgs[argNum].nVarType != nVarType_DOUBLE)
		{
			return 0;
		}

		if (pArgs[argNum].isNull)
		{
			pReturnValue->isNull = 1;
			return 1;
		}

		total += pArgs[argNum].dVal;
	}

	pReturnValue->isNull = 0;
	pReturnValue->dVal = total;
	return 1;
}
```



## XML Definition File

## Installing In Alteryx

## Wrapping It Up