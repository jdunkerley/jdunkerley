# Virtual Machines in the Clouds: AWS, Azure and GCP

As I have ended up doing some work in AWS, Azure and now GCP as well; I thought I would write a quick post comparing the experience of building a simple web hosting site on each of them. There are other cloud providers but the market share of these three is huge. I don't have experience of the Alibaba cloud (the other major player) so won't cover that here. The chart below shows the market share according to [Gartner](https://www.gartner.com/en/newsroom/press-releases/2019-07-29-gartner-says-worldwide-iaas-public-cloud-services-market-grew-31point3-percent-in-2018) in 2017 and 2018:

![Cloud Provider Revenues](assets/cloud-vms/revenue.png)

## Shared Concepts

So before we get practical lets talk about the common things (albeit with different names) that these cloud providers all have and highlight some of the differences between them.

### Cloud Computing

First, lets for the sake of argument define *Cloud Computing*. The [NIST definition](https://nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-145.pdf) is:

```
Cloud computing is a model for enabling ubiquitous, convenient, on-demand network access to a shared pool of configurable computing resources that can be rapidly provisioned and released with minimal management effort or service provider interaction.
```

They define a five characteristics they expect of a cloud provider:

- *Self-service*: users can provision resources as needed without human interaction needed from the provider
- *Broad Network Access*:  capabilities can be accessed on a variety of devices over the internet
- *Resource Pooling*: cloud resources are shared across a large number of users and reassigned on demand.
- *Rapid Elasticity*: the amount of resources can be scaled up or down with demand, to almost 'infinite' scale
- *Measured Service*: all resources utilisation is monitored in a way appropriate for the service. This is typically used for the pay per use model.

Likewise, they define different service models:

- *Infrastructure as a Serivce (IaaS)* is where virtual appliances, storage, compute and networks are provided so the user can stand up and configure to create an environment in the cloud. The users are responsible for the installation and configuration of all devices but not the underlying hardware.
- *Platform as a Service (PaaS)* is a complete development and deployment environment in the cloud. It still includes compute, storage and networking but now things like the Operating System and Database Software get managed by the provider. This reduces the operational load for the users and lets them concentrate on their 'value' proposition.
- *Software as a Service (SaaS)* is hosted applications running in the cloud. In this case, users don't provision machines 

You don't have to use just one model, you can mix and match. For example, run a collection of virual machines (IaaS) connecting to a managed database instance (PaaS). For this blog, I am only really looking at IaaS capabilities.

### Global Infrastructure

One of huge benefits of working in the cloud, is that it is global. If you were to stand up your own data centre (I'm British so that's how it's spelt!), it would probably be in your local country. Normally, you would pick a second location a bit away from the first to allow for disaster recovery - again likely in the same country (and often not as far away as you might like). All of the three cloud providers are truely massive, global operations.

All three of them have multiple *regions*. A region is an area within a country. They are separate geographic zones from each other. All the providers have a long list of regions, the map below shows where they are as of 20th November 2019.

<div class='tableauPlaceholder' id='viz1574283835347' style='position: relative'><noscript><a href='https:&#47;&#47;jdunkerley.co.uk'><img alt=' ' src='https:&#47;&#47;public.tableau.com&#47;static&#47;images&#47;Cl&#47;CloudRegions&#47;CloudRegions&#47;1_rss.png' style='border: none' /></a></noscript><object class='tableauViz'  style='display:none;'><param name='host_url' value='https%3A%2F%2Fpublic.tableau.com%2F' /> <param name='embed_code_version' value='3' /> <param name='path' value='views&#47;CloudRegions&#47;CloudRegions?:embed=y&amp;:display_count=y&amp;publish=yes' /> <param name='toolbar' value='yes' /><param name='static_image' value='https:&#47;&#47;public.tableau.com&#47;static&#47;images&#47;Cl&#47;CloudRegions&#47;CloudRegions&#47;1.png' /> <param name='animate_transition' value='yes' /><param name='display_static_image' value='yes' /><param name='display_spinner' value='yes' /><param name='display_overlay' value='yes' /><param name='display_count' value='yes' /><param name='filter' value='publish=yes' /></object></div>                <script type='text/javascript'>                    var divElement = document.getElementById('viz1574283835347');                    var vizElement = divElement.getElementsByTagName('object')[0];                    if ( divElement.offsetWidth > 800 ) { vizElement.style.width='1000px';vizElement.style.height='827px';} else if ( divElement.offsetWidth > 500 ) { vizElement.style.width='1000px';vizElement.style.height='827px';} else { vizElement.style.width='100%';vizElement.style.height='727px';}                     var scriptElement = document.createElement('script');                    scriptElement.src = 'https://public.tableau.com/javascripts/api/viz_v1.js';                    vizElement.parentNode.insertBefore(scriptElement, vizElement);                </script>

Google and Amazon both use nice easy naming conventions, which helps groups the regions (e.g. *us-east-1*). Microsoft does a little more shuffling with its naming (e.g. *ukwest* versus *westeurope*), but often specifies the country in the region name which is helpful. It is worth stating that not all regions are equal - some will have capabilities that others don't, some will be larger, some smaller. For example in AWS, often the new features are released in *us-east-1* first and then rolled out to other regions afterwards. In all cases, a region is in a single legal jurisdiction which can be critical for data sovereignty and regulations.

Moving out from regions, Azure groups regions into *[geographies](https://azure.microsoft.com/en-gb/global-infrastructure/geographies/)*. These are collections of regions which share the same legal jurisdictions. For the other two providers, there are certain blocks of regions (the *us-* and *europe-* or *eu-* for example) which share similar jurisdictions. As always when handling data, you will need to ensure that you host it in the correct region.

Within a region, all three providers define *availability zones*. Each availability zone is one or more data centres (though it can often be easiest to think of them as a single data centre) with independent power, connectivity and resources. The zones are physically far enough apart to reduce risk of environmental factors knocking out more than one, but they are close enough together to ensure low latency when communicating between zones within the region. 

Finally, all of the providers have lots of additional *edge locations* (also called points of presence). These are connection points onto the providers own networks and serve as <abbr title="Content Delivery Network">CDN</abbr> locations for the respective services. They work as the point for end users to connect onto the cloud locally and allow you to reduce latency to your application.

### Networkings

These locations and all the availability zones (and hence the regions) are interconnected to each other using the providers dedicated global networks. They all spend huge amount of time and resource designing and building these networks. AWS had a [great session at re:Invent 2016](https://www.youtube.com/watch?v=uj7Ting6Ckk) talking all about this.

Within the providers space, often the first thing you will want to define is a *Virtual Private Cloud (VPC)*. This is your space in the cloud. It a logically isolated section in the cloud where you can provision and deploy resources. Both AWS and GCP use the VPC terminology, Azure calls the concept a Virtual Network (VNet). In AWS and Azure, these are region specific. In GCP's case a VPC can span multiple regions as needed. To achieve this in Azure and AWS you would need to *peer* the VPCs with one in each region. Peering is beyond the scope of this post as it is quite an advanced topic.

VPNs and Direct Connect, Subnet Firewall, Routing

### Virtual Machines and Storage

VMs
HDD/SSD
Object Storage

### Delivery Network and HTTPS

Load Balancer

### APIs, SDKs and CLIs

One of the defining features of a cloud provider is the self-service ability to stand up and configure new resources. All three providers offer a web based UI, called the *console*. These give you the ability to either examine or deploy anything from your web browser. Generally, I wouldn't recommend deploying this way except for experimentation as it is better to script and automate provisioning as early as possible. 

-- Portal Image ??

Additionally, they all have mobile app offering versions of these consoles. This can be a very useul extension for alerting and monitoring your cloud presence whenever needed.

While I would probably suggest doing the rest of this post in [Terraform](https://www.terraform.io/) for the sake of comparison, I will do all of it using the CLIs each provider has.

## High Level Design

So for my highly exciting and very advanced website, my plan is to create two virtual machines in different availability zones within one region. 