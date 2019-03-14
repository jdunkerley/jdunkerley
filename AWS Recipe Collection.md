# So I have far too many recipe cards...

![Too Many Recipe Cards](assets/recipe-cards/recipe-cards.jpg)

Over the years, I have collected (read hoarded) hundreds of recipe cards from trips to the supermarket. I've generally kept them in folders so I can find them when I want but I want to do better. I'd love to be able search for them, find out commonly paired ingredients, work out cooking times.

Over the last few months, I have been working with serverless technologies in AWS both for hosting a secure HTML5 front end application and for ingesting and performing natural language processing on documents with the output being stored on Elasticsearch running on EC2 instances. I thought I could probably use some of these ideas make a way to hold all my recipe cards and have some fun along the way pushing different AWS technologies.

Additionally, I'll want to use AWS technologies to secure the platform, take images in and OCR the content. 

## So Why Serverless...

So I've covered this before for [Azure Functions](https://jdunkerley.co.uk/2018/02/06/creating-a-simple-azure-function-in-javascript-with-vs-code/) and while have done a post [AWS Lambda](https://jdunkerley.co.uk/2019/01/08/creating-a-simple-aws-lambda-in-c/) haven't talked about Lambda's strengths recently. As mentioned in the Azure post, they have lots of benefits.