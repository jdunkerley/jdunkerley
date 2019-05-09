# AWS Summit London - 8th May 2019

![Conference](assets/awssummit/conference.jpg)

I and a group of my colleagues from Scott Logic went to the AWS Global Summit at the ExCel centre yesterday. It is the biggest of the global summits, and gives a taste of the huge Re:Invent conference. This post is my thoughts from my day and the sessions I attended. There is a huge range of sessions (about 75) on offer, and this is just a taste from the ones I went to.

## Keynote

The day started with an impressively quick registration process (last time took an age!) and a decent range of coffee and pastries before heading into the keynote. The keynote was presented by Matt Garman, VP of Compute Services at AWS, giving a overview of the current state of the AWS cloud. The platform continues to grow at an astounding rate with a annual run rate now approaching *31 billion* dollars a year, which is a 41% growth for last year.

![]()

Two of the customer case studies in the the
Review of the progress of Sainsbury's move to the cloud, mention of GDS and MoJ

![]()

The Keynote also highlighted two of AWS programs. The first, [AWS re:start](https://aws-restart.com/) gives training and placements to young people and military veterans who possibly havent had any experience and gets them up to speed on 

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
    - AWS Cloud Map to track estate

## Stream Processing and managing Real Time Data (Javier Ramirez - @supercoco9)

- Challenges of data streaming
  - Set up
  - Scaling
  - High availability
- AWS streaming on cloud
  - Various tech on platform but managed by AWS
  - AWS Managed Streaming for Kafka (v1.1.1/v2.1.0)
  - Flink on top of Kinesis Data Analytics
  - Managed ElasticSearch as the query engine
- Kinesis is fundamental service (billing and cloudwatch)
  - Serverless platform - play for traffic in the stream
  - Lambda for transform (e.g. Apache Log to JSON) but some built in already
- Streams vs Firehose
  - Latency on streams is much lower, Firehose up to 60 seconds
  - Data streams can have server based applications
  - Data streams very like Kafka, allows replay of events
  - Data streams need number of shards
    - Can use cloud watch alarms to auto scale up and down
    - Not fully automatically (still a lot easier than Kafka with physical hardware)
    - Library in awslabs
  - Can mix and match (e.g. Firehose takes Stream as an input or vice versa)
- Analytics
  - Interact with streams in real time with SQL or Java applications (Apache Flink)
  - SQL allows for rolling windows (either overlapping or not)
  - Stream joined to static data
  - Fully managed and elastic

- NYC TLC Trip Record Data

Real time analytics at HSBC (Prakash Sethuraman)

- Scale (169PB data, trillions of dollars every day) and Diversity (global nature)
- What and How at Re-Invent 2018
- Moved from batch to stream processing
- Legacy main frame for golden source, aim to get to modern cloud stores
- Direct connect from DC to AWS
- Deliver to mobile apps
- Kinesis allows them to keep it flexible, no large capex, chop and change as needed
- Use Kafka within the DC  (Change Data Capture)
- Then through NiFi (choosen for throughput) ==> push out rather than reach in
- Store duplicated data to allow for faster responses
- Failed experiments can be switched of (hard in a bank...)
- Roughly 0.5BB messages with 0.25BB events
- All secure from the beginning, users have to come back in to get the details
- DLS (SNS/SQS)
- Lambda Latencies (spin up time, ENIs - reuse where possible)
- Design of VPC
- Eaxctly once processing is hard

## Deep Dive on Serverless

Comic Relief gave an awesome overview of their platform.

## Wrapping Up

![Innovation Cycle](assets)

It was great to spend a day immersed in AWS. The cloud technologies continue to grow at an incredible rate, with new concepts and technologies all the time. The summit gave us a great chance to be buried in what people are doing and see successful implementations of various customers. I think one of the big aspects that was enphasised by various of the real world case studies was the cloud and CI?CD technologies allow for a very fast innovation cycle where we shouldn't be scared to have failed experiments as the