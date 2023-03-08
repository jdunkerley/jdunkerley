# Using Enso to Solve Preppin Data Challenges

Enso is a functional programming language that lets you quickly and simply load, blend, and analyze your data. We've been building out the core capabilities of the product and are rapidly working on the IDE and cloud release to give a straightforward experience for users using it.

To show some of the new capabilities, I have tackled the first three challenges of 2023 posted on [Preppin Data](https://preppindata.blogspot.com/). These data challenges are posted by [Carl Allchin](https://twitter.com/datajedininja), [Jonathan Allenby](https://twitter.com/jonathanallenby), [Jenny Martin](https://twitter.com/jennymartinds14), and [Tom Prowse](https://twitter.com/TomProwse1). They are solvable in many data tools and make an excellent set of tasks to see how to use Enso.

This blog was written using a recent nightly build; many features and functions are still maturing and subject to change as we approach our release. In addition, we are still working on adding more "widgets" to the nodes and improving data visualization capabilities to help guide you through building the workflow. These will appear over the next month or two in the nightly builds.

## Week 1 - The Data Source Bank

- https://preppindata.blogspot.com/2023/01/2023-week-1-data-source-bank.html

![Parsing the Date](./parse_date.png)

Loading a dataset into Enso is as simple as dragging the file onto the IDE, and it will then set up the first node and attempt to parse the data. However, Enso's default CSV parsing only recognizes dates in ISO format (`yyyy-MM-dd`). So to parse the 'Transaction Date' column, we need to select it (using the `get` method) and then parse it (using the `parse` method), and finally replace the original column in the table (with the `set` method).

For this challenge, we need to derive three values - the day of the week, the bank code, and whether a transaction was in-person or online. For the first two, I used the same process - select the column, apply a function over each row, and then add the result back to the table. 

![Format the Date](./date_format.png)

Creating the "day of week" column involves formatting the date. The `format` function on a `Date` allows for this. Enso uses Java for date and times so the [usual date format specifiers](https://docs.oracle.com/javase/8/docs/api/java/time/format/DateTimeFormatter.html) work - so in this case the expression is `_.format "EEEE"` (the `_` is a shorthand to create a lambda function). To build this within the IDE, I took a single value from the column (using `.first`), and then the component browser showed the available functions for a `Date`. If you then detach the incoming node, the new node becomes a reusable function I can feed into the `map` function on the column.

![Day of Week computation](./day_of_week.png)

For the "bank" column, we need to split the "Transaction Code" string and take the first part. The process was the same - pick the input column, get a single value, and create a mapping function. In this case, the mapping used the `split` and `first` functions.

![Get bank](./make_bank.png)

Having built these nodes, Enso allows me to make a reusable function by selecting them and pressing `Ctrl-G`. The result can then be fed into the same `map` as above.

![Make bank column](./make_bank_column.png)

The final column was created using Enso expressions, an Excel formula-like syntax allowing a shorthand to derive a new column. You can reference existing columns (specified by name in square brackets) and use all the functionality defined on a column. In this case, `'IF [Online or In-Person]==2 then "In-Person" else "Online"'` will decode the column into the text values.

![Aggregations](./aggregations.png)

Finally, the last task is aggregating the results to produce the three new tables. The `aggregate` function allows us to do this. It takes a vector of `Aggregate_Column` to create the summarised tables. These columns are either group bys or aggregate calculations. For example, `operator8.aggregate [Group_By "Bank", Group_By "Day of Week", Group_By "Type of Transaction", Sum "Value" "Value"]`.

The final workflow is shown below:

![Completed Week 1](./week1_completed.png)

[Code](https://gist.github.com/jdunkerley/44d0e1f66fedad1223a3a08bd44c440d)

## Week 2 - International Bank Account Numbers

- https://preppindata.blogspot.com/2023/01/2023-week-2-international-bank-account.html

For the second challenge, we need to construct "IBAN" codes for a set of transactions. In this case, two input files are provided - one with SWIFT codes for banks and another with transaction data. There is one slight complication with bringing the data in. All the values are text in the "Swift Codes" file, so Enso doesn't automatically detect the headers. The `use_first_row_as_names" function renames the columns to the first value.

![Input and Join](./input_join.png)

Having read in the files, the next step is to join the two data sets. The `join` function allows you to specify the type of join (such as Inner, Left Outer, Full) and the columns to join on (defined as a Vector). If the two inputs have the same first column, it will, by default, automatically perform an inner join using this.

![Tidy Columns](./tidy_columns.png)

There is a small amount of preparation before creating the IBAN for each transaction. First, the transaction file has been automatically parsed, and the account numbers have been converted to integers. To use these values in the final expression, we need to convert them to text. Using the same process as for week one and using the `map` function with `.to_text` creates the required column. For the "Sort Code", we need to remove `-` from the values; a simple "replace" on each record covers this.



## Week 3 - Targets for DSB

- https://preppindata.blogspot.com/2023/01/2023-week-3-targets-for-dsb.html

