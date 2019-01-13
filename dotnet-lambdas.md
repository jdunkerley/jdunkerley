# Creating a Simple Lambda in C#

I've been using AWS Lambda for a little while now. It's a great platform and allows you to rapidly get something up and running. So far, I've built them in both JavaScript and Python - both of which have been very straight forward. I've spent a fair chunk of my development life working in C# so wanted to try putting one together in C# using .Net Core.

I've been planning to build a simple Diabetes log app for my own use and this will be the base of a series of posts on creating a serverless application on AWS (and possibly Azure as well). All the code will be on [GitHub](https://github.com/jdunkerley/diabeteslog/blob/).

## Getting Set Up

For this walkthrough, I will be using [.Net Core 2.2 SDK](https://dotnet.microsoft.com/download) but will have to target .Net Core 2.1 as this is the currently the highest supported version in Lambda. Lets start by getting an empty C# Class Library. If you create a new folder and then run the following within the folder:

```bash
dotnet new classlib
```

This will create the empty project structure based on the folder name (diabeteslog in my case):

```bash
├── Class1.cs
├── diabeteslog.csproj
└── obj
    ├── diabeteslog.csproj.nuget.cache
    ├── diabeteslog.csproj.nuget.g.props
    ├── diabeteslog.csproj.nuget.g.targets
    └── project.assets.json
```

First, we need to reconfigure the project to target `dotnetcore2.1`. If you edit the `.csproj` file to look like:

```xml
<Project Sdk="Microsoft.NET.Sdk">

  <PropertyGroup>
    <TargetFramework>netcoreapp2.1</TargetFramework>
  </PropertyGroup>

</Project>
```

Next, we need to add the reference to the Core Lambda library. Run:

```bash
dotnet add package Amazon.Lambda.Core
```

## Create The Simple Handler

Now we can set up the handler. This is the entry point that AWS Lambda will call. First, rename `Class1.cs` to `Handler.cs`. Change the code to:

```c#
using System;
using System.Threading.Tasks;

using Amazon.Lambda.Core;

namespace DiabetesLog
{
    public static class Handler
    {
        public static async Task EntryPoint(ILambdaContext context) 
        {
            context.Logger.Log($"Started Invocation - {context.AwsRequestId }");
            await Task.Delay(100);
            context.Logger.Log($"Finished Invocation - {context.AwsRequestId }");
        }
    }
}
```

This will provide a simple entry point for the Lambda to call. It will log on start, wait 100 milliseconds and then log on exit.

Verify the project can be build by running `dotnet build`.

## Publish The Lambda

For this initial Lambda, I am just going to use a simple PowerShell script to publish it to AWS. You will need to install the [AWS CLI](https://docs.aws.amazon.com/cli/latest/userguide/install-windows.html) to run this. You can verify that CLI is working by running `aws --version`. The output from mine is shown below:

```
aws-cli/1.15.10 Python/3.6.5 Windows/10 botocore/1.12.57
```

To publish a Lambda, we need to do the following:

- Build the package
- Create a zip file of the package
- Create a role and policy in AWS IAM
- Create the lambda

### Building and Zipping

The following script will build the dotnet DLL and then create the zip ready for upload:

```powershell
Push-Location $PSScriptRoot

dotnet publish -c Release -r linux-x64 . /p:GenerateRuntimeConfigurationFiles=true

Push-Location .\bin\Release\netcoreapp2.1\linux-x64\publish\
Compress-Archive -Path * -CompressionLevel Fastest -DestinationPath $PSScriptRoot\lambda_function.zip -Force
Pop-Location
```

This will move into the directory of the script and the build the project in Release mode. After this it will move into the build folder and compress the output into a zip file called `lambda_function.zip`.

### Creating the IAM Role and Policy

For this initial lambda all I want it to be able to do is log to CloudFront. The role we want to create looks like:

```json
{
    "Version": "2012-10-17",
    "Statement": [{
        "Effect": "Allow",
        "Principal": {
            "Service": "lambda.amazonaws.com"
        },
        "Action": "sts:AssumeRole"
    }]
}
```

And the policy looks like:

```json
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": "logs:CreateLogGroup",
            "Resource": "arn:aws:logs:<REGION>:<ACCOUNT>:*"
        },
        {
            "Effect": "Allow",
            "Action": [
                "logs:CreateLogStream",
                "logs:PutLogEvents"
            ],
            "Resource": [
                "arn:aws:logs:<REGION>:<ACCOUNT>:log-group:/aws/lambda/<FUNCTIONNAME>:*"
            ]
        }
    ]
}
```

We will need to substitute values for the region, account and function name. The function name will come from the name of the folder. The PowerShell script below will get the default region, account of the logged in user and the folder name:

```powershell
$folderName = Split-Path -leaf $PSScriptRoot
$region = aws configure get region
$accountId = aws sts get-caller-identity --output text --query 'Account'
```

The AWS CLI allows you pass JSON strings in as arguments. The syntax in PowerShell is a little odd. The strings need to be in single quotes with the quotes escapes. For example: `'{\"Input\": \"Value\"}'`. The code below will create the AWS Role and set the ARN for the role in `$roleARN` and create the Police and set the ARN for the policy in `$policyARN`:

```powershell
$roleARN = (aws iam create-role --role-name $folderName --assume-role-policy-document '{\"Version\":\"2012-10-17\",\"Statement\":[{\"Effect\":\"Allow\",\"Principal\":{\"Service\":\"lambda.amazonaws.com\"},\"Action\":\"sts:AssumeRole\"}]}' | ConvertFrom-Json).Role.Arn
$policy = (ConvertTo-Json -Depth 3 -Compress @{
    "Version" = "2012-10-17"
    "Statement" = @(
        @{
            "Effect" = "Allow"
            "Action" = "logs:CreateLogGroup"
            "Resource" = "arn:aws:logs:$($region):$($accountId):*"
        }
        @{
            "Effect" = "Allow"
            "Action" = @("logs:CreateLogStream", "logs:PutLogEvents")
            "Resource" = @("arn:aws:logs:$($region):$($accountId):log-group:/aws/lambda/$($folderName):*")
        }
    )
}).Replace("`"","`\`"")
$policyARN = (aws iam create-policy --policy-name $folderName --policy-document $policy | ConvertFrom-Json).Policy.Arn
aws iam attach-role-policy --role-name $folderName --policy-arn $policyARN
```

At this point having added these to the initial script, we now have covered the first 3 steps. The last is to create the Lambda. The entry point for a .Net based lambda is `Assembly::Namespace.ClassName::MethodName`. By default the Assembly will be equal to the `$folderName`.

```powershell
Start-Sleep -s 10
$namespace = (Get-Content .\Handler.cs | Select-String -Pattern "namespace " | Select -ExpandProperty Line).Replace("namespace ","")
aws lambda create-function --function-name $folderName --runtime "dotnetcore2.1" --handler "$folderName::$namespace.Handler::EntryPoint" --zip-file fileb://lambda_function.zip --role "$roleARN"
```

The first line gives IAM time for the Role to be properly created. After this we read the namespace from the Handler.cs file and then create the Lambda function.

The final step in my create script is to delete zip file and pop the location:

```powershell
Remove-Item .\lambda_function.zip
Pop-Location
```

Running this [completed script](https://github.com/jdunkerley/diabeteslog/blob/853a1b521b5a2a3171ba5670d7d5cb9f43221a8c/CreateLambda.ps1), you should get and output like:

```none
PS C:\Repos\diabeteslog> .\CreateLambda.ps1
Microsoft (R) Build Engine version 15.9.20+g88f5fadfbe for .NET Core
Copyright (C) Microsoft Corporation. All rights reserved.

  Restore completed in 40.64 ms for C:\Repos\diabeteslog\diabeteslog.csproj.
  diabeteslog -> C:\Repos\diabeteslog\bin\Release\netcoreapp2.1\diabeteslog.dll

Build succeeded.
    0 Warning(s)
    0 Error(s)

Time Elapsed 00:00:01.19
{
    "FunctionName": "diabeteslog",
    "FunctionArn": "arn:aws:lambda:eu-west-2:123456789:function:diabeteslog",
    "Runtime": "dotnetcore2.1",
    "Role": "arn:aws:iam::123456789:role/diabeteslog",
    "Handler": "diabeteslog::DiabetesLog.Handler::EntryPoint",
    "CodeSize": 4193,
    "Description": "",
    "Timeout": 3,
    "MemorySize": 128,
    "LastModified": "2019-01-07T23:12:27.778+0000",
    "CodeSha256": "Uhp7iyiCQOpA3rNE2z28yue8j01fnxHJ5p6i9E6r9r8=",
    "Version": "$LATEST",
    "TracingConfig": {
        "Mode": "PassThrough"
    },
    "RevisionId": "ac2bc9e5-d24a-4fde-a47b-0e11fc72e40f"
}
```

## Testing the Lambda

We now have created and published a Lambda function. The last part is to test execution. You can do this by running:

```powershell
aws lambda invoke --function-name (Split-Path -leaf .) test.log; cat test.log; Remove-Item test.log
```

This will invoke the lambda function and get the output in test.log. It will then display this output and finally delete the file. If you run this command you should get an output like:

```none
PS C:\Repos\diabeteslog> aws lambda invoke --function-name (Split-Path -leaf .) test.log; cat test.log; Remove-Item test.log
{
    "StatusCode": 200,
    "ExecutedVersion": "$LATEST"
}
```

## Tear Down Script

As it is always useful to be able to tear down as well as create. This is a fairly simple script:

```powershell
$folderName = Split-Path -leaf $PSScriptRoot
$accountId = aws sts get-caller-identity --output text --query 'Account'

aws lambda delete-function --function-name $folderName
aws iam detach-role-policy --role-name diabeteslog --policy-arn "arn:aws:iam::$($accountId):policy/$($folderName)"
aws iam delete-policy --policy-arn "arn:aws:iam::$($accountId):policy/$($folderName)"
aws iam delete-role --role-name $folderName
```

This will delete the lambda, role and policy created by the create script.

## Wrapping Up

So we have successfully created a simple Lambda function. It doesn't do anything yet but does set everything up we need. The next part will look at configuring an API Gateway and adjusting the function to respond to HTTPS requests.