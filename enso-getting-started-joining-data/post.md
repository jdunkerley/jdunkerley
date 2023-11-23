## Getting Started With Enso - JSON and Joins

In this post, we will use the results from the [last post](https://jdunkerley.co.uk/2023/11/10/getting-started-with-enso-parsing-selecting-and-aggregating/) and look at how each category has done against some sales targets the company has set.

The completed workflow from that post can be downloaded from [GitHub]().

The sales targets are stored in a JSON file, so we will need to parse that and then join it to the results from the last post. The raw JSON file can be download from [GitHub](https://raw.githubusercontent.com/jdunkerley/jdunkerley/master/enso-getting-started-joining-data/sales_targets.json).


## Downloading and Parsing the Sales Targets

Let's take a look at the JSON file:

```json
{
    "Furniture": { 
        "2015": [5000,2000,19000,10000,8000,11000,11000,10000,27000,10000,26000,23000],
        "2016": [5000,2000,20000,10000,8000,12000,12000,10000,29000,10000,27000,24000],
        "2017": [5000,2000,22000,11000,9000,13000,13000,11000,31000,11000,29000,25000]},
    "Office Supplies": { 
        "2015": [5000,2000,19000,9000,8000,11000,11000,9000,27000,9000,25000,22000],"2016": [5000,2000,20000,10000,8000,11000,11000,10000,28000,10000,26000,23000],
        "2017": [5000,2000,21000,10000,9000,12000,12000,10000,30000,10000,28000,24000]},
    "Technology": { "2015": [5000,2000,19000,10000,8000,11000,11000,10000,27000,10000,26000,23000],"2016": [5000,2000,20000,10000,8000,12000,12000,10000,29000,10000,27000,24000],"2017": [5000,2000,22000,11000,9000,13000,13000,11000,31000,11000,29000,25000]}
}
```

Each category is listed as a key in the JSON object. The value for each category is another JSON object with the years as keys and the sales targets as arrays. The first task is to load this into Enso. There are various ways to read data from a URL in Enso, 
