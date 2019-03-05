This is my set of notes for the AWS Developer Associate Exams. It based on having done the SA Associate

# Weekly Guide
*Times shown as at 18-Jan-2018*

1. Revisit IAM / EC2 / S3 (5:26:18 - but content all covered in SA)
2. Look at Serverless (2:26:45)
3. Build something in Serverless (2 week project)
4. DynamoDB (1:36:02)
5. KMS and Other AWS Services (1:37:54)
6. Developer Theory (2:50:50)
7. Advanced IAM, Monitoring (1:15:42)
8. Practice Papers (2 weeks - 5 mock tests allowing about 2 hours for each test and review)

**Total Time: 10 weeks**

## Reviewed

- Free Test - June 2018
- Free Test CDA
- Test 1
- Test 2
- Test 3
- Test 4
- Test 5

# IAM

- Use CloudTrail to monitor STS
- To use a role run `STS:AssumeRole` to get access as the role
- If get error can use `aws sts decode-authorization-message` to get detail

## Policy Types
- Managed Policies (owned by AWS, cannot be edited => copy to customer manage)
- Customer Manager Policy (only with own account)
- Inline policy (embedded into the User, Group or Role ==> Customer Manager generally recommended) 
- `aws iam simulate-custom-policy` to test permissions. Need to get context keys to supply to CLI

## Web Identity Federation
- Authenticate with other internet providers (e.g. Google, Facebook, Amazon, SAML (Active Directory) and Open ID)
- `Cognito` acts a identity brokers between AWS and internet providers
    - OAuth 2.0 flows
    - Can customise look and feel (logo labels etc)
    - Can hold user pool as well as other providers (built in signup, signin and guest users)
        - Can have password rules and MFA requirements
        - Verification code flow
        - Has groups as well as users with mapping to IAM
    - User logs into provider, get JWT token from broker (Facebook), cognito exchanges for temporary limited IAM token
    - `Push Syncronization` with SNS to send user updates to devices
    - SAML
        - Security Assertion Markup Langauge 2.0
        - Endpoint https://signin.aws.amazon.com/saml
        - https://docs.aws.amazon.com/IAM/latest/UserGuide/id_roles_providers_enable-console-saml.html
        - SAML Federation for organisation SSO to AD
    - Streams
        - Allows access to data stored in Cognito
    - Can detect compromised credentials on User Pool
        - Use `Block use` in advanced security
        - Checks on Sign In, Sign Up, Password Change


# EC2

# ECS

- Fully managed orchestration service
- Run a docker container
- IAM Role controls access to resources (like an EC2) set at Task level
- Part of the VPC so usual VPC access
- Can use Securty Group on instances to isolate
- Often stick an Application LB in front of ECS

# S3

- Public url: https://[bucket].s3-website-[region].amazonaws.com
- MultiPart upload - 3 steps: Initiate, Upload part, Complete
- Rough performance is 3,500 writes / second and 5,500 reads / second
    - If encrypted will be bottlenecked by KMS as well (5,500 / second)
- Still use prefix if absolutely needed...
- To block unencrypted uploda need to us Bucket Policy Denying if no `x-amz-server-side-encryption`
- 503 errors can be when have millions of versions of a file - check inventory
- CloudFront TTL use minimum TTL and origin can add Cache-Control or Expires headers

# Serverless Computing

- Can use a dead letter queue for failed function invocations 
    - Either SQS and SNS
    - Function retried twice if invoked asyncronously
    - Need to grant permission to lambda to `SendMessage` to SQS or `Publish` to SNS
- Lambda defaults to 3s for timeout (max 15 minutes)
- Step function to orchestrate performing discrete functions or tasks and co-ordinate
- Lambda@Edge allows for code in front of CloudFront

## Lambda ALIAS

- By default points at single version (pointer to specific version)
- `routing-config` allows you to point at two versions
- Controls percentage at each version

## API Gateway

