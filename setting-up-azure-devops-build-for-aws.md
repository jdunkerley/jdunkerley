# Setting Up Azure DevOps As CI From GitHub to AWS Lambda

Pre-requisites:
- Have a Azure DevOps account
- [VSTS CLI](https://docs.microsoft.com/en-gb/cli/vsts/install?view=vsts-cli-latest)


Create PAT
https://docs.microsoft.com/en-gb/azure/devops/organizations/accounts/use-personal-access-tokens-to-authenticate?view=vsts

```powershell
vsts login
vsts configure
vsts configure --defaults instance=https://jdunkerley.visualstudio.com/
```

Create project

```powershell
$folderName = Split-Path -leaf $PSScriptRoot
vsts project create --name $folderName
```