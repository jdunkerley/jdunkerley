# Creating a Cubic Spline In Python and Alteryx

As a bit of a thought experiment, I wondered if how hard it would be to create a cubic spline interpolation within Alteryx. As with many of my games [*BaseA* rules](https://jdunkerley.co.uk/2019/11/29/lets-alteryx-the-advent-of-code-2019/) apply. Let's start by reviewing how to create a cubic spline and then build it up.

Let's start with how to build a cubic spline. I chose to use the algorithm as described in [Wikiversity](https://en.wikiversity.org/wiki/Cubic_Spline_Interpolation). Specifically with type II simple boundary conditions.. I'm not going through the maths but will describe the steps to build the spline.

## Building a Tridiagonal Matrix and Vector

First, step is given an *X* array and a *Y* array of equal length *n* (greater than 2), we want to build a *tri-diagonal matrix* which we will then solve to produce the coefficients for the piece-wise spline. The goal of the spline is that it hits every point *(x, y)* and that the first and second derivatives match at these points too.

Sticking with notation in the paper, lets define `H` to be an `n-1` length array of the changes in `X`:

![](https://math.now.sh?from=h_i=x_i-x_%7Bi%2B1%7D) for ![](https://math.now.sh?from=i=0..n-2)

A [tri-diagonal matrix](https://en.wikipedia.org/wiki/Tridiagonal_matrix) is a square matrix where all values except for the main diagonal and first diagonals below and above this for example:

```
1   2   0   0
2   3   2   0
0   2   3   2
0   0   2   1
```

One advantage of a tri-diagonal matrix is that they are fairly straight forward to invert and solve linear equations with them. For the sake of coding up the algorithm - I will define `B` to be the `n` length array holding the diagonal elements, `A` to be the `n-1` length array of the diagonal above this and `C` to be the `n-1` length array of the diagonal below:

```
b0   c0    0    0
a0   b1   c1    0
 0   a1   b2   c2
 0    0   a2   b3
```

For the spline, these are arrays are given by:

![Equation for A](https://math.now.sh?from=a_i%3D%5Cfrac%7Bh_i%7D%7Bh_i%2Bh_%7Bi%2B1%7D%7D) for ![](https://math.now.sh?from=i=0..n-3)

![Equation for B](https://math.now.sh?from=b_i=2) for ![](https://math.now.sh?from=i=0..n-1)

![Equation for C](https://math.now.sh?from=c_i%3D%5Cfrac%7Bh_i%7D%7Bh_%7Bi-1%7D%2Bh_i%7D) for ![](https://math.now.sh?from=i=1..n-2)

Using the simple boundary condition that the second derivative is equal to 0 at the end, gives the values for c<sub>0</sub> and a<sub>n-2</sub> both equal to 0.

We can code this up easily enough in Python:

```python
from typing import Tuple, List

def compute_changes(x: List[float]) -> List[float]:
    return [x[i+1] - x[i] for i in range(len(x) - 1)]

def create_tridiagonalmatrix(n: int, h: List[float]) -> Tuple[List[float], List[float], List[float]]:
    A = [h[i] / (h[i] + h[i + 1]) for i in range(n - 2)] + [0]
    B = [2] * n
    C = [0] + [h[i + 1] / (h[i] + h[i + 1]) for i in range(n - 2)]
    return A, B, C
```

Next, we need to compute the right-hand side of the equation. This will be an array of length `n`. For notation, I choose to call this `D` same as in the Wikiversity article:

![Equation for D(0)](https://math.now.sh?from=d_0%3D0)

![Equation for D](https://math.now.sh?from=d_%7Bi%7D%3D6%5Cfrac%7B%5Cfrac%7By_%7Bi%2B1%7D-y_i%7D%7Bh_%7Bi%2B1%7D%7D-%5Cfrac%7By_i-y_%7Bi-1%7D%7D%7Bh_i%7D%7D%7Bh_i%2Bh_%7Bi%2B1%7D%7D) for ![](https://math.now.sh?from=i=1..n-2)

![Equation for D(N-1)](https://math.now.sh?from=d_%7Bn-1%7D%3D0)

Implementing this in Python looks like:

```python
def create_target(n: int, h: List[float], y: List[float]):
    return [0] + [6 * ((y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1]) / (h[i] + h[i-1]) for i in range(1, n - 1)] + [0]
```

## Solving the Tridiagonal Equation

To solve a tridiagonal system, you can use [Thomas Algorithm](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm). Mapping this onto the terminology above. We first derive length *n* vectors *C'* and *D'*:

![Equation for C'](https://math.now.sh?from=c'_0%3D%5Cfrac%7Bc_0%7D%7Bb_0%7D)

![Equation for C'](https://math.now.sh?from=c'_i%3D%5Cfrac%7Bc_i%7D%7Bb_i-c'_%7Bi-1%7Da_%7Bi-1%7D%7D) for ![](https://math.now.sh?from=i=1..n-2)

![Equation for C'](https://math.now.sh?from=c'_%7Bn-1%7D=0)

![Equation for D'](https://math.now.sh?from=d'_0%3D%5Cfrac%7Bd_0%7D%7Bb_0%7D)

![Equation for D'](https://math.now.sh?from=d'_i%3D%5Cfrac%7Bd_i-d'_%7Bi-1%7Da_%7Bi-1%7D%7D%7Bb_i-c'_%7Bi-1%7Da_%7Bi-1%7D%7D) for ![](https://math.now.sh?from=i=1..n-1)

Having worked out *C'* and *D'*, you can then calculate the result vector `X`:

![Equation for X'](https://math.now.sh?from=x_%7Bn-1%7D%3Dd'_%7Bn-1%7D)

![Equation for X'](https://math.now.sh?from=x_%7Bi%7D%3Dd'_i-c'_ix_%7Bi%2B1%7D) for ![](https://math.now.sh?from=i=n-2..0)

The implementation of this in Python is shown below:

```python
def solve_tridiagonalsystem(A: List[float], B: List[float], C: List[float], D: List[float]):
    c_p = C + [0]
    d_p = [0] * len(B)
    X = [0] * len(B)

    c_p[0] = C[0] / B[0]
    d_p[0] = D[0] / B[0]
    for i in range(1, len(B)):
        c_p[i] = c_p[i] / (B[i] - c_p[i - 1] * A[i - 1])
        d_p[i] = (D[i] - d_p[i - 1] * A[i - 1]) / (B[i] - c_p[i - 1] * A[i - 1])

    X[-1] = d_p[-1]
    for i in range(len(B) - 2, -1, -1):
        X[i] = d_p[i] - c_p[i] * X[i + 1]

    return X
```

## Calculating the Coefficients

```python
def compute_spline(x: List[float], y: List[float]):
    n = len(x)
    if n < 3:
        raise ValueError("Too short an array")
    if n != len(y):
        raise ValueError("Array lengths are different")

    h = compute_changes(x)
    A, B, C = create_tridiagonalmatrix(n, h)
    D = create_target(n, h, y)

    M = solve_tridiagonalsystem(A, B, C, D)

    coefficients = [[(M[i+1]-M[i])/(6*h[i]), M[i]/2, (y[i+1] - y[i]), y[i]] for i in range(n-1)]
```
    