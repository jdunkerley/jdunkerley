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
8. Practice Papers (2 weeks)

**Total Time: 10 weeks**

# IAM

# EC2

# S3

# Serverless Computing

# DynamoDB

- Fast, flexible NoSQL db
- Single digit ms latency at any scale
- Fully managed
- Both document and key value data models
- Backed by SSD storages
- Always acreoss 3 DCs (avoids single point of failure)
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
         - Partition Key and Sort Key
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
- Scan:
    - Examines all entries in a table
    - Can filter but applied after dumping entire data
    - Slower than Query
    - Lower page size to stop Scan from blocking access to tables
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

*ToDo*

## Dynamo DB Streams
- Time order sequence of modification events (CUD)
- Stored for 24 hours
- Encrypted at rest
- Dedicated enpoint
- Trigger lambdas (lambda polls the stream)
- Near real time

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
- Deploying and scaling Web Apps
  - Deployment
  - Provisioning
  - Load Balancing / AutoScaling
- Written in Java, .Net, PHP, NodeJS, Python, Ruby, Go, Docker
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
- Code and configuration in an S3 bucket
    - Config written in JSON or YAML
    - Called `.config` in `.ebextensions` folder
    - .ebextensions in top level of application source code bundle
- Delete of environment deletes whole stack
- When using with RDS
    - Good for Dev / Test as database coupled with environment
    - For production, decouple and launch separately
        - Additional Security Group on ASG
        - Provided connection string configuration to application servers

# Kinesis

- Streaming data from 1000s of sources simultaneous in small sizes (Kbs)
- Load, Analyze Streaming Data
- 3 Services
    - Streams
        - Data stored in Shards
        - 24hr to 7 day retention
        - NUmber of Shards controls capacity of streams
        - Consumers read and send on
    - Firehose
        - No need to worry about shards or streams
        - Completely automated
        - Can use Lambda to analyse
        - Sends data to S3
        - No automatic retention - either straight to Lambda, S3, Elasticsearch
        - If to Redshift via S3
    - Analytics
        - SQL queries as it exists in Firehose or Streams
        - Store in S3, Redshift, Elasticsearch

# CodeStar - CI / CD

- [White Paper](https://d1.awsstatic.com/whitepapers/DevOps/practicing-continuous-integration-continuous-delivery-on-AWS.pdf)
- Cloud9 - Code Editor
- Code Commit (Git) ==> Code Build (Build System) ==> Tests ==> Code Deploy ==> Envronments
- Code Pipeline links it all together

## Code Commit
- Git
- HTTPS and SSH
    - IAM based need to create Git credentials in IAM
- Notifications to SNS or CloudWatch

## Code Build

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
    - `version`, `os` settings
    - `files` to copy from `source` to `destination`
    - `hooks` 
        - `BeforeInstall` and `AfterInstall`
        Set of commands to run (runas support and timeouts)
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
- Can integrate with Jenkins