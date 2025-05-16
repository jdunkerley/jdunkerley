# Looking at the 2025 rainfall data for the UK in Enso

It's been a little while since I last posted, as the team and I at Enso have been hard at work on the new version of the app. We released [version 2025.1](https://ensoanalytics.com/trial) a couple of weeks ago with a huge amount of [new features and improvements](https://community.ensoanalytics.com/c/what-is-new-in-enso/enso-analytics-2025-1-1-has-been-released).

![Sunny London](./london-sunny.jpg)

As any who live in the UK will know, it has been a (very dry spring)[https://www.bbc.co.uk/weather/articles/c5y64p3m7pyo] so far. As I write this, it is another beautiful sunny day in London, and the forecast is for more of the same. I thought I would use Enso to take a look at the rainfall data for the UK. I will be using the [UK Met Office](https://www.metoffice.gov.uk/) data, which is available for free on their [Climate Data](https://www.metoffice.gov.uk/research/climate/maps-and-data/data/historic-station-data) page. The specific data I will be looking at is the [UK monthly rainfall data](https://www.metoffice.gov.uk/research/climate/maps-and-data/uk-and-regional-series) starting from 1836.

## Fetching and Parsing the data

![Sample data](./data-sample.jpg)

Looking at the data, it is clear it is in a fixed width format. The first 5 lines are the header, and the rest of the data is in a fixed width format. Lets bring this data into Enso and see what we can do to parse it. *Currently, Fixed Width native support is only in the nightly  build of Enso, so I will show how to do it using the current release version. The nightlies are available from our GitHub repository [here](https://github.com/enso-org/enso/releases).* 

![Read data](./data-read.jpg)

The `Data.read` command is used to read data into Enso. It can read from files or from URLs. In this case, it downloads the data from the Met Office website and reads it into Enso. The data is identified as plain text, so it is read into Enso as `Text`. We can however, tell it to read it as a delimited file and then skip the header rows.

![Delimited configuration](./delimited-format.jpg)

To do this, choose the `Delimited` option in the format command. This will allow us to specify the delimiter, which in this case choosing `{none}` as it is not delimited. We can also specify the number of header rows to skip, which here is 5. The data is then read into Enso as a single column Table. As we are only interested in the monthly data, use a `set` component to take just the first 88 (4 for the year, 7 for each month) characters of each value (using `Text Left`).

![Delimited data](./delimited-data.jpg)

Now, we want to split into columns. I'm going to use regular expressions here as that is my favourite way of doing such things. The `tokenize_to_columns` function takes a column and a regular expression and splits the column into multiple columns. If there is a marked group in the regular expression, it will be used the value and the other parts discarded. The expression I am using is `\s{0,7}(\S+)`. This will match any whitespace (up to 7 spaces) followed by a non-whitespace sequence (the actual data we want).

I then do a couple more transformations to the data:

1. Use the first row as the column names.
2. Replace empty values with `Nothing`.
3. Parse all the values.

![Parsed data](./parsed-data.jpg)

## Comparing year by year

Now we have the data in a nice format, we can start to do some analysis. I want to change the data to a row based format with `year`, `month` and `rainfall` columns. A transpose function will rotate the data and then we can convert the month name to a season as well. I created a lookup table (using `Table.input`) to convert the month names to number and seasons. I then use a `merge` function to join the data with the lookup table. This gives us a table with the year, month, season and rainfall values.

![Structured data](./structured-data.jpg)

As December is in the winter, need to add a `season year` column to the data. This is just the year, unless it is December, in which case it is the next year. Using a `set` component with an [expression](https://help.enso.org/docs/using-enso/expression-syntax) of `if [month]="dec" then [year]+1 else [year]` to make the new column.

![Final Tidy](./final-tidy.jpg)

Finally, in preparing the data, need to add a row for May 2025. As this is not currently in the monthly data. Based on the article, the average for Spring 2025 is 80mm, so for May this comes out as just 2.7mm. Using another look up table and merge function we can update this value and then filter out the future rows (where the rainfall is `Nothing`).

![By Year](./by-year.jpg)

To create the yearly data, filter down to the interesting months (here March, April and May). Using another look up table and linked to a `filter` where `month Is_In` allows you to do this easily. Then aggregate by `season year` creating the total rainfall for each year.