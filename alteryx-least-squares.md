# Least Squares in Alteryx

A friend on the Alteryx community recently asked me about recreating the Excel trendline capabilities within Alteryx. 

![Excel Options](assets/least-squares/excel-options.png)

If you have the predictive tools installed then Alteryx has a [Linear Regression tool](https://help.alteryx.com/2018.2/lm.htm) which will fit a linear model to the set. This uses R and produces a model for the entire dataset. For my implementation, I wanted to allow for groups of data and go back to first principles and use the core Alteryx tools (i.e. no SDKs, R or Python) to build it.

## Least Squares

