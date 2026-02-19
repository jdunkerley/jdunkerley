# Using Enso to Analyse Fund Performance (part 2)

In the last post, we looked at taking daily valuations and transactions for a hypothetical investment account and computing daily returns and an index series. In this post, we'll take that analysis further by computing some drawdown statistics and comparing the fund's performance against a benchmark.

We'll start this analysis using the index and return series we created in the last post. If you wish to follow along this post, you can download the data from [GitHub](https://raw.githubusercontent.com/jdunkerley/jdunkerley/refs/heads/master/enso-fund-performance-2/index.csv) or recreate it using the steps in the previous post.

![Index Series](image.png)

## Maximum Drawdown

The maximum drawdown is a measure of the largest percentage drop from a peak to a trough in the value of an investment. More formally, if <em>V<sub>t</sub></em> is the value of the investment at time <em>t</em>, then the drawdown can be expressed as:

![d_t = 1-\frac{v_t}{Max^{0}_{t}(v_i)}](latex_drawdown.png)

As <em>V<sub>t</sub></em> will be between 0 and the maximum value, the drawdown will be between 0 and 1, where 0 indicates no drawdown and 1 indicates a complete loss of value. To compute this in Enso, we can use the `running` function to keep track of the maximum value seen so far, and then apply a formula to compute the drawdown at each point in time.

![Drawdown series](image-1.png)

To make the values a little clearer, I choose to format the drawdown as a percentage with one decimal place, which is just a matter of multiplying by 100 and rounding the result. The expression for this percentage drawdown is `Round((1-[Value]/[Max Value])*100,1)`.

Finally to compute the maximum drawdown, we can use an aggregate function to find the maximum value in the drawdown series. This gives us the maximum drawdown for the entire period.

![Maximum drawdown](image-2.png)

This means that at some point during the period, the fund experienced a drawdown of 12.8%, which is the largest drop from a peak to a trough in the value of the investment within this dataset.

## Maximum Drawdown Duration

The maximum drawdown duration is a measure of the longest period of time when an investment is in a drawdown state, meaning it has not yet recovered to its previous peak value. We can compute this by first creating a column that counts the number of consecutive days that the investment has been in a drawdown state.

You could choose to count this in terms of calendar days or trading days. In this case, I have chosen to count in calendar days. Adding a `Days Since Previous` column that computes the number of days since the previous row will allows us to compute the duration. The expression: `coalesce(date_diff(offset([Date],-1),[Date]),0)` computes the difference in days between the current date and the previous date, treating the first row as zero.

We can then create a `Days In Drawdown` column that uses a conditional expression to check if the current value is in a drawdown state (i.e., if the `Drawdown` is not equal to `0`). By default, this will warn about floating point comparisons, but we can choose to `..Ignore` this warning in the `on_problems` dropdown. We need to then convert this column into a running sum, again using the `running` function:

![Days in Drawdown](image-3.png)

Note: we need to choose to `..Update` for `set_mode` as I am choosing to replace the existing series rather than creating a new one. If you don't do this, Enso will error telling you that the column already exists.

Next, we need to reset the count of days when the value recovers to a new peak. If we use an expression of `if [Drawdown]=0 then [Days In Drawdown] else nothing` then we get a series where the value if `Nothing` when the value is in drawdown, and the cumulative number of days in drawdown when it is not. Combining this with a `fill_nothing` function picking up the previous value allows us to create a series that is the `Days In Drawdown` at the last point of recovery. Finally, removing this from the `Days In Drawdown` series will give us the number of days in drawdown at each point in time:

![Resetting Days in Drawdown](image-4.png)

Again, we can then compute the maximum value in this series to get the maximum drawdown duration:

![Maximum drawdown duration](image-5.png)
 
 ## Joining to a Benchmark

To compare the fund's performance against a benchmark, we can download historical price data for a relevant index. In this case, I chose to use the FTSE 100 index as a benchmark for UK equity performance. We can download the historical prices for the FTSE 100 from [this link](https://uk.investing.com/indices/uk-100-historical-data) (due to the rights of the data, I can't share it directly, but you can download it yourself from the link).

![FTSE 100 Raw Data](image-6.png)

The downloaded CSV is a set of formatted values, so first we need to clean the data and parse to value. First, using the `use_first_row_as_names` function will name the columns. Then a couple of `parse` functions will convert the columns to the correct type.

![Parsing the FTSE 100 data](image-7.png)

Let's again convert the price series into an index series. We can do this using the expression `[Price]/(first([Price]))` to create an index that starts at 1 on the first day (once we have sorted the data by date), and then grows according to the price changes.

We can then join this back to the original index series for the fund, using a `merge` function to add the benchmark index. Followed by a `fill_nothing` to carry forward the last value of the benchmark index when there are missing values (e.g., due to non-trading days). Finally, we can compute the benchmark return using a formula of `coalesce([BenchmarkIndex]/offset([BenchmarkIndex],-1)-1,0)`.

![Merged with the fund data](image-8.png)

We can now compute the correlation between the fund returns and the benchmark returns. The Pearson correlation coefficient measures the linear relationship between two variables, in this case, the fund returns and the benchmark returns. A correlation of 1 indicates a perfect positive linear relationship, -1 indicates a perfect negative linear relationship, and 0 indicates no linear relationship. To compute this in Enso, first we get the `Return` column form the table and then the `Benchmark Return` column. We can then use the `compute_bulk` function to create a table with the correlation between these two series.

![Series correlation](image-9.png)

Finally, we can make a scatter plot of the fund returns against the benchmark returns to visually inspect the relationship between the two. This can be done by selecting the three columns (`Date`, `Value` and `BenchmarkIndex`) and then choosing the Scatter Plot option in the visualisation menu.

![Scatter plot of fund versus index](image-10.png)

## Computing Excess Returns and Sharpe Ratio

**ToDo: Add Risk Free Rate and Sharpe Ratio section here**

Now that we have joined the benchmark to the fund data, it is simple to compute the excess return of the fund over the benchmark by subtracting the benchmark return from the fund return. We can then compute a daily sharpe ratio by dividing the excess return by the standard deviation of the excess return. This can then be annualised by multiplying by the square root of 252 (the number of trading days in a year).

Finally, we can compute the correlation between the fund returns and the benchmark returns using the `correlation` function. This will give us a measure of how closely the fund's performance tracks that of the benchmark.

## Wrapping Up

In this post, we have looked at how to take the index and return series we had created and compute some drawdown statistics, as well as how to join the fund data to a benchmark and compute the correlation. 

Finally, we also looked at how to compute excess returns over a risk free rate and compute a Sharpe ratio to measure the risk-adjusted performance of the fund.

If you'd like to try this yourself, you can download a trial of Enso from the [Enso website](https://www.ensoanalytics.com/). The data files used in this project are available from my GitHub repository:

- [index.csv](https://raw.githubusercontent.com/jdunkerley/jdunkerley/refs/heads/master/enso-fund-performance-2/index.csv)

The completed Enso project file is also available on GitHub:

**ToDo: Add link to completed Enso project file here**
- [Enso Project](https://github.com/jdunkerley/jdunkerley/blob/master/enso-fund-performance-2/Fund%20Performance%20Blog.enso-project)