- Client ==> Method Request ==> Integration Request ==> Backend
- Backend ==> Integration Response ==> Method Response ==> Client
- Method are API interface and frontend
- Integration are where API interacts with bacekn

# DynamoDB

- Fast, flexible NoSQL db
- Single digit ms latency at any scale
- Fully managed
- Both document and key value data models
- Backed by SSD storages
- Encryption must be chosen at creation
- Always across 3 DCs (avoids single point of failure)
- Global tables
    - multi region
    - multi master
- Consistency - Eventual (default - usual within 1s) or Strongly
- Model: 
    - Tables, Items (like a row), Attributes (like a column)
    - Key: Name of Data, Value: data
    - Documents in JSON, HTML or XML
- Stored by Primary Key (also called HashKey)
    - 2 Types: 
    - Partition Key
         - Used as part of hash to determine physical location data is stored
         - No 2 items can have same Primary Key
    - Composite Key
         - Partition Key (HASH) and Sort Key (RANGE)
         - 2 items may have same partition key but not primary key
         - Same Partition Key stored together, then sorted by sort key
- Access managed by IAM
    - Control access and creation
    - Create role allowing you to get temporary access keys to access table
    - IAM Condition allowing only own record access
         - Use UserID as Partiion Key and then make access only if match
         - `dynamodb:LeadingKeys` value name
- Two classes of Table
    - Standard
    - Global
- Supports indexes
    - Performance boost as per SQL
    - 2 Types: Local or Global Secondary Index
    - Local Secondary Index
        - Created at table creation (and fixed)
        - Same Partition Key, different sort key
    - Global Secondary Index
        - Add whenever
        - Different partition key and sort key
- Transactions (Reinvent 2018)
    - ACID (Atomic, Consistent, Isolated and Durable)
        - Ideal properties of transaction 
    - All or nothing action across multiple tables
    - Durable across system failure
    - e.g. buying an item in a game
- TTL
    - Time to live
    - Expiry time for data
    - Stored as attribute in Unix timestamp format
    - Reduce storage cost
    - Automatically deleted at some point after (within 48 hours) 
    - Can filter in scans/queries
    - Manage TTL lets you set attribute and lets you preview state

## Query or Scan
- By default All attributes
    - `ProjectionExpression` to get subset
- Query: 
    - finds by Primary Key (not necessarily unique)
    - Option Sort Key name and value to refine
    - Always sorted by Sort Key
        - Can be reversed using `ScanIndexForward` parameter
        - *Only on Queries, despite name*
    - Eventually Consistent by Default, need to specify for Strongly Consistent
    - Can use secondary index as well
- Scan:
    - Examines all entries in a table
    - Can filter but applied after dumping entire data
    - Slower than Query
    - *Lower page size* to stop Scan from blocking access to tables
    - Can run scan in parallel scans
        - Logically divide table into segment and scan in parallel
        - Impact on performance
- Additionally Get and BatchGet
    - Retrieve by Primary Key(s)

## Provisioned Throughput
- Specify in Read and Write Capacity Units (per second)
    - 1 WCU = 1x 1KB Write
    - 1 RCU = 1x Strongly Consistent Read of 4KB or 2x Eventually Consistent Read of 4KB
- Support for autoscaling 
    - On-Demand Capacity pricing (Reinvent 2018)
    - Pay per request
    - Use if Unknown or unpredicatble workload
- Can change price model once a day
- Write costs more than read
- Partial items round up
- ProvisionedThroughputExceededExceptions
    - Request rate too high for capacity
    - AWS SDK will automatically retry using exponential back off (feature of every AWS SDK)
    - If hand rolled then use exponential back off approach
- `ReturnConsumedCapacity` in query to get used capacity: NONE (off) / TOTAL / INDEXES

## DyanamoDB Accellerator (DAX) or Elasticache
- In Memory cache for DynamoDB
    - Write through cache - i.e. written at same time as Table update
    - If present gets from Cache on read (Cache Hit)
    - Otherwise does *Eventually Consistent* read (Cache Miss)
