# Using Azure DevOps to build, test and deploy a React Website to AWS

I've recently been working on building a new website in React and wanted to set up Continuous Integration and also Continuous Deployment. I'm keeping the code in a private [Azure Repo](https://azure.microsoft.com/en-gb/services/devops/repos/) so [Azure DevOps](https://azure.microsoft.com/en-gb/services/devops/) seemed the obvious choice to use. The site will be hosted in S3 behind a Cloud Front distribution using Route 53 as DNS.

As with many people, I chose to start with [Create React App](https://create-react-app.dev/) to create the initial project. I chose to use TypeScript as I am a big fan of the additional tooling that the typing provides.

```bash
npx create-react-app website --template typescript
```

This creates the basic website you can build and run:

- `yarn start` - starts the development server
- `yarn build` - builds the website
- `yarn test` - starts and runs the test runner

When developing, it's easy to work with the combination of the development server and the test runner. Within the CI/CD space, I want to do the following:

- Lint the code
- Build the code
- Test the result and publish the test results
- Deploy the result to S3

Lets look at making these work locally first.

## Linting and Code Style

Create React App sets up eslint inside itself, however I'm a fan of the [JavaScript Standard Style](https://standardjs.com). I chose to use eslint to verify the code style. Running `npx eslint --init` allows for easy set up of the linter for the project:

```
? How would you like to use ESLint? To check syntax, find problems, and enforce code style
? What type of modules does your project use? JavaScript modules (import/export)
? Which framework does your project use? React
? Does your project use TypeScript? Yes
? Where does your code run? Browser
? How would you like to define a style for your project? Use a popular style guide
? Which style guide do you want to follow? Standard: https://github.com/standard/standard
? What format do you want your config file to be in? JavaScript
```

Then an adjustment to packages.json to add a new script:

```json
{
  "scripts": {
    "lint": "eslint --ext .js,.ts,.jsx,.tsx src"
  },
}
```

This instructs eslint to check all JavaScript and TypeScript files within the `src` folder. Running `yarn lint` shows that even before starting building the site there are a few things to fix. Fortunately, a quick `npx standard --fix` sorts them all out.

The last thing I need to add to the linter is the support for `jest`. Running `yarn add --dev eslint eslint-plugin-jest` adds the additional plugin. Then you need to add a couple entries into the `.esliint.js` file:

```json
{
  "env": {
    "jest/globals": true
  },
  "plugins": {
    "jest"
  }
}
```

The complete eslint file is available [here](https://gist.github.com/jdunkerley/317a1955561fee1e261977155bf5f5ba).

## Running the Test in a CI environment and outputting the results

Create React App sets up a testing framework by default. It is based on top of [Jest](https://jestjs.io/) and easily allows you to test your code as you work. However, the built in `yarn test` command runs in watch mode and doesn't work within a CI environment. To make it work within Azure DevOps, we need to do two things. One set an environment variable called `CI` equal to `true`. 

*bash:*
```bash
CI=true yarn test
```

*PowerShell:*
```powershell
($env:CI = "true") -and (yarn test)
```

*Windows Command Prompt:*
```bat
set CI=true&&yarn test
```

Annoyingly setting environment variables vary a lot between bash, PowerShell and the Windows Command prompt. The Azure hosted [build environment](https://docs.microsoft.com/en-us/azure/devops/pipelines/agents/hosted?view=azure-devops) can be Windows, Ubuntu or Mac OS X. I chose to build on top of `ubuntu-latest` so will need to use the appropriate command.

After this, we need to make the process produce an XML file of test results in JUnit format (which Azure DevOps understands). There is an npm package jest-junit which provides a JUnit reporter to jest. Run `yarn add --dev jest-junit` and then edit the test command in `packages.json` to:

```bash
react-scripts test --reporters=default --reporters=jest-junit
```

Now if you run `yarn test` a new file `junit.xml` will be generated. I added this to my `.gitignore` as didn't want to commit it.

# Creating the Build File

So we're now ready to create the build pipeline. The easiest way to do this is to add a YAML file with the build configuration. I called mine `azure-pipelines.yml` (which is the default name if you use the wizard in Azure DevOps).

```yaml
trigger:
- master

pool:
  vmImage: 'ubuntu-latest'

steps:
- task: NodeTool@0
  inputs:
    versionSpec: '10.x'
  displayName: 'Install Node.js'
```

This will set up and create the VM and wire it to be build on every commit to master. It also sets the Node.js version. Next lets add the build, lint and tests steps:

```yaml
- script: |
    yarn install
  displayName: 'yarn install'

- script: |
    yarn lint
  displayName: 'yarn lint'
  
- script: |
    CI=true yarn test
  displayName: 'yarn test'
```

If we push this up to the repo the build pipeline is automatically created. On the next commit, it will be built. The last command for setting up the CI part of the pipeline is to capture the test output:

```yaml
- task: PublishTestResults@2
  condition: succeededOrFailed()
  inputs:
    testRunner: JUnit
    testResultsFiles: '**/junit.xml'
```

The complete YAML file is [here](https://gist.github.com/jdunkerley/3ee989aa52a6daf3b9dc3810c933c759).

