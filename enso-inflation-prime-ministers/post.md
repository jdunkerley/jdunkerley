

One of Enso's strengths is its ability to easily gather and process data from various sources, particularly web-based APIs. In this post, using Enso, I'll show how to combine data from [FRED](https://fred.stlouisfed.org/) and [Wikipedia](https://www.wikipedia.org/) to create a dataset of UK Prime Ministers and the annualised inflation rate during their time in office.

## Setting up access to the FRED API

The FRED is a fantastic source of economic data. It provides a free API to access its data, which makes it easy to gather data using Enso. First, you'll need to sign up for a free API key from [FRED](https://fred.stlouisfed.org/docs/api/api_key.html).

Having created an account and obtained an API key, let's make a new Enso secret to store it securely. We can then use it within our projects. First, switch to the "Data Catalog" view in Enso and then click on the "Cloud" to access the Cloud drive.

![Accessing the Cloud Drive](access-cloud-drive.jpg)

Then click on the create new secret button in the toolbar.

![Create New Secret](create-new-secret.jpg)

This will pop up a dialog where you can enter your API key. Give it a name (e.g. `FredAPIKey`) and then paste in your API key. Then click "Create" to save the secret.

![Create Secret Dialog](create-secret-dialog.jpg)

Now we can use this secret in our Enso workflow. Let's create a new project and bring the inflation data for the United Kingdom into Enso. 

## Searching the FRED database

FRED has a search API at `https://api.stlouisfed.org/fred/series/search` which we can use to find the series ID for the UK inflation data. Let's start by building up a new component to make this request.

![Insert URI Component](component-browse-uri.jpg)

Click on the "Input" button in the toolbar to create a new input component. Type `uri` to search for the `URI.parse` component within the Component Browser. This will allow us to create a new URI that we can use to make our search request. Enter the above URL into the `uri` parameter input.

Next, the API needs some query parameters to specify the search term, the file type, and our API key. We can add these using the `add_query_argument` component on the URI. To do this, click on the output port of the `URI.parse` component and choose `add_query_argument` from the Component Browser. Each query argument takes a key (the name of the parameter) and a value, which can be either a Text value or point to an Enso secret. 

![Adding a Query Argument](add-query-argument.jpg)

We'll need to add three query arguments: `search_text` with a value of `Inflation UK`, `file_type` with a value of `json`, and `api_key` with a value pointing to our `FredAPIKey` secret (which you can choose using the secret browser). Add each one in turn by clicking on the output port of the previous `add_query_argument` component.

![URI with Query Arguments](with-query-arguments.jpg)

To then bring the data into Enso, add a `fetch` component to the last `add_query_argument` component, to make the HTTP request. Enso automatically detects the content type and parses the JSON response into an Enso data structure (technically a `JS_Object` in this case).

![Fetch Search Results](fetch-search-results.jpg)

In this case, the series are returned in the `seriess` field of the response. We can use a `get` component and then a `to_table` component to convert this into a `Table`, which we can then sort (by `popularity`) and filter to monthly series.

![Sort and Filter Series](sort-and-filter.jpg)

Looking through the results, we can see that the series ID for the UK inflation data is `GBRCPHPLA01IXOBM`, which corresponds to "Consumer Price Indices (CPIs, HICPs), COICOP 1999: Consumer Price Index: Overall Index Excluding Energy, Food, Alcohol and Tobacco for United Kingdom". We can now use this to get the actual inflation data.

## Getting and preparing the inflation data

Now we have the series ID, we can use the FRED series observations API at `https://api.stlouisfed.org/fred/series/observations` to get the actual inflation data. We can build this request in a similar way to the search request. This API needs the `series_id`, `file_type`, and `api_key` query parameters.

To get the series ID value into the request, you can right-click in the table cell and choose "Get Value" to create a component linked to the table with that value.

![Get Series ID Value](get-series-id-value.jpg)

Then, as before, build up the URI with the query arguments and a `fetch` component to make the request. The only extra difference is that the value for the series ID is now linked to the `get_value` component (drag from the output port to the value parameter).

*You can also copy and paste the search request components to save time, remember to change the URL and update the `search_text` query argument to `series_id` and link its value to the `get_value` component.*

![Inflation Data Request](inflation-data-request.jpg)

Converting this into a table of data is very straightforward. The observations are in the `observations` field of the response, so we can use a `get` component to extract this and then a `to_table` component to convert it into a table. To then tidy up the table, we can use a `parse` component to convert the values from text to numbers and dates, and then a `select_columns` component to keep only the `date` and `value` columns.

![Parse Inflation Data](parse-data-series.jpg)

## Getting the Prime Ministers data from Wikipedia

Now we have the inflation data, we need to get the list of UK Prime Ministers and their time in office. Wikipedia has a page listing all the Prime Ministers of the United Kingdom at [https://en.wikipedia.org/wiki/List_of_prime_ministers_of_the_United_Kingdom](https://en.wikipedia.org/wiki/List_of_prime_ministers_of_the_United_Kingdom). We can use Enso's web scraping capabilities to extract this data.

To do this, let's add a `Data.fetch` component to fetch the HTML content of the Wikipedia page. As this doesn't need any special query arguments, just enter the URL directly into the `uri` parameter. Wikipedia requires a user-agent header to be set, so we can add this by clicking the `+` button in the headers parameter and entering `User-Agent` as the key and something like `Enso/2025.2 (https://ensoanalytics.com)` as the value.

By default, Enso returns a `Response` object for HTML responses (allowing you to access headers, status code, etc.). To get the HTML content, we can either use a `decode_as_text` component or change the `format` parameter of the `Data.fetch` component to `Plain_Text`.

![Fetch Wikipedia Page](wikipedia-fetch.jpg)

Enso caches web requests while working within a project. If you change parameters sent to the server it will refresh, but if you change values within Enso that don't affect the request, it will use the cached version. To force a refresh, you can use the Refresh button in the top toolbar.

As anyone who has worked with me will know, I like using regular expressions to extract data from text. So lets extract all the tables from the HTML. A pattern of `<table.*?</table>` will find tables within the HTML. Using `tokenize` component will split the HTML into a list of tables, and then using `first` will select the target table.

![Extract Tables with Regex](first-table.jpg)

To now convert this HTML into a table, again will use a regular expression. The pattern `<tr>(?<HTML>.*?)</tr>` will match each row in the table, and then using a `parse_to_table` component will convert this into a table with a single column containing the HTML for each row. *The special `(?<HTML>...)` syntax is a named capture group, which allows us to name the column in the resulting table.*

![Parse Table Rows](parse-table-rows.jpg)

We can now drop the first two rows (which are headers) using a `drop` component and then 
