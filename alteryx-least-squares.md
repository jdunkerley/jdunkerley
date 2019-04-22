# Least Squares in Alteryx

A friend on the Alteryx community recently asked me about recreating the Excel trendline capabilities within Alteryx. 

![Excel Options](assets/least-squares/excel-options.png)

If you have the predictive tools installed then Alteryx has a [Linear Regression tool](https://help.alteryx.com/2018.2/lm.htm) which will fit a linear model to the set. This uses R and produces a model for the entire dataset. For my implementation, I wanted to allow for groups of data and go back to first principles and use the core Alteryx tools (i.e. no SDKs, R or Python) to build it.

Excel has 6 options for fitting a trendline to a dataset. With the exception of *Moving Average* these are all fitted using the least squares method. The five models are:

- *Exponential*: ![$y=Ae^{Bx}$](assets/least-squares/exp.svg)
- *Linear*: ![$y=A+Bx$](assets/least-squares/lin.svg)
- *Logarithmic*: ![$y=A+Blog_e(x)$](assets/least-squares/log.svg)
- *Power*: ![$y=Ax^B$](assets/least-squares/pow.svg)
- *Polynomial*: ![$y=A+Bx+Cx^2+...$](assets/least-squares/poly.svg) *depends on Order*

For my Alteryx macro, I plan to support Exponential, Linear, Logarithmic and Power (Moving Average isn't really the same and Polynomial will need more work). 

The Excel tool also allows you specify an intercept (the value when ![$(x=0)$](assets/least-squares/x_0.svg)) for Linear, Exponential and Polynomial. In other words fixing the value of *A* in each case. In the *Power* case, the intercept is always *0* and for the *Logarithmic* case it will be an error as the logarithm is not defined at 0. I want my macro to also support this.

Finally, you can get the trendline in Excel to output both the equation and the value of ![$r^2$](assets/least-squares/r2.svg). So finaly requirement is to do this as well

A quick shout out to the [LaTeX Previewer](http://www.tlhiv.org/ltxpreview/) by [Troy Henderson](http://www.tlhiv.org/) - I used it to create the SVG of all the LaTeX in this post. Onto the Maths...

## Least Squares Method for Regression

Lets imagine we have a dataset of points ![$(x_i, y_i)$](assets/least-squares/xy.svg). 

The goal of the least squares method is to fit a function ![$f(x)$](assets/least-squares/fx.svg), which minimises the square of the errors, ![$e_i](assets/least-squares/e.svg), where ![$e_i](assets/least-squares/e.svg) is defined as ![$e_i=y_i-f(x_i)$](assets/least-squares/ei.svg). In other words:

![$\min_f(x) \sum_{i=1}^{n} e_i^2$](assets/least-squares/min_ei.svg)

So let's work this out for the *Linear* case. Let's expand out the summation and then see if we can simplify it:

First, substitute for ![$e_i](assets/least-squares/e.svg):

![$\sum_{i=1}^{n} e_i^2=\sum_{i=1}^{n} (y_i-f(x_i))^2$](assets/least-squares/sum_ei_yi.svg)

Next, replace ![$f(x)$](assets/least-squares/fx.svg):

![$ =\sum_{i=1}^{n} (y_i-(A+Bx_i))^2$](assets/least-squares/sum_ei_lin.svg)

Now, expand out the square:

![$ =\sum_{i=1}^{n} (y_i^2-2y_i(A+Bx_i)+(A+Bx_i)^2)$](assets/least-squares/sum_ei_lin2.svg)

![$ =\sum_{i=1}^{n} (y_i^2-2y_iA-2y_i Bx_i+A^2+2ABx_i+B^2x_i^2)$](assets/least-squares/sum_ei_lin3.svg)

So the goal is to find *A* and *B* to minimise the above expression: 

![$\min_{A,B} \sum_{i=1}^{n} (y_i^2-2y_iA-2y_i Bx_i+A^2+2ABx_i+B^2x_i^2)$](assets/least-squares/min_ei_lin3.svg)

Going back to calculus basics, a maximum or minimum will be when the first derivative is 0. So let's differentiate with respect to *A* :

![$\frac{\partial }{\partial A}  \sum_{i=1}^{n} (y_i^2-2y_iA-2y_i Bx_i+A^2+2ABx_i+B^2x_i^2)=\sum_{i=1}^{n} (-2y_i+2A+2Bx_i)$](assets/least-squares/da_lin.svg)

So we want to find when this is 0. Rearranging (and dividing by 2):

![$\sum_{i=1}^{n} A=\sum_{i=1}^{n}y_i-\sum_{i=1}^{n}Bx_i$](assets/least-squares/da_lin2.svg)

We can then expand the sums for this:

![$nA=\sum_{i=1}^{n}y_i-B\sum_{i=1}^{n}x_i$](assets/least-squares/da_lin3.svg)

Dividing by *n*, this gives us:

![$A=\bar{y}-B\bar{x}$](assets/least-squares/da_lin4.svg)

Where ![$\bar{x}$](assets/least-squares/x_bar.svg) and ![$\bar{y}$](assets/least-squares/y_bar.svg) are the average of *x* and *y* respectively.

Now back to the original expression, this time let's differentiate with resepect to *B*:

![$\frac{\partial}{\partial B} \sum_{i=1}^{n} (y_i^2-2y_iA-2y_i Bx_i+A^2+2ABx_i+B^2x_i^2)=$](assets/least-squares/db_lin.svg)
![$\sum_{i=1}^{n} (-2y_i x_i+2Ax_i+2Bx_i^2)$](assets/least-squares/db_lin1.svg)

Following a similar approach, we can get that:

![$B=\frac{\sum_{i=1}^{n}y_i x_i+An\bar{x}}{\sum_{i=1}^{n}x_i^2}$](assets/least-squares/db_lin2.svg)

If we have a known intercept, then we can substitute this for *A* above. Otherwise, we can substitute our expression we had above for *A*:

![$B\sum_{i=1}^{n}x_i^2=\sum_{i=1}^{n}x_i y_i-n(\bar{y}-B\bar{x})\bar{x}$](assets/least-squares/db_lin3.svg)

![$B(\sum_{i=1}^{n}x_i^2-n\bar{x}^2)=\sum_{i=1}^{n}x_i y_i-n\bar{y}\bar{x}$](assets/least-squares/db_lin4.svg)

![$B=\frac{\sum_{i=1}^{n}x_i y_i-n\bar{y}\bar{x}}{\sum_{i=1}^{n}x_i^2-n\bar{x}^2}$](assets/least-squares/db_lin5.svg)

So to find *A* and *B* all we need to compute is:

- *n* - the number of records
- ![$\bar{x}$](assets/least-squares/x_bar.svg) - the average of *x*
- ![$\bar{y}$](assets/least-squares/y_bar.svg) - the average of *y*
- ![$\sum_{i=1}^{n}x_i^2$](assets/least-squares/sum_x_sq.svg) - the sum of *x* squared
- ![$\sum_{i=1}^{n}x_i^2$](assets/least-squares/sum_xy.svg) - the sum of *x* times *y*

All of this is straight forward using a Summarize tool.

## Building the First Macro

So let's start building the macro. This first version will handle computing *A* and *B* for the linear model.

![Macro Version 1](assets/least-squares/macro_v1.png)

We start by taking a standard macro input. I have chosen not to expose a FieldMap but instead create new variables called `__X__` and `__Y__`. I use a drop down box to allow you to map the field to each, using an action tool to update the raw XML of a pair of formula tools. 

Next, I compute values for `__XX__` and `__XY__` which I will need to compute the totals. Then it is on to the Summarize tool to compute the five values I need. Additionally, I use a List Box to allow selection of the Group By within this tool. This is a little fiddly inside the formula for action tool, but basically it works by adding the group by entries to the raw XML of the summarise:

```
IIF([#1]='""',
    '',
    '<SummarizeField field=' + 
      Replace([#1], '|||', ' action="GroupBy" /><SummarizeField field=') +
    ' action="GroupBy" />')
+
[Destination]
```

Finally, last step is to compute the `Slope` and `Intercept` and to use a select tool to drop all the intermediary fields. One last little catch is to remember to select the *Output fields change based on macro's configuration or data input* option within the Interface Designer.

![Output fields change](assets/least-squares/output_fields.png)

## Expanding to Other Models

Currently, it can only . We've basically finished the hard work. 