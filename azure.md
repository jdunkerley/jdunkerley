# Azure

## Certification

Azure Certified Administrator (Associate) - Powershell and CLI
- AZ-103 - Microsoft Azure Administrator

Azure Certified Developer (Associate) - Coding focused
- AZ-203 - Developing Solutions for Microsoft Azure
  - https://microsoftlearning.github.io/AZ-203-DevelopingSolutionsforMicrosoftAzure/

Azure Certified Architect (Expert)
- AZ-300 - Microsoft Azure Architect Technologies
- AZ-301 - Microsoft Azure Architect Design

Architect covers most of Developer and Administrator

Exam format:
- 40-60 querstion, 150 minutes (time not normally an issue)
- Some questions can't be reviewed (watch out for those)
- Some practical questions (Performance based testing) - built in hands on lab
- Moving away from multiple choice, make it harder to pass from pure memory

## Resources

- **Tools**: https://azure.microsoft.com/en-ca/tools/
- **Labs**: https://www.microsoft.com/handsonlabs/SelfPacedLabs
- **Azure Samples**: https://github.com/Azure-Samples/
- **Docs**: https://docs.microsoft.com/en-us/azure/
- **Azure Citadel**: https://azurecitadel.com/

## Overall

- Why cloud - Hosting Responsibilities:

| Resonsibility | Self | Co-Located | Dedicated Host | Cloud |
|---|---|---|---|---|
| Upfront Costs (CapEx) | 1 | 1 | 1 | 0 |
| Ongoing Costs (OpEx) | 1 | 1 | 1 | 1 |
| Setup Time | 1 | 1 | 1 | 0 |
| Computing Hardware | 1 | 1 | 0 | 0 |
| Electricity | 1 | 0 | 0 | 0 |
| Physical Space | 1 | 0 | 0 | 0 |
| Physical Security| 1 | 0 | 0 | 0 |
| Internet Service | 1 | 0 | 0 | 0 |
| 24/7 Support | 1 | 1 | 0 | 0 |
| Firewall security | 1 | 1 | 1 | 0.5 |
| Patches and updates | 1 | 1 | 1 | 0.75 |
| Application | 1 | 1 | 1 | 1 |
| Monitoring | 1 | 1 | 0.5 | 0.5 |

- Azure Services

** AWS Equivalent in brackets! **
- **Compute**: 
  - Virtual Machines (EC2)
  - Functions (Lambda)
  - Container Instances (ECS)
  - Batch (Batch)
  - Service Fabric (??)
  - VM Scale Sets (Auto Scaling Group)
  - Azure Kubernetes Service (EKS)
- **Networking**:
  - CDN (CloudFront)
  - DNS (Route53)
  - ExpressRoute (Direct Connect)
  - Traffic Manaer (??)
  - Load Balancer (ELB/ALB/NLB)
  - Application Gateway (API Gateway)
  - VPN Gateway ()
  - DDoS Protection (WAF)
  - Network Watcher (Guard Tower?)
- **Storage**:
  - Blob Stroage (S3)
  - File Storage (EFS, FSx) - SMB 3.0 and HTTPs
  - Disk Storage (EBS)
  - Queries
  - Data Lake Store
  - Backup
  - Site Recovery
  - Managed Storage
  - StorSimple
- **Web + Mobile**:
  - Web Apps
  - Mobile Apps
  - API Apps
  - Logic Apps
  - API Management (API Gateway)
  - Media Services - stream real time videos for example
  - Azure Search
  - Notification Hub
- **Databases**:
  - SQL Database (RDS)
  - SQL Data Warehouse (Redshift)
  - SQL Server Stretch DB - allows an on premise SQL Server to 'stretch' into the cloud
  - Azure PostgreSQL / MySQL
  - Table Storage 
  - Redis Cache ()
  - Cosmos DB (Dynamo DB)
- **Analytics**:
  - HDInsight (Redshift)
    - with Spark, Storm and R Server
  - Stream Analytics
  - Log Analytics
  - Data Factory
  - Data Catalog
  - Event Hubs
  - Data Lake Analytics
  - Azure Analysis Services
  - Azure Databricks
