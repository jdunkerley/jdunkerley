# Creating a Simple Azure Function in JavaScript with VS Code

![Azure Function Logo](assets/azure-functions/logo.jpg)

Azure Functions are a serverless way to create application within the Azure cloud. They can be written in a variety of languages (C#, JavaScript for example) using a variety of tools (or just in the web UI). This post walks through creating a simple HTTP server which will return the current time. The function will be written in JavaScript using VS Code (and all the comamnds I use can be executed within the integrated PowerShell terminal there as well).

## Why Using Functions

Serverless architecture is very popular at the moment. They allow you to easily get an application or service up and running and easily scale from a tiny instance through to managing a huge number of requests. The 'pay and go' nature of serverless instance means you only pay for what you need and don't have redundant processes running unnecessarily. In addition due to the nature of the serverless instance, there are some nice security benefit in that each execution of process is running in a known state.

[Azure Functions](https://azure.microsoft.com/en-us/services/functions/) allow you to provide a simple application with different 'hooks' which trigger it to run. These can be simple web hooks (as will be the case in this walkthrough) or events on other cloud based services (for example a file being written to OneDrive). One nice benefit of Azure Functions is that they can easily be tied into services of other vendors. They are of course not alone, AWS offers [Lambdas](https://aws.amazon.com/lambda/) and GCP has [functions](https://cloud.google.com/functions/) (which is still in beta).

Their pricing model is based on the compute time of the function. As there is no virtual machine or even container running when the function isn't executing this is a great model. Both [Azure](https://azure.microsoft.com/en-us/pricing/details/functions/) and [AWS](https://aws.amazon.com/lambda/pricing/) have the same free tier of 400,000 GB-seconds or 1 million requests. Exactly how quickly you will go through the GB-s will depend on complexity of the function and the size of the host instance you choose.

So which one to use - AWS, Azure of GCP? At the time of writing, GCP is still in beta and not subject to any SLA or guarentees. For that reason, I would probably select one of the other two. The language range of AWS and Azure may be a factor - for example AWS support Java where as Azure does not. Lambdas are more limited to other AWS services but if you are already in AWS that is probably not an issues. The two big things that I think Azure functions have over AWS lambda are great local debugging support and the ability to deploy on premise. I won't cover the on premise side in this post but read the overview of [Azure Function Runtime](https://docs.microsoft.com/en-us/azure/azure-functions/functions-runtime-overview) for more details.

## Getting Started

You will need a new version of [NodeJS](https://nodejs.org/en/) (version 8.5 or later) installed. In addition you will need the [azure-functions-core-tools](https://www.npmjs.com/package/azure-functions-core-tools) npm package installed. Version 1 of library this only works on Windows (as it uses the .Net Framework), however there is a version 2 currently in preview which uses .Net Core and can be run anywhere. This guide uses version 1.0 on Windows. To install the functions run:

```bash
npm i -g azure-functions-core-tools@core
```

which will produce something like:

```none
C:\Repos\AzureFunctionFirst> npm i -g azure-functions-core-tools
C:\Users\jdunk\AppData\Roaming\npm\azurefunctions -> C:\Users\jdunk\AppData\Roaming\npm\node_modules\azure-functions-core-tools\lib\main.js
C:\Users\jdunk\AppData\Roaming\npm\func -> C:\Users\jdunk\AppData\Roaming\npm\node_modules\azure-functions-core-tools\lib\main.js
C:\Users\jdunk\AppData\Roaming\npm\azfun -> C:\Users\jdunk\AppData\Roaming\npm\node_modules\azure-functions-core-tools\lib\main.js

> azure-functions-core-tools@1.0.8 postinstall C:\Users\jdunk\AppData\Roaming\npm\node_modules\azure-functions-core-tools
> node lib/install.js

+ azure-functions-core-tools@1.0.8
added 45 packages in 18.724s
```

### func init

To create the project, run

```bash 
func init AzureFunctionsDemo
```

This will create a new folder called `AzureFunctionsDemo` ready to host functions with:

```none
├── .vscode
|   └── launch.json
├── .gitignore
├── host.json
└── local.settings.json
```

It will also run `git init` on the folder. The `.gitignore` is set to ignore various .Net and JS files by default. 

The `local.settings.json` is a file allowing for local connections and settings needed to run or debug the funcion locally. It is listed in the `.gitignore` so it won't be committed. You can read more on the [online documentation](https://docs.microsoft.com/en-us/azure/azure-functions/functions-run-local#local-settings-file).

The `launch.json` file configures how VS Code will debug the functions. Again this won't be committed to git. The set up is just to attach to a node instance.

The `host.json` file is empty at this point but it will be where all the settings go needed to run the functions either locally or in Azure. The file is currently empty but we will populate later in this post.

### func templates and func new

Next is to create our first function. There are various templates built into the npm library. If you run `func templates list` you will get a list of all the templates for all the langauages (103 templates currently in 9 languages). If you run `func templates list -l JavaScript` then you just get the JavaScript ones

```none
  Blob trigger
  Cosmos DB trigger
  Event Grid trigger
  Event Hub trigger
  External file trigger
  Face locator
  Generic webhook
  GitHub commenter
  GitHub webhook
  HTTP GET
  HTTP POST
  HTTP trigger
  HTTP trigger with parameters
  IoT Hub (Service Bus Queue)
  IoT Hub (Service Bus Topic)
  IoT Hub (Event Hub)
  Manual trigger
  Queue trigger
  SAS token generator
  SendGrid
  Service Bus Queue trigger
  Service Bus Topic trigger
  Timer trigger
```

For this case I am going to use the 'HTTP trigger with parameters'. Within the `AzureFunctionsDemo` folder run:

```bash
func new -l JavaScript -n HTTPTrigger -t 'HTTP trigger with parameters'
```

This will create a new folder caller `HTTPTrigger` so that the whole tree looks like:

```none
├── .vscode
|   └── launch.json
├── HTTPTrigger
|   ├── function.json
|   └── index.js
├── .gitignore
├── host.json
└── local.settings.json
```
