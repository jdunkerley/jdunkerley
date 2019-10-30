# Diabetes Log (Part 2) - Data...

So forever ago (well back in January), I wrote a post on creating a [.Net Core lambda](https://jdunkerley.co.uk/2019/01/08/creating-a-simple-aws-lambda-in-c/). The goal I want to produce is a tool to record and help me track everything I need for looking after me. 

## What Does Being a Diabetic Entail...

So this is my regime. Everybody who has Diabetes will have their own way of doing what they need - I am far from an expert but this is how I deal with it. I was diagnosed as a diabetic back in November 2004. I am a Type 1, hence this is what I know about. While there are similarities with Type 2, I wont talk about that as have no personal knowledge.

Diabetes mellitus type 1 is an auto immune disease where the immune system attacks the beta producing cells in the pancreas. This means that the body can only produce little or no insulin. Without insulin, the body cannot regulate the level of sugar in the blood. Incorrect sugar levels can lead to a whole range of compications both in the short term and longer term. In order to control this, you need to inject insuling into the body in some way.

The causes of Type 1 are complicated. It is not a simple genetic condition (though the presence of a family member with it increses the risk). It is not caused by exercise or diet but some environmental factors do play into it. In my case, I had an operation and nasty infection and that was the trigger event. 

Insulin control is far from trivial. If you have too little sugar in your blood, then you can have an incident of hypoglycemia (a hypo). This can lead to confusion and clumsiness, and in extreme cases loss of consciousness or even death. At the opposite end of the spectrum, if you have high levels of sugar in your blood, then you can cause significant organ damage over time. In extreme cases you can develop Ketoacidosis which can again end in a coma and time in hospital. So careful management is key.

![Equipment](assets/diabetes/equipment.jpg)

The way to monitor this is to check you blood sugar levels. The best measure of this is a full blood test called an HbA1C. This measures the level of the sugar in the blood over the last 2 to 3 months. I get this done about twice a year when I see a nurse or consultant. The goal is to have this less than 7%. For day to day monitoring you can check the instant level of sugar in your blood using glucose monitor. I use [AccuCheck Mobile](https://www.accu-chek.co.uk/blood-glucose-meters/mobile) - prick my finger and it gives me a reading of the instant level. For the instant measurement the goal is to be in the range of *4 - 8 mmol/l*. If you averaged *7 mmol/l* you would get an HbA1C of about 7%. Readings of less than *4 mmol/l* would indicate a hypo, higher that *8* would indicate too much sugar.

There are so many factors which affect the blood sugar level, it is crazy - some are obvious, some less so. Let's start with insulin. I have two types of insulin that I need to take - Levermir and Novo Rapid. [Levermir](https://en.wikipedia.org/wiki/Insulin_detemir) is a long-acting insulin designed to provide a background level in the blood to keep the body able to process some sugar. It takes a while to start acting and then lasts for about 18 hours so I need to take this twice a day. [Novo Rapid](https://en.wikipedia.org/wiki/Insulin_aspart) is a fast-acting insulin taken in responce to food. It starts acting within about 15 minutes and lasts around 3 hours. The real complication is working out how much insulin to take. This takes us on to the next factor food! The chart below shows a qualitative view of the insulin level in my body over a 24 hour period.

![Insulin Levels](assets/diabetes/insulin.jpg)

... Food and Carb counting

