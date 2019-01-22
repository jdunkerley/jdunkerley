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

 For this guide, I will be using the current version of Alteryx Design 2018.4 and Visual Studio 2017. 
 