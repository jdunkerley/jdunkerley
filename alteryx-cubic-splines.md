# Creating a Cubic Spline In Python and Alteryx

As a bit of a thought experiment, I wondered if how hard it would be to create a cubic spline interpolation within Alteryx. As with many of my games *BaseA* rules apply. Let's start by reviewing how to create a cubic spline and then build it up.

## Building a Cubic Spline

Let's start with how to build a cubic spline. I chose to use the algorithm as described in [Antony Jameson paper](http://aero-comlab.stanford.edu/Papers/splines.pdf). I'm not going through the maths but will describe the steps to build the spline.

First, step is given an *X* array and a *Y* array of equal length *n* (greater than 2), we want to build a *tri-diagonal matrix* which we will then solve to produce the coefficients for the piece-wise spline. The goal of the spline is that it hits every point *(x, y)* and that the first and second derivatives match at these points too.

Sticking with notation in the paper, lets define `H` to be an `n-1` length array of the changes in `X`:

![](https://math.now.sh?from=h_i=x_i-x_%7Bi%2B1%7D)

A tri-diagonal matrix is a square matrix where all values except for the main diagonal and first diagonals below and above this for example:

```
1 2 0 0
2 3 2 0
0 2 3 2
0 0 2 1
```

One advantage of a tri-diagonal matrix is that they are fairly straight forward to invert and solve linear equations with them. For the sake of coding up the algorithm - I will define `A` to be the `n` length array holding the diagonal elements, `B` to be the `n-1` length array of the diagonal above this and `C` to be the `n-1` length array of the diagonal below:

```
a1 b1  0  0
c1 a2 b2  0
 0 c2 a3 b3
 0  0 c3 a4
```

For the spline, these are arrays are given by:

![Equation for A](https://math.now.sh?from=a_i=2)

![Equation for B](https://math.now.sh?from=b_i%3D%5Cfrac%7Bh_%7Bi%2B1%7D%7D%7Bh_i%2Bh_%7Bi%2B1%7D%7D)

![Equation for C](https://math.now.sh?from=c_i%3D%5Cfrac%7Bh_i%7D%7Bh_i%2Bh_%7Bi%2B1%7D%7D)

** Boundary Points **

We can code this up easily enough in Python:

```python
def compute_changes(x):
    return [x[i+1] - x[i] for i in range(len(x) - 1)]

def create_tridiagonalmatrix(x, y):
    n = len(x)
    if n < 3:
        raise ValueError("Too short an array")
    if n != len(y):
        raise ValueError("Array lengths are different")

    h = compute_changes(x)
    A = [2] * n
    B = [h[i+1] / (h[i] + h[i+1]) for i in range(n - 1)]
    C = [h[i] / (h[i] + h[i+1]) for i in range(n - 1)]
```