- Point API calls at DAX cluster rather than DynamoDB
- Read only boost
- 10x moves to microsecond performance
- Idea for Read-Heavy or burst performance
- Can use Elasticache as well if prefered
    - Memcached or Redis
    - Memcached is not Multi-AZ
    - Redis has option of multi az redundancy
    - Allows both Write Through or Lazy Load
        - Have to watch for data becoming stale on Lazy Load
        - Handle this by having a Time To Live in Elasticache (doesn't avoid, just lowers change)
        - Lazy Load has less data being held so more resource efficient
    - Use for RDS

## Creating DynamoDB
- Initially permission EC2 or Lambda via IAM (DynamoDB Full DB)
- Create a table
```
aws dynamodb create-table
  --table-name students
  --key-schema
  --attribute-definitions
```

## Dynamo DB Streams
- Time order sequence of modification events (CUD)
- Stored for 24 hours
- Encrypted at rest
- Dedicated enpoint
- Trigger lambdas (lambda polls the stream)
- Near real time
- Creation of materialised views

# KMS

- Key Management Service
- Main difference from CloudHSM is that KMS is shared hardware rather than dedicated
- Manage encryptions keys
- 2 roles - use (encrypt/decrypt) or manage
  - Can add external users as well as IAM Users and Groups
- Included in EBS, S3, RDS, Redshift, Workmail, Elastic Transcoder and others 
- Part of IAM
- *Not global* - keys are regional
- When creating a key - Key material can be from KMS or external
- When deleting a key
  - First disable the key
  - Then schedule deletion (within 7 - 30 days)
- Customer Master Key
  - Metadata: Alias, creation date, description and key state
  - Key material 
  - Can not be exported (need to use Cloud HSM if you want to export)
- Envelope Encryption
  - Envelope Key (key used to encrypt data)
  - Envelope Key is encrypted by the customer master key
  - Data Key is decrypted Envelope Key
  - Command `GenerateDataKey` or `GenerateDataKeyWithoutPlaintext`
- KMS API Calls
  - aws kms encrypt --key-id <KeyName> --plaintext <File/Text> --output text --query CipherTextBlob
  - aws kms decrypt --key-id <KeyName> --ciphertext-blob <File/Text> --output text --query PlainText
  - aws kms re-encrypt --ciphertext-blob <File/Text> --destination-key-id <KeyName>
    - Take encrypted one, decrypt and then re-encrypt it
  - aws kms enable-key-rotation
    - Key will automatically be rotated once a year
    - only is key material from KMS (not imported)

# SQS

- Decouple components
- Oldest AWS service
- Webservice access to a queue
- 256KB of text (any format e.g. JSON/XML)
- Retention is 1 min to 14 days (default 4 days)
- Poll based system not a push based systems (use SNS)
- Can work as a buffer layer (producer faster than consumer)
  - Could use length of queue to autoscale processors
- Messages marked as invisible while being processed
  - If processing fails re-appears after visibility time out
  - Default time out is 30 s 
  - Maximum time out is 12 hours
- Two Types of Queue
  - Standard
    - Delivered at least once
    - Best effort ordering
    - Nearly unlimited rate
  - FIFO
    - Exactly once 
    - Firt in first out
    - Max of 300 tx / s
    - Support message groups 
- Long Polling
  - Doesnt return from request until either timeout or a message sent
  - Reduces costs
- Compatible with JMS (v1.1) - only standard queues
- Delay Queues - can lag a message up to 15 minutes (default is 0). Can set on indiviual messages.
- Can have message attributes as well as payload

# SNS

- Web service to send notifications
- Push to mobile devices, SMS, email, SQS or HTTP endpoint
  - Also can trigger a lambda
  - Simple APIs
- Grouped by topics
  - Allows recipients to subscribe
  - Support different target types and more than one subscriber
- Redundant storage across multiple AZs
- Push based (pub-sub)
  - User can add filter policy when they subscribe to get subset
- Pricing
  - $0.5 / 1m Requests
  - $0.06 / 100k HTTP deliveries
  - $0.75 / 100 SMS
  - $2 / 100k emails
- SES
  - Simple Email Service
  - Automated emails (e.g. Marketing email, shipping email)
  - Can receive email as well (to S3 or trigger SNS / Lambda)
  - Not subscription based just need email of target

# Elastic Beanstalk

- https://www.youtube.com/watch?v=SrwxAScdyT0
- Deploying and scaling Web Apps (or Docker environments)
  - Deployment
  - Provisioning
  - Load Balancing / AutoScaling
- Written in Java, .Net, PHP, NodeJS, Python, Ruby, Go, Docker
    - Packer to create custom environments using AMI and Platform.yaml
- Widely used platfroms Tomcat, NGINX, IIS, etc.
- Can be within a VPC
- Can include additional resources such as RDS
- GUI driven
  - Control of EC2 type
  - EBS can fully manage or can take over full EC2 management
  - Managed platform updates 
      - OS, PHP etc
      - Control times
- Pay for and control of deployed resources
- EBS Deployment Policy
    - All at Once
        - All simultaneous
        - All go out of service (not for prod systems, for test & dev)
        - If fails would need to roll back
        - Works on single instance
    - Rolling
        - Deploys in batches 
        - Performance impact as cluster shrinks by batch size
        - If fails would need to roll back
    - Rolling with Additional Batch
        - Adds another batch
        - Performance not impacted
        - If fails would need to roll back
    - Immutable
        - Completely new fresh instances in new auto scaling group
        - Once healthy moved to existing group and old terminated
        - Preferred option for mission critical systems
        - Roll back easy as just involves killing new ASG
        - Works on single instance
    - Blue/Green
        - As per immutable but swap URL in DNS at end
        - Create new environment, deploy to it, swap URLs
- Code and configuration in an S3 bucket
    - Config written in JSON or YAML
    - Called `.config` in `.ebextensions` folder
    - `.ebextensions` in top level of application source code bundle
    - Use to change the instance size (S3 file)
    - Precedence: Default, .ebextensions, Saved Config, Settings directly applied
- Delete of environment deletes whole stack
- When using with RDS
    - Good for Dev / Test as database coupled with environment
    - For production, decouple and launch separately
        - Additional Security Group on ASG
        - Provided connection string configuration to application servers
    - Can control what happens with `Retention` setting - e.g. Create snapshot to keep DB
- Every deployment creates a version
    - Will hit version limit
    - Use Application Version Lifecycle policy to delete old versions
    - Each application can have multiple versions
- Periodic stuff use a `cron.yaml` file
- EB CLI allows for monitoring and working with environment
- Change environment to change runtime

# Kinesis

- Streaming data from 1000s of sources simultaneous in small sizes (Kbs)
- Load, Analyze Streaming Data
- 3 Services
    - Streams
        - Data stored in Shards
        - 24hr to 7 day retention
        - Number of Shards controls capacity of streams
            - Max of incoming write / 1000 and outgoing read / 2000 
        - Consumers read and send on
        - Can only control order within a shard using `sequenceNumberForOrdering` parameter
    - Firehose
        - No need to worry about shards or streams
        - Completely automated
        - Can use Lambda to analyse / transform
        - Sends data to S3
        - No automatic retention - either straight to Lambda, S3, Elasticsearch (+Splunk)
        - If to Redshift via S3
    - Analytics
        - SQL queries as it exists in Firehose or Streams
        - Store in S3, Redshift, Elasticsearch

# CodeStar - CI / CD

- [White Paper](https://d1.awsstatic.com/whitepapers/DevOps/practicing-continuous-integration-continuous-delivery-on-AWS.pdf)
- Cloud9 - Code Editor
- Code Commit (Git) ==> Code Build (Build System) ==> Tests ==> Code Deploy ==> Envronments
- Code Pipeline links it all together
- CodeStart is the overall service for SDLC in cloud

## Code Commit
- Git
- HTTPS and SSH
    - IAM based need to create Git credentials in IAM
- Notifications to SNS or CloudWatch
- Cross Account
    - Create cross account IAM role
    - Grant role access
    - Provide ARN to users

## Code Build
- Fully managed code build service
- e.g. CodeCommit => Docker Build ==> ECR host on ECS
    - Code in Git in Code Commit
    - ECS Cluster running Linux ASG in a VPC
    - Docker repository in ECR
    - Docker build and then push to ECR (docker build, docker tag, docker push)
    - Create a Task definition in ECS for image
    - Create a Service to launch the Task in Cluster (allows control of placement of instances as wel)
- Controlled by `buildspec.yml`
    - Defaults to spec file in source code
    - Can be hard coded in console (useful if cant change source code)
    - Can be passed in `start-build` command (`buildspecOverride`)
- Environment variables (`env`) section (Key Value Pair)
    - Constants or from Paramater Store 
- Phases (`phases`): 
    - Install, PreBuild, Build, PostBuild
    - Sequence of shell commands
- Specify output `artifacts`
- CodeBuild runs on a docker image
    - AWS provide some standard one (Ubuntu)
    - Can be any image
- Shortened logs in console, full logs in CloudWatch
- To access VPC resources need to add VPC-specific configuration

## Code Deploy
- Deploy to EC2, on-premise of Lambda
- Integrates with Jenkins, GitHub, CodePipeline
- Config management tools Ansible, Puppet and Chef
- Two deployments methods In-Place or Blue/Green
    - In-Place (or Rolling update):
         - One/Half/All instance at a time is upgraded 
         - Capacity loss, rollback involves redeploy
         - Take out of Load Balancer during upgrade
         - Not available for Lambda
    - Blue-Green
         - New instances created and deployed
         - No capacity loss
         - New instances added to ELB and then ready
         - Much easier to switch back if problem (just ELB config)
- `Deployment Configuration` set of rules as well as success/fail conditions
- `appspec.yml` file defines the deployment actions
    - Defines parameters for code deploy
    - Same location as files to deploy
    - EC2 (YAML):
        - `version`: for future use, currently 0.0
        - `os`: windows or linux
        - `files` to copy from `source` to `destination`
        - `hooks` 
            - `BeforeBlockTraffic`, `BlockTraffic`, `AfterBlockTraffic`
            - `ApplicationStop` 
            - `DownloadBundle`
            - `BeforeInstall`, `Install` and `AfterInstall`
            - `ApplicationStart` and `ValidateService`
            - `BeforeAllowTraffic`, `AllowTraffic`, `AfterAllowTraffic`
            Shell script to run (runas support and timeouts)
    - Lambda (YAML or JSON):
        - `version`: for future use, currently 0.0
        - `resources`: name and properties of Lambda
        - `hooks`: as per EC2 - e.g. to valdiate deployment at stages `BeforeAllowTraffic` and `AfterAllowTraffic`
        - Can specify version of Lambda to be deployed
        
- Code Deploy agent installed on EC2 or On-Premise machines
- Service role in IAM for CodeDeploy controls permissions
    - Download from AWS URL and run install
    - Runs as service on AWS Linux
- Example deploy a web app from S3 bucket to EC2
    - `aws deploy create-application --application-name mywebapp`
    - `aws deploy push --application-name mywebapp --s3-location s3://bucket/stuff.zip --ignore-hidden-files`
    - Next:
        - Define deployment group which is set of target machines
        - Choose Deployment type
        - Choose ASG, EC2 by Tag, OnPremise
        - Point at Load Balancer for config
    - Create the Deployment
        - Gives additional options about file overwrites
        - Can override default roll back configuration

## Code Pipeline
- CI/CD service
- Orchestrate build, test and deployment on code changes
- Links with Lambda, Elastic Beanstalk, Cloud Formation, ECS
- Source code from CodeCommit, GitHub, or S3
    - Trigger by CloudWatch alert
    - S3 enable versioning and trigger on new version
- Can integrate with Jenkins

# Cloud Formation

- Infrastructure as code - Manage, configure, provision
    - Version control
    - Consistency
    - Time and effort
- Defined in templates (YAML or JSON)
    - Store in S3 (can be uploaded directly in console into S3 bucket)
    - Sample templates from AWS
- Free to use
- Under management tools
- Dependencies built in sequence
- Managed as Stacks (build and delete)
- Structure:
    - `AWSTemplateFormatVersion: "2010-09-09"`: specifies file format (only one supported)
    - `Description`
    - `Metadata` - custom fields
    - `Parameters` - Input values, provided at stack launch by user
    - `Conditions` - Custom expressions to allow template to make decisions
    - `Mappings` - Define values in a dictionary key (e.g. RegionMap)
    - `Transform` - Include snippets of code from other files in S3 (https://docs.aws.amazon.com/AWSCloudFormation/latest/UserGuide/CHAP_TemplateQuickRef.html), code re-use
    - `Resources` - The AWS Resources you are deploying (*required!*)
    - `Outputs` - Outputs from the scripts, can be passed downstream
- Rollback on failure by default
    - Can be disabled for debugging etc
    - --disable-rollback from CLI/API
- Nested Stacks
    - Allow for reuse of template within a template
    - Part of `Resources` section as a Stack type
    - Must have `TemplateURL` can have `Parameters`
- For EC2 can use `cfn-init` to install software on instances
- StackSets extend stacks over multiple regions and account
- ChangeSets used to change running resources

## Serverless Application Model (SAM)

- Cloudformation extension for serverless
- Simplified syntax
- Add a `Transform: AWS::Serverless-2016-10-31` line to template after Version
    - Tells AWS is SAM template
- Place yaml file in same folder as Lambda code
- Package whole lot including YAML to S3 bucket
- SAM CLI
    - sam package: Package all the local resources for a SAM to s3-bucket (applies transform)
    - sam deploy: deploys the serverless app using CF

# Monitoring

- CloudWatch - monitor performance and logs
- CloudTrail - monitors API calls to AWS (think audit)
- AWS Config - records state of AWS environment and notifies of changes (think version control of environment)
- XRay allow you to trace through execution path
    - Needs IAM permission to write to XRay
    - Docker image in ECS
    - Use interceptors to catch all HTTP requests
    - XRay Lambda Environment Variables
        - `_X_AMZN_TRACE_ID` - trace ID for XRay
        - `AWS_XRAY_CONTEXT_MISSING` - what happens if no trace ID (error log by default)
        - `AWS_XRAY_DAEMON_ADDRESS` - IP:PORT for XRay daemon
- To monitor ELBs look at the access logs

## Cloudwatch

- Can aggregrate in CloudWatch using statistic sets
- Can monitor Compute (ASG, ELBs, R53 Healthcheck), Storage and CDN, Databases and Analytics
- Also monitors billing (alerts on threshold) 
- Gathers logs into log streams
- EC2 Monitors by default (CPU, Network, Disk IOs, Status Check) every 5 mins (cost for every 1 minute) 
    - Cannot monitor by default RAM or Disk space - custom metric (minimum of every 1 minute)
- Stored indefinitely by default (including terminated instances)
- Can get data using GetMetricStatistics API
- Alert on all metrics, trigger an action including a lambda
- Can include outside resources of AWS - SSM agent and Cloudwatch agent
- Default EC2 metrics are 5 min (basic monitoring), detailed 1 min, High resolution metrics allow for 10s or 30s
- Alerts over multiple evaluation periods, control for period length, number of points
- For logs from EC2 need to install CloudWatch agent