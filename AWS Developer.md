This is my set of notes 

# S3

# Lambda

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
    - Documents in JSON, HTML or ML
- Stored by Primary Key
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