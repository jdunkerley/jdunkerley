## Google Cloud

Cloud Compute (NIST defintion)
- On-demand self service resources
- Broad network access
- Resource pooling and elastic resources
- Measured service (PAYG or reserve)

Co-location of renting in shared facilites. Then moved towards to virtualisation, but still have to buy underlying machines setting it up and running. Google is a containerised architecture where resources are provisioned automatically.

Every company is a data company.

GCP meet you where you are from IaaS to PaaS to managed services
- IaaS: [Compute Engine](https://cloud.google.com/compute/)
- Hybrid: [Kubernetes Engine](https://cloud.google.com/kubernetes-engine/)
- PaaS: [App Engine](https://cloud.google.com/appengine/)
- Serverless: Functions
- Managed Services

(SaaS offerings out of scope)

Worldwide network:
- 100 points of presence (edge locations)
- Multi-Region (Europe) - Region (Europe-West2) - Zone (like a DC, low latency with region)
    - Can place some services in multi-region
    - 15 or so regions
- Projects to manage groups of resources (globally unique ID, non unique name)
    - Track resourcing and quotas
    - Enable billing and mangage permission and APIs
    - Collection of projects in a Folder (must be part of an Org Node)
    - Collection of foldes in an Org Node
      - Organisation Admin
      - Organisation Policy Admin (broad control of resources)
      - Project Creators (project level control)
      - Linked to GSuite if you have one or use Google Cloud Identity  
    
Environment goals:
- Carbon neutral
- Renewable energy

Open APIs in GCP
- uses Open Source APIs

Multi-layer security
- Designs and builds own data centres
  - Limited access
  - Multiple physical security controls
- Networking and Servers are custom designed
  - Custom security chip (*Titan*)
- Encryption at rest and in RPC calls (auto encrypted)
  - Hard drive encryption
- Second factors
- Google Front End
  - Built in DDoS attacks
- Machine Learning in log in and network use
- Red team tests

Security Collaboration
- Google owns infrastructure security
- You are responsible for securing data
- Principle least privilege

Access to GCP:
- Console (Web UI)
  - Has built in shell with SDK
- SDK (CLIs)
  - gcloud (main SDK)
  - gsutil (Cloud Storage)
  - bq (Big Query)
  - Available as docker image
- RESTful APIs
  - OAuth 2.0
  - JSON
  - Quotad and needs to be enabled
  - API Explorer in Console
- Client Libraries
  - Cloud Client libraries (community owned libraries that feel correct for language)
  - API Client libraries (full feature set but less aligned with languages)
- Cloud Console Mobile App

## IAM

- IAM Policies can be on Org / Folder / Project and flow downwards
      - Who: Google account, group, service account, GSuite, Cloud identity domain
      - Can do What: IAM Role (collection of permissions)
      - On What Resource

- 3 types of roles:
  1. Primitive: Broad apply to project and affect all resources
     - Owner (every thing including roles and permissions, set up billing)
     - Billing Administrator (just control billing side)
     - Editor (can change state)
     - Viewer (examine)
  1. Predefined Roles: apply to a particular service in a project
  1. Custom Roles: allow you to define a precise set of permissions

- Service Accounts
  - Have an email address to identify them
  - Use cryptographic material rather than password
  - These need to be managed

## Services

- Compute
  - Compute Engine (VMs)
  - Kubernetes Engine
  - App Engine
  - Cloud Functions
- Storage
  - Bigtable
  - Cloud Storage
  - Cloud SQL
  - Cloud Spanner
  - Cloud Datastore
- Big Data
  - Big Query
  - Pub/Sub
  - Data flow
  - Data proc
  - Data lab
- Machine Learning
  - Natural Language API
  - Vision API
  - Machine Learning
  - Speech API
  - Translate API

Cloud MarketPlace (formerly Launcher): pre-packaged solutions, estimate costs per month
 - base images get patched but once deployed yours to manage

 ## Compute Enginge
 - Virtual machines
  - Specify CPU, Memory, Storage size and type, GPU
  - Custom size possible as well
  - Flexibly resized
  - Designed to offer fast and consistent performance
  - Firewall attached to VM
- Storage
  - Persistent disk: standard or SSD
  - Local SSD disk (lost if terminated)
  - Snapshot available
  - High throughput available at no extra cost
- Pricing
  - Per second
  - Preemptible VM (Spot instances)
- Scaling
  - Huge single instance
  - Autoscaling for horizontal scaling
  - Cloud Load Balancing
    - Single global anycast IP
    - Point of presence to LB
    - Types:
      1. Global HTTP(S) (Level 7) - support URL level routing
      1. Global SSL Proxy (Level 4) - non-https ssl traffic (e.g. WSS), specific ports
      1. Global TCP Proxy (Level 4) - non-ssl traffic, specific ports
      1. Regional (TCP/UDP on any port)
      1. Regional internal (within VPC)
- Virtual Private Cloud Network
  - connect resources to each other
  - segment networks
  - connect to internet
  - Firewall:
    - Firewall rules
    - Can be on tags of resources
  - Routing tables: static routes
  - VPC are global (not regional)
  - Subnets are regional but scan span zones
  - Interproject:
    - Peering to connect networks
    - Shared VPC for full IAM control
  - Interconnect:
    - VPN tunnels (Cloud Router using Border Gateway Protocol)
    - Direct Peering (Put a router in one of google's point of presence) - No SLA
    - Carrier Peering (Use a carriers router in a PoP) - No SLA
    - Dedicated Interconnect (Private circuits for trafic to GCP at PoP) - has SLA if done to spec
- Cloud DNS
  - 8.8.8.8
  - Managed zones then add/edit/delete records
  - REST APIs
- Cloud CDN

## Storage

- On the VM in either persistent or local SSD
- Cloud Storage (S3 equivalent)
  - Object storage within Buckets
  - Unique key (URL)
  - Serving web content, DR, direct download
  - Immutable, new versions
  - Encrypted at REST and access via HTTPS
  - Buckets:
    - Globally unique name
    - Storage class and lifecycle rules
      - MultiRgional: 99.95% SLA GeoRedundant
      - Regional: 99.90% SLA Near compute engine
      - Nearline: 99.00% SLA (Access less than 1/mth) Backup, longtail content
      - Coldline: 99.00% SLA (Access less than 1/year) DR, Archive
    - Region or multi-region
    - IAM Policy and ACL
    - Version rules
  - Obejcts
    - Files in a flat namespace
    - ACLs
  - Transfer in:
    - gsutil (CLI), console
    - Storage Transfer Service (online large batch) - source HTTPS, another cloud or another region
    - Transfer Appliance (up to 1 PB - Beta, Snowball)
- Cloud Big Table
  - NoSQL, wide-column database service
  - Sparsely populated to store petabytes of data
  - HBase API
  - Support for streaming data in (Spark Streaming, Storm, Cloud Dataflow Streaming)
  - Batch Processing in MapReduce/DataFlow/Spark
- Cloud Datastore
  - NoSQL Highly Scalable
  - App Backend
  - Transaction support (unlike Big Table)
  - SQL like query engine
- Cloud SQL
  - RDBMS
  - MySQL, PostgreSQL, SQL Server (Beta)
  - Replicas: Read, Failover, external
  - Managed backups
  - Vertical (R/W) and Horizontal (RO) scaling
  - Security: Firewalls, encrypted at REST and in transit in GCP internal
- Cloud Spanner  
  - Horizontally scalable RDBMS
  - Strong consistency
  - SQL ANSI 2011
- Cloud Big Query
  - 