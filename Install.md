Initial Install:
- Chrome
- Office 365

```powershell
Set-ExecutionPolicy Bypass -Scope Process -Force; iex ((New-Object System.Net.WebClient).DownloadString('https://chocolatey.org/install.ps1'))
choco install chocolateygui
choco install 7zip
choco install nodejs
choco install yarn
choco install filezilla
choco install git
choco install python
choco install pip
choco install dotnetcore-sdk
choco install kubernetes-cli
choco install azure-cli
choco install awscli
choco install vscode
choco install vscode-insiders
choco install pandoc
choco install putty
choco install cascadiacode
choco install treesizefree
choco install r.studio
choco install postman
choco install paint.net
# choco install microsoft-windows-terminal
choco list --local-only
Enable-WindowsOptionalFeature -Online -FeatureName Microsoft-Windows-Subsystem-Linux
Invoke-WebRequest -Uri https://aka.ms/wsl-ubuntu-1804 -OutFile Ubuntu.appx -UseBasicParsing
Add-AppxPackage .\Ubuntu.appx
```

Install:
- Adobe Acrobat Reader
- DropBox
- JDK
- JetBrains Toolbox
- Tableau
- [Syncfusion Metro Studio](https://www.syncfusion.com/downloads/metrostudio)
- Alteryx
- Alteryx Predictive Tools
- PowerBI
- SnagIt
- Logi Options
- SQL Server Management Studio
- Visual Studio Professional
- Printer
- Desk set up

Toolbox
- IntelliJ
- PyCharm
- DataGrip
- DotPeek
- DotTrace
- DotMemory

VS PlugIns
- Productivity Power Tools

VS Code plugins
```bash
code --install-extension visualstudioexptteam.vscodeintellicode
code --install-extension donjayamanne.githistory
code --install-extension eamodio.gitlens
code --install-extension mauve.terraform
code --install-extension ms-python.python
code --install-extension ms-vscode-remote.remote-wsl
code --install-extension ms-dotnettools.csharp
code --install-extension ms-vscode.powershell
code --install-extension ms-vsonline.vsonline
code --install-extension msjsdiag.debugger-for-chrome
code --install-extension octref.vetur
code --install-extension vscode-icons-team.vscode-icons
code --install-extension mauve.terraform
code --install-extension alexcvzz.vscode-sqlite
code --install-extension eg2.vscode-npm-script
code --install-extension HookyQR.beautify
```

VS Code Insider plugins
```bash
code-insiders --install-extension visualstudioexptteam.vscodeintellicode
code-insiders --install-extension ms-vscode-remote.remote-wsl
code-insiders --install-extension donjayamanne.githistory
code-insiders --install-extension eamodio.gitlens
code-insiders --install-extension davidanson.vscode-markdownlint
code-insiders --install-extension streetsidesoftware.code-spell-checker
```

Python Packages
```bash
python -m pip install --upgrade pip
pip install pytest
pip install pylint
pip install rope
pip install boto3
pip install flask
pip install flask-cors
pip install requests
pip install feedparser
pip install numpy
pip install scipy
pip install pandas
pip install pandas-datareader
pip install joblib
pip install peewee
pip install workalender
pip install tzlocal
pip install beautifulsoup4
pip install spacy
```

Yarn
```bash
yarn global add @vue/cli
```

Alteryx Extensions
- Crew Macros
- Abacus
- Omnibus

ToDo:
- Microsoft Keyboard Mouse
- SmallBasic
- Slack
- Visio
