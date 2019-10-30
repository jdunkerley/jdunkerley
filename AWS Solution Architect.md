# AWS Solution Architect (Professional)

This assumes you have AWS Solution Architect (associate) level knowledge.

## Exam Warning

- A lot to read
- 2 minutes a question
- AWS is out to trick, answers will often be very close to viable but some keyword will say why not

## Data Stores

Focussing on:
- New and existing solutions
- Migrations
- Reliability / High Availability / Business Continuity
- Both Database and Files
- Cost control components

Concepts
- Types:
    - Persistent: Durable, sticks around over reboots
        - S3, Glacier, RDS, EBS
    - Transient: Temporary stored while passed between
        - SQS, SNS
    - Ephemeral: Only while instance exists, lost in reboot or on EC2 termination
        - Instance store, Memcached
- IOPS vs Throughput
    - Input/Output Operations per Seconds: How fast can read/write (speed)
    - Throughput: How much data can be moved at a time (volume)
- Consistency Models
    - ACID: Atomic, Consistent, Isolated, Durable (transactions, don't scale well)
    - BASE: Basically Available, Soft state ==> Eventual Consistency (cluster slowly updated)

- Remember Encryption!

## Service Overview

- S3
    - Object store
        - More like a database than file system
        - Access by key (not a file path)
        - Think of it like a multi-AZ database will eventual consistency
        - Can cross region replicate
    - Backs a lot of AWS services
    - Size: 0b to 5TB
        - Single put limit is 5GB
        - Multi-part upload if > 100MB
    - Consistency:
        - Read after write in new PUTs
            - If you have made a HEAD/GET before creation, then it will be eventually consistent as needs to update the cached states
        - Evential consistency for overwrite PUTs/DELETEs
        - Updates to a single key are atomic (ordered by timestamp)
    - Security:
        - Can you access S3, then bucket, then object
        - IAM policy on User/Role
        - Resource based: Object ACL, Bucket Policy
    - Supports versioning (allows undelete as well) 
        - billable size is total count, lifecycle management
    - Analytics services can use S3 for storage or as data source
    - Storage Class Analysis to optimise storage type
    - Other bits:
        - Transfer acceleration using CloudFront to upload
        - Requester pays for data transfer
        - Events onto SNS/SQS/Lambda
        - Supports bit torrent protocol
    - Recommended to use endpoint within your VPC for S3
    - *Ensure you know how to properly secure a bucket*
- Glacier
    - Cheap, slow to respond and seldom accessed
    - Cold storage
    - Integrated with S3 and AWS Storage Gateway Virtual Tape Library
    - Faster retrieval speeds (a few minutes) at cost
    - Glacier Vault (like a bucket), Archive (like an object), IAM and Policy (Glacier Vault Lock)
    - Max file size is 40TB
    - Files are immutable (can be deleted, created or a whole new version)
    - Process to add vault lock, Initiate a Vault Lock (24 hours to Abort/Complete)
- Elastic Block Storage (EBS)
    - Virtual Hard Drive with EC2
    - Single AZ can only attach to EC2 in same one (use snapshots to move AZ/Region)
    - Pay for space of drive even if unused
    - Instance stores are temporary and locked to a single EC2, better performance vs EBS
    - Snapshots for backup or for sharing with others or to encrypt a drive
        - Deltas on top of full
        - Cost is sum of all
        - On deletes it merges into later (tags in Git)
- Elastic File System (EFS)
    = NFS drives for EC2
    - Only pay for what you use (Cost 3x EBS, 20x S3)
    - Multi AZ metadata and data storage
    - If from on-premise, NFS via Direct Connect or EFS File Sync agent
- Storage Gateway
    - VMWare or HyperV virtual machine for on-premise
    - Can be run as EC2
    - Local storage backed by S3/Glacier
    - Useful for DR or Cloud Migrations
    - Modes:
        - File Gateway: NFS/SMB mounts to S3
        - Volume Gateway Stored Mode (Gateway-stored volume): iSCSI drive on ASG async replicated to S3
        - Volume Gateway Cached Mode (Gateway-cached volume): iSCSI drive with data primary on S3 with frequently used data cached on ASG
        - Tape Gatway (Gateway-Virtual Tape Library): iSCSI tape library for backup
    - Can support bandwidth limiting allowing better experience in remote offices
- WorkDocs: Dropbox/Google drive
- Database on EC2:
    - Full control/flexibility but have to manage everything
    - Good if RDS not support type (e.g. SAP HANA)
    - Can be quicker migration path
    - Remember operational costs (patching, replications)
- RDS
    - MySql (+Aurora), SQL Server, Oracle, Postgres, Maria
    - Drop in replacement with automated backups, scaling, replication and redundancy
        - For MySQL must use InnoDB (XtraDB on Maria) if need replication
    - When not to use:
        - Lots of Blobs (S3)
        - Automated scaling, Name/Value pairs, unstructured (Dynamo DB)
        - Other platforms or complete control (EC2)
    - MultiAZ/Read Replicas
        - MultiAZ is sync replication and if one fails another becomes master
        - ReadReplicas is async replication
        - If whole region fails then:
            - Promote read replica to stand alone SingleAZ
            - Set up MultiAZ
            = Restore other read replicas
        - Avoids backup performance hit if run Multi AZ mode
- DynamoDB
    - Paper by Werner: Dynamo - Amazon Highly Available Key Value Store
      https://www.allthingsdistributed.com/files/amazon-dynamo-sosp2007.pdf 
    - Managed Multi AZ NoSQL with Cross Region replication
    - Eventual consistency (BASE) but can do Strongly
        - Use DynamoDB Transactions if need ACID compliance
    - Priced on throughput rather than compute
    - Provision Read/Write capacity
        - Can autoscale but some limits
        - Can use On-demand where no knowledge (premium)
    - Primary key - either just a Partition key or Partition and Sort keys
    - Indexes: 
        - Global secondary index (as Primary Key but different fields)
        - Local Secondary index (same partition different sort)
        - Max of 5 Global and 5 Local across at most 20 fields
        - Increases storage space
        - Project other properties you need
- RedShift
    - Petabyte scale data warehouse
    - Cost effective versus on premise
    - PostgreSQL compatible, columnar data store, parallel processing
    - Can use S3 data files RedShift Spectrum (data lake concept)
- Neptune
    - Graph database with support from Gremlin and SPARQL
- Elasticache
    - In memory
    - Either Redis or Memcached
    - Cost per node and time
    - Use Redis to work around multicast issue
    - Memcached is more basic, cache objects, dynamic scaling
    - Redis has encryption, HIPPA, clustering/replication, pub/sub, geospatial indexes, backup/restore

*WhitePapers*
- https://d1.awsstatic.com/whitepapers/Storage/aws-storage-options.pdf

