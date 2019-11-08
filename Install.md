```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
choco install chocolateygui
choco install 7zip
choco install yarn
choco install filezilla
choco install git
choco install python
choco install pip
choco install dotnetcore-sdk
choco install kubernetes-cli
choco install azure-cli
choco install vscode
choco install vscode-insiders
choco install pandoc
choco install putty
choco install cascadiacode
# choco install microsoft-windows-terminal
choco list --local-only
```

Install:
- Windows Feature: windows subsystem for Linux
- Windows Store: Ubuntu

