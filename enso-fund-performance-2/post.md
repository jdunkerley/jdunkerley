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
