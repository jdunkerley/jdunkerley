

# Lets Get Terraform Set Up

- [Download the distribution](https://www.terraform.io/downloads.html) for the OS of your choice
- Extract the executable and add to path
- Run `terraform -version`

- Ensure az command and find 
az account list --query "[].{name:name, subscriptionId:id, tenantId:tenantId}"

foreach ($name in ()) { echo $name }


```powershell
pip list -o --format=json | ConvertFrom-Json | Where-Object {$_.version -ne $_.latest_version} | ForEach-Object { & pip install $_.name --upgrade } 
```

```powershell
$env:ARM_SUBSCRIPTION_ID=($(az account show --query id) -replace '"', '')
```