- **AI + ML**:
  - ML Studio
  - Cognative Services
  - Bot Service
  - Batch AI
  - Genomics
  - ML Services
- **IoT**: (millions of small bits of data)
  - IoT Hub
  - IoT Suite
  - IoT Edge
  - Event Grid
  - Time Series Insights
  - Location Services
- **Enterprise**:
  - Service Bus
- **Security**:
  - Azure AD
    - Azure AD Domain Services
    - B2B
    - B2C
  - MFA
  - Key Vault
  - Security Center
- **Developer Tools**: 
  - Azure DevOps (Team Services)
  - DevTest Labs
  - Application Insights
  - HockeyApp
  - App Center
- **Management**:
  - Scheduler
  - Automation
  - Azure Portal
  - Cloud Shell
  - Mobile App
  - Cost Management
  - Azure Migrate
  - Azure Advisor
  - Azure Monitor

# Geostructure

- Launched in February 2010 as Windows Azure
- March 2014 rebranded to Microsoft Azure
- Currently 42 regions
  - Several data centers in a region (can be 1 DC)
  - Not all are equal
  - Doesn't let you choose which DC (or Availability Zone)
  - Each region is paried (e.g. East US paired to West US) 
    - Highest availability if do that
    - Highest speed connection between them
    - No necessarily bi-directional
- Geographies contain the regions: Americas, Europe, Asia Pacific, Middle East & Africa
- Some regions support Availability Zones (growing number)

## Account

- Account holds Name, email, credit card/billing details
  - Authenticated by Azure AD (could be at individual, team, organisation level)
  - Email can be associated with multiple tennants within Azure AD
- Account can have multiple subscriptions within it
- Subscription types: Free / Pay as you Go / Enterprise Agreement / Student
- One common use is have Dev / Test / Production as different subscriptions and control access
- Billed monthly at subscription level
- All resources must be in a Resource Group which are part of a Subscription
- Resources can be tagged as well
- Support: Developer / Standard / Professional Direct / Premier

## Virtual Machines

- ARM templates which can be used for automation

- Access via RDP (Remote Desktop Protocol - port 3389) or SSH (port 22)
- Pay for machine, managed disk and storage transaction
  - Pay by minute
  - Reductions if agree for reserved (1y ~ 66%, 3y ~ 50%)
- 120+ machine sizes, 6 Types:
  - General Purpose (balanced CPU to memory): B, D, A, DS, DC
  - Compute optimised (high CPU): F, FS
  - Memory optimised (high memory)L E, ES, M, G, GS, D*, DS*
  - Storage optimised (high disk throughput / IO): LS
  - GPU: NV, NC, ND
  - High Performance (massive power): H
- Creating machine
  - Select a subscription and then a resource group
  - Unique name within a resource group
  - Region impact machine choice and availability zone
  - Create a username and password for administrator (Linux can post SSH public key)
  - No inbound rules by default (RDP:3389 and SSH:22 should be limited when you grant access)
  - Autoshutdown built in to turn off (optional)
  - Cloud Init is equivalent to bootstrap
- Set of resources created
  - Auto shutdown
  - Virtual machine
  - Disk (spec depends on VM class: Standard HDD, SSD, Ultra SSD)
  - Network interface card
  - Public IP (optional, can attach a DNS name to it .[region].cloudapp.azure.com)
  - Network security group
- To Delete
  - Either delete each resource
  - Delete as a resource group
- ASSIGNMENT: Create a Ubuntu Linix VM, connect to it via SSH, tear down
- Availability
  - Single VM is 99.9%
  - Set: VM is part of a group of machines, identify at creation
    - Fault domains: different physical sets of hardware (e.g. power, network)
    - Update domains: how Microsoft rolls out updates to the Azure infrastructure
    - Up to 99.95%
  - Zones: same as in AWS
    - Specify which AZ you want machine deployed into
    - Not available in all regions
    - Up to 99.99%
    - Also need to have a load balancer
