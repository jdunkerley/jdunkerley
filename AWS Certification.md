# My Experience from AWS Certifications  

![SA](https://jdunkerley79.files.wordpress.com/2019/03/solution-architect.png)
![Developer](https://jdunkerley79.files.wordpress.com/2019/03/developer.png)

So I have now completed both the [Solution Architect Associate](https://aws.amazon.com/certification/certified-solutions-architect-associate/) and [Developer Associate](https://aws.amazon.com/certification/certified-developer-associate/) certifications and thought I would share my thoughts from the process.

## Why Bother?

For a long time, I haven't been particularly bothered by doing any certifications. I have always believed in proving myself in the work I do, and this was particularly true when I worked as an in-house developer. As a consultant for [Scott Logic](https://www.scottlogic.com/), I naturally have to have a more public profile - certification is one way of saying I know a technology area.

Alteryx introduced certification a couple of years ago and that was my first exposure to actually doing one. It was a requirement of being an [ACE](https://community.alteryx.com/t5/ACE-Program/bd-p/ace-program) that you at least the [Core certification](https://community.alteryx.com/t5/Certification/bd-p/product-certification). In this case, I have spent far too much time digging deep into Alteryx so didn't do much study for them. The Expert certification really made me value the process. It is a very hard test and really demonstrates a great breadth of detailed knowledge in the platform.

In AWS case, the SA associate gives a fantastic introduction to their services. AWS is a huge platform and grows at an astronomical rate - take a look at the [2018 What's New](https://aws.amazon.com/about-aws/whats-new/2018/) to give you an idea. The SA associate exposes you to a wide range of services giving you a brief introduction to many of them. In addition, it goes much deeper into some import topic areas such as EC2 and IAM. This helps hugely when working on the platform as you have a good idea where to start looking.

Finally, the cloud is a new world and we should think in a new way. If all we do is lift and shift from data centre to the cloud, then the benefits are a lot smaller. The SA gives a lot of advice and examples of how to build things in a cloud way. Changing the way, you think about using the platform is another huge value from passing the certifications.

## So How Did I Approach It

There are hundreds of blogs or YouTube videos about how I study and passed AWS certification in 2 weeks. I don't agree with this approach. My principal goal was to turn my experience of working in AWS (which was quite limited) into a skill that would allow me to work quickly and effectively in the platform. Because of this, I wanted to take my time going over the topics and experimenting and building with each area that I looked at.

A group of us working on the same project decided we all wanted to work together to learn about AWS. We met once a week over breakfast in [SmoothBean](https://www.tripadvisor.co.uk/Restaurant_Review-g504167-d7171237-Reviews-Smoothbean-Croydon_Greater_London_England.html) in Croydon to see how we were all getting on, chat and ask each other questions (and eat nice breakfast and drink coffee!).

![Study Group](https://jdunkerley79.files.wordpress.com/2019/03/studygroup.jpg)

At first, we just wanted to learn about AWS in general but when we investigated it felt like the SA associate was a good way. Next, we looked into finding some material to use to help study. We looked into a few options before settling on the [A Cloud Guru](https://acloud.guru/) [course](https://acloud.guru/learn/aws-certified-solutions-architect-associate). You can also get the course on [Udemy](https://www.udemy.com/aws-certified-solutions-architect-associate/). 

In general, I aimed to watch about 2 hours of videos a week and then spend time playing with and experimenting with the technologies we had been looking at. The table below shows roughly what I looked at each week:

1. Look at 10,000-foot overview and the certifications path
2. IAM and AWS CLI 
3. S3 101 - get familiar with S3, including life cycle management
4. Work on a DropBox style 's3Box' web app
5. S3 Additional Services - Cloudfront, Storage 
6. EC2 and EBS
7. Load Balancers, Cloud Watch and IAM Roles
8. Bootstrapping, Metadata, Autoscaling and Placement Groups
9. Lambda and Create a simple serverless website
10. DNS and Databases
11. Creating a VPC
12. Application Services (SNS (+SES), SQS, SWF, Kinesis and Media Services)
13. Well Architected Framework

After getting to the end of the material, the next step was to prepare for the exam. ACG provides some questions in the material including a mock test at the end. After doing these, I looked for some decent mock papers we could use to prepare for the test itself. [WhizLabs](https://www.whizlabs.com/) have a set of [mock papers](https://www.whizlabs.com/aws-solutions-architect-associate/practice-tests/). We spent about 2 weeks looking at these papers. The details in the answers they provide are really good.

Our small group picked a date to sit the test together. We did this as we were getting towards the end of the VPC date. This helped us encourage each other and work together through the mocks. It was nice to meet up together prior to taking the exam and then to discuss how we found it after taking it. Our group all did very well following this approach on the SA exam. The video material covered the syllabus well, and the practice papers lined up well with the format and style of questions we faced in our exams.

## Developer Certification

After completing the SA associate, we took a little break and a breather to decide what we wanted to do next. Some moved on to look at other cloud providers but a group of us chose to repeat the process and go for the developer exam. Historically, these two exams have hugely overlapped. Since the revamp of the exams in 2018, this has been less true. That being said there is still a large overlap. We followed the same approach starting with the [ACG course](https://acloud.guru/learn/aws-certified-developer-associate-june-2018) on the material. The list below shows the schedule I followed:

1. Revisit IAM / EC2 / S3 (5:26:18 - but content all covered in SA)
2. Look at Serverless (2:26:45)
3. Build something in Serverless (2-week project)
4. DynamoDB (1:36:02)
5. KMS and Other AWS Services (1:37:54)
6. Developer Theory (2:50:50)
7. Advanced IAM, Monitoring (1:15:42)

Again, after we completed the videos, we chose a date and then set about doing the [WhizLabs practice papers](https://www.whizlabs.com/aws-developer-associate/practice-tests/). I will say that the questions involve more detail than was covered in the videos. I think acing this test without actively using the platform and specific services is a lot harder. Again all of our group did pass the test well.

While they are mostly to help me remember my notes from studying the course are on [GitHub](https://github.com/jdunkerley/jdunkerley/blob/master/AWS%20Developer.md).

## Alteryx Certification

Just for completeness, I thought I would mention the Alteryx certifications. I spend a fair amount of my spare time exploring and playing with the capabilities of Alteryx. I sat both the Core and Advanced certification as soon as could with limited preparation (I did glance over the syllabus but was generally happy). Core involves having some exposure to the platform and can easily be studied from the material provided on the [community website](https://s3-us-west-1.amazonaws.com/ayx.policies/Core+Certification+Exam+Prep+Guide.pdf). Advanced likewise has a decent amount of [material](https://s3-us-west-1.amazonaws.com/ayx.policies/Advanced+Certification+Exam+Prep+Guide.pdf) to help you prepare.

Expert is another challenge altogether. It is a very tricky practical exam and can only be taken at an Inspire conference (there are now three of these allowing you three chances a year if you feel like jet setting!). The first thing to do is to read carefully the areas they are going to challenge you on (you receive details in advance of Inspire telling you the topics). I would then recommend doing the [weekly challenges](https://community.alteryx.com/t5/Weekly-Challenge/bd-p/weeklychallenge) and answering questions on the community to prepare. Finally, watch the live training on these topics and make sure you are happy with it. Finally, remember the format of the test. You only need to answer 3 questions correctly out of 5 available (you can answer up to 4).

One last thing on Alteryx tests - they are open book. AWS ones were sat in an exam centre with no internet access (ironic for a cloud exam!). The Alteryx ones (including expert) allow you to search the web. So, my advice for expert is do the ones you are happy with straight away and get them done. Then you will hopefully have time to search the community or google for something similar to the question asked and then you can adapt to solve the problem at hand (it still won't be easy!).

## What's Next

Those of us who have completed the developer certification are having a break for a bit. We want to consolidate our knowledge and use it to build exciting and challenging projects (watch this space...) but then I am sure we will return to do some of the professional or specialist challenges. 
