# Using Enso to Analyze Fund Performance

In this post, I'll demonstrate how to use Enso to analyze the performance of a hypothetical investment account.  Starting from a daily valuation and transaction history, we'll create a return series, compare against a benchmark, and finally compare against inflation data to see the real return of the account.

## The Input Data

For this analysis, we have two input data files:

- Daily Valuations: A CSV file (`FundValues.csv`) containing the daily valuation of the investment account.
- Transactions: An Excel workbook (`CashTransactions.xlsx`) containing the transaction history of the account, including deposits and withdrawals.

In the hypothetical example, there are monthly investments of £1,000 made on the first business day of each month. There are annual fees of 0.5% charged on the total account value, 14th of each month. The account starts with an initial investment of £20,000.

Let's load the data into Enso - using `Data.read` (or by dropping the files into the Enso workspace).

![Initial Raw Data](image.png)

## Computing Daily Returns

To compute the daily returns of the fund, we need to account for both the changes in valuation and the cash flows (deposits and withdrawals). Firstly, joining the two datasets on the date field allows us to see both the valuation and any transactions on the same date. As there may be dates with no transactions, we use a left join to ensure all valuation dates are retained. Then, we sort the data by date to ensure chronological order.

![Joined Data](image-1.png)

Next, we compute the return. This is equal to the change in valuation adjusted for any cash flows, divided by the previous day's valuation. As the data is in chronological order, we can use the `offset` function in an Enso expression to get the previous day's valuation. The formula for daily return is:

```
Return = ([AccountValue] - coalesce([CashValue],0)) / coalesce(offset([AccountValue], -1), 0) - 1
```

Where `coalesce([CashValue],0)` ensures that if there is no cash transaction on that day, it is treated as zero. The offset function will return a `Nothing` value if there is no previous day (i.e., the first row), so we also use `coalesce` to treat that as zero.

![Return series](image-2.png)

Note, Enso warns that the first row includes a division by zero, as there is no previous day valuation. This is expected, and we can filter out this row in the next step. If you choose to `Ignore` in the `on_problems` dropdown, the warning will disappear.

Finally on the data prep, remove the first row (using `drop (First 1)`) and the extra columns we no longer need, leaving just the date and return columns (using `select_columns ["Date", "Return"]`).

## Computing Monthly Returns

To compare the fund's performance against a benchmark, we need to compute the monthly returns from the daily return series. We can do this by grouping the data by month and calculating the cumulative return for each month.

Adding a `Month` column to the data, equal to the first day of the month for each date, allows us to group by this new column. There are a couple of ways to produce this, but the simplest is to use the `first_of_month` function.

As Enso doesn't have a built-in aggregate product function, will need to use logarithms to compute the cumulative return. The formula for cumulative return using logarithms is just the sum of the log(1 + daily return) for each day in the month, and then converting back to a return using the exponential function:

