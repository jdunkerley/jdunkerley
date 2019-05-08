# AWS Summit London - 8th May 2019

A group of us went to the AWS Global Summit in London. This is a quick review of my day. You will learn tons from a day like this, even

The day started with a (keynote)[] giving a overview of the current state of the AWS cloud. Review of the progress of Sainsbury's move to the cloud, mention of GDS and MoJ

## High Performance Computing (Frank Munz - @frankmunz)

- Used to be a saying that the best way to do HPC was to go surfing for 18 months
- Cloud stops the massive CapEx up front costs

1. Compute (EC2 Instances, Spot Instances, Autoscaling)
  - 190 instance types upto 4Ghz and 12TiB
  - `p3dn.24xlarge` type: 
     - `p` - instance type - GPU compute instances (most powerful)
     - `3` - version
     - `d` - (Capabilities) NVMe-bassed SSD disk 
     - `n` - (Capabilities) networking boost (100Gbs)
     - `23xlarge` - size of the machine
  - `Z1d` type:
     - `Z` - high speed single core speed (4GHz)
  - AMD EPYC processors      
     - `a` capability suffix
     - 10% cost saving
  - AWS Graviton processor
     - `A` capability suffix
     - ARM based
     - 45% cost saving but not binary compatible

2. Storage

3. Networking
  - Placement Groups
  - Network capabilities on EC2 instances
  - Elastic Fabric Adapter
    - Lowers the latency for HPC-class interconnect
    - Bypasses TCP stack and allows use of low latency MPI

4. FileSystem
  - AWS FSx Luster (Linux + Cluster)
    - Sub millisecond of latency
    - Data stored in S3 and loaded into the FSx Luster

AWS Parallel Cluster
 - Easy cluster management
 - Set of Python scripts
 - Build on top of Cloud Formations
 - Installs `pcluster` cli

Case Study of AstraZeneca use of AWS HPC for Genomics Research
- Challenges
  - Compute performance - on prmeise preformance capabilities 
  - Dependency on 3rd party providers
  - Operation Efficiency
- Identify variations in a genome, this gets picked out and loaded to a database and then final analysis looking at overall statistics based over the database
- Aiming for 20 million genomes, about 10 Petabytes of data
- Moved to AWS platform
  - Serverless technologies (Step Functions, Lambda, SQS)
  - Storage (DynamoDB, S3, Aurora MySQL)
  - Batch for running software when beyond serverless capabilities
     - FPGA compute used here
- Nothing is running most of the time, all triggered by autoscaling on demand
  - Receives batches of data
  - Very efficient both in operational and performance measures

## CI/CD On AWS (Danilo Poccia - @danilop)

- Infrastructure as code
- Continuous Integration using Code Pipeline
  - GitHiub or Code Commit
  - Triggered by push to ECR
  - Example of building a lambda and creating a zip within S3, sets up a Cloud Formation
  - Triggered by WebHooks (e.g. Quay or DockerHub)
  - CloudWatch events to trigger (scheduled builds)
- Continuous Deployment uning Code Deploy
  - Originally just EC2 but expanded to other services such as Lambda and ECS
  - Talk over deploying multiple versions of a lambda and using an alias to control deployment
  - Can use a PostTraffic hook to allow you to tag back in git repo that it was released
  - Yaml configuration
    - Type - Canary  `Canary10Percent5Minutes` / Linear `Linear10PercentEvery1Minute`
  - API Gateway can have a canary stage invoking a different lambda version (or function even)
    - multi functions backing an API
    - Percentages not managed by CodeDeploy - more like and A/B release process
  - ECS allows for blue-green environemnts
  - AWS Deployment Framework (ADF) = github awslabs

Dunelm talking about CICD within Serverless world - rapid, independent tribes (Tonino Greco - @toninog)
- SAM CLI wrapped in Ansible (as IaC is also in Ansible)
- Jenkins pipeline
- Centrally managed build library used by developers across board
    - Each tribe can customise what they need
- Git commit triggers jenkins webhook
    - Dedicated worker node (Spot instances) for each commit
    - Install build required software
    - Run a security checks
    - Build / Compile
    - Tests (Unit and Functional)
    - Deploy (SAM CLI in Ansible)
- &gt; 200 pipelines active, 15 deploys a day (generally 95% successful)
- Security audit built in
- Slack integration on state / result of builds. Also integrated some AWS environment control
- Lambda to clean up on branch delete or merge

## Modern Cloud Architecutres - 

- Innovate as rapidly as possible
  - Try lots of experiments
  - Reduce 'collateral damage' from failed experiment (CapEx not held, release resources)
- IT landscape has changed
  - Lots of interfaces and channels direct to end users (customers)
  - Become data driven
  - Rapid rate of change, innovation
- Core capabilities
  - Security
  - Resiliance (expected and unexpected failyre)
  - Elastic (scale up and down easily)
  - Modular
  - Automated (run and reverse experiments quickly and easily)
  - Interoperable
- Often not green field
  - Lift and shift (re-host) through to redesign (re-invent)
- Finra: Serverles stock trade verification service
  - 3 months to develop
  - Huge cost reduction
  - Only running when market is open
- Checklist:
  - Enable Security and Compliance across the entire app lifecycle
    - Durning both development cycle and running in live
  - Structure apps as a collection of microservices
    - Velocity and scaling
    - Can lead to organisational changes, lots of small teams working on small pieces (Conways Law)
    - Event driven architectures
  - Databases in modern applications
    - Relational or not? Strong consistency, normalisation
    - Use the database technology appropriate for choice
  - Build with serverles technologies as much as possible
    - Avoid heavy lifting of creating or managing environments
    - Concentrate on the value add code
    - For event driven, use functions as a service
    - For long running, look at Fargate for serverless containers
    - If need more control, options are there and can be added as needed
 - Infrastructure as Code
    - Only provision resources using code (services like Cloud Formation or Terraform)
    - Treat as source code
    - Declaritive state of what you want to be there (state diffed and updated)
    - Chagnes repeatable and predictable
    - Easy to build developer and test environments
 - SAM
    - IaC for serverless
    - Open source
    - Yaml definition
    - SAM CLI
    - Local development environment
  - Rapidly iterate with CI/CD tools
    - Ship more core faster with more confidence
    - Typically one pipeline per service
    - Feedback => Ideas => Experiemnt 'Innovation Flywheel'
  - Use monitoring to optimise end user experience
    - Complexity comes with Microservices
    - X-Ray for tracing