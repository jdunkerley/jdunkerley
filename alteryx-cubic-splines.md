# Creating a Cubic Spline In Python and Alteryx

As a bit of a thought experiment, I wondered if how hard it would be to create a cubic spline interpolation within Alteryx. As with many of my games [*BaseA* rules](https://jdunkerley.co.uk/2019/11/29/lets-alteryx-the-advent-of-code-2019/) apply. Let's start by reviewing how to create a cubic spline and then build it up.

Let's start with how to build a cubic spline. I chose to use the algorithm as described in [Wikiversity](https://en.wikiversity.org/wiki/Cubic_Spline_Interpolation). Specifically with type II simple boundary conditions.. I'm not going through the maths but will describe the steps to build the spline.

## Building a Tridiagonal Matrix and Vector

First, step is given an *X* array and a *Y* array of equal length *n* (greater than 2), we want to build a *tri-diagonal matrix* which we will then solve to produce the coefficients for the piece-wise spline. The goal of the spline is that it hits every point *(x, y)* and that the first and second derivatives match at these points too.

Sticking with notation in the paper, lets define `H` to be an `n-1` length array of the changes in `X`:

![Equation for H](https://math.now.sh?from=h_i=x_i-x_%7Bi%2B1%7D) for ![i=0 to n-2](https://math.now.sh?from=i=0..n-2)

A [tri-diagonal matrix](https://en.wikipedia.org/wiki/Tridiagonal_matrix) is a square matrix where all values except for the main diagonal and first diagonals below and above this for example:

```plaintext
1   2   0   0
2   3   2   0
0   2   3   2
0   0   2   1
```

One advantage of a tri-diagonal matrix is that they are fairly straight forward to invert and solve linear equations with them. For the sake of coding up the algorithm - I will define `B` to be the `n` length array holding the diagonal elements, `A` to be the `n-1` length array of the diagonal above this and `C` to be the `n-1` length array of the diagonal below:

```plaintext
b0   c0    0    0
a0   b1   c1    0
 0   a1   b2   c2
 0    0   a2   b3
```

For the spline, these are arrays are given by:

![Equation for A](https://math.now.sh?from=a_i%3D%5Cfrac%7Bh_i%7D%7Bh_i%2Bh_%7Bi%2B1%7D%7D) for ![i=0 to n-3](https://math.now.sh?from=i=0..n-3)

![Equation for B](https://math.now.sh?from=b_i=2) for ![i=0 to n-1](https://math.now.sh?from=i=0..n-1)

![Equation for C](https://math.now.sh?from=c_i%3D%5Cfrac%7Bh_i%7D%7Bh_%7Bi-1%7D%2Bh_i%7D) for ![i=1 to n-2](https://math.now.sh?from=i=1..n-2)

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

![Equation for D](https://math.now.sh?from=d_%7Bi%7D%3D6%5Cfrac%7B%5Cfrac%7By_%7Bi%2B1%7D-y_i%7D%7Bh_%7Bi%2B1%7D%7D-%5Cfrac%7By_i-y_%7Bi-1%7D%7D%7Bh_i%7D%7D%7Bh_i%2Bh_%7Bi%2B1%7D%7D) for ![i=0 to n-2](https://math.now.sh?from=i=1..n-2)

![Equation for D(N-1)](https://math.now.sh?from=d_%7Bn-1%7D%3D0)

Implementing this in Python looks like:

```python
def create_target(n: int, h: List[float], y: List[float]):
    return [0] + [6 * ((y[i + 1] - y[i]) / h[i] - (y[i] - y[i - 1]) / h[i - 1]) / (h[i] + h[i-1]) for i in range(1, n - 1)] + [0]
```

## Solving the Tridiagonal Equation

To solve a tridiagonal system, you can use [Thomas Algorithm](https://en.wikipedia.org/wiki/Tridiagonal_matrix_algorithm). Mapping this onto the terminology above. We first derive length *n* vectors *C'* and *D'*:

![Equation for C'](https://math.now.sh?from=c'_0%3D%5Cfrac%7Bc_0%7D%7Bb_0%7D)

![Equation for C'](https://math.now.sh?from=c'_i%3D%5Cfrac%7Bc_i%7D%7Bb_i-c'_%7Bi-1%7Da_%7Bi-1%7D%7D) for ![i=1 to n-2](https://math.now.sh?from=i=1..n-2)

![Equation for C'](https://math.now.sh?from=c'_%7Bn-1%7D=0)

![Equation for D'](https://math.now.sh?from=d'_0%3D%5Cfrac%7Bd_0%7D%7Bb_0%7D)

![Equation for D'](https://math.now.sh?from=d'_i%3D%5Cfrac%7Bd_i-d'_%7Bi-1%7Da_%7Bi-1%7D%7D%7Bb_i-c'_%7Bi-1%7Da_%7Bi-1%7D%7D) for ![i=1 to n-1](https://math.now.sh?from=i=1..n-1)

Having worked out *C'* and *D'*, you can then calculate the result vector `X`:

![Equation for X'](https://math.now.sh?from=x_%7Bn-1%7D%3Dd'_%7Bn-1%7D)

![Equation for X'](https://math.now.sh?from=x_%7Bi%7D%3Dd'_i-c'_ix_%7Bi%2B1%7D) for ![i=n-2 to 0](https://math.now.sh?from=i=n-2..0)

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

So the last step is to convert this into a set of cubic curves. To find the value of the spline at the point *x*, you want to find *j* such that *x<sub>j</sub> &lt; x &lt; x<sub>j+1</sub>*. Lets define *z* as

![Equation:z=\frac{x-x_j}{h_j}](https://math.now.sh?from=z%3D%5Cfrac%7Bx-x_j%7D%7Bh_j%7D)

*z* has property of being 0 when *x* = *x<sub>j</sub>* and 1 when *x* = *x<sub>j+1</sub>*. The value of spline at *x*, *S(x)* is:

![Equation of spline](https://math.now.sh?from=S%28x%29%0A%3D%5Cfrac%7B(M_%7Bj%2B1%7D-M_j)h_j%5E2%7D%7B6%7Dz%5E3%0A%2B%5Cfrac%7BM_jh_j%5E2%7D%7B2%7Dz%5E2%0A%2B(y_%7Bj%2B1%7D-y_j-%5Cfrac%7B(M_%7Bj%2B1%7D%2B2M_j)h_j%5E2%7D%7B6%7D)z%0A%2By_j)

We can now put together the whole function to build the spline coefficients. The final part we need is to wrap it up into a function which will find *j* and then evaluate the spline. There is a great library in python, [bisect](https://docs.python.org/3/library/bisect.html) which will do a binary search to find *j* for us. 

The code below implements this also validates the inputs:

```python
def compute_spline(x: List[float], y: List[float]):
    n = len(x)
    if n < 3:
        raise ValueError('Too short an array')
    if n != len(y):
        raise ValueError('Array lengths are different')

    h = compute_changes(x)
    if any(v < 0 for v in h):
        raise ValueError('X must be strictly increasing')

    A, B, C = create_tridiagonalmatrix(n, h)
    D = create_target(n, h, y)

    M = solve_tridiagonalsystem(A, B, C, D)

    coefficients = [[(M[i+1]-M[i])*h[i]*h[i]/6, M[i]*h[i]*h[i]/2, (y[i+1] - y[i] - (M[i+1]+2*M[i])*h[i]*h[i]/6), y[i]] for i in range(n-1)]

    def spline(val):
        idx = min(bisect.bisect(x, val)-1, n-2)
        z = (val - x[idx]) / h[idx]
        C = coefficients[idx]
        return (((C[0] * z) + C[1]) * z + C[2]) * z + C[3]

    return spline
```

The complete python code is available as a [gist](https://gist.github.com/jdunkerley/e23f29b07cae817203b8157d8a86e8a0):

<script src="https://gist.github.com/jdunkerley/e23f29b07cae817203b8157d8a86e8a0.js"></script>

## Testing the Spline

As always need to test to make sure all is working:

```python
import matplotlib.pyplot as plt

test_x = [0,1,2,3]
test_y = [0,0.5,2,1.5]
spline = compute_spline(test_x, test_y)

for i, x in enumerate(test_x):
    assert abs(test_y[i] - spline(x)) < 1e-8, f'Error at {x}, {test_y[i]}'

x_vals = [v / 10 for v in range(0, 50, 1)]
y_vals = [spline(y) for y in x_vals]

plt.plot(x_vals, y_vals)
```

Creates a small spline and plots the results:

![Test Plot](/assets/spline/test_plot.png)

## Recreating In Alteryx

So now we have the process, lets rebuild it in Alteryx. For the input, the macro takes two separate inputs - a table of KnownXs and KnownYs and a list of target Xs. So our first task is to build *H, A, B, C, D* from the inputs:

![Create Tridiagonal System](/assets/spline/create_habcd.png)

Using some [Multi-Row Formula](https://help.alteryx.com/current/designer/multi-row-formula-tool) tools it is fairly easy to create these. The expressions are shown below. In all cases the value is a *Double* and *NULL* is used for row which don't exist:

```plaintext
H=[X]-[Row-1:X]
A=IIF(ISNULL([Row+1:H]),0,[H]/([H]+[Row+1:H]))
C=IIF(ISNULL([Row-1:H]),0,[H]/([H]+[Row-1:H]))
B=2
```

Then using a [Join](https://help.alteryx.com/current/designer/join-tool) (on row position) and a [Union](https://help.alteryx.com/current/designer/union-tool) to add the last row to the set. Finally, *D* is given by:

```plaintext
IIF(IsNull([Row-1:X]) or IsNull([Row+1:X]),
    0,
    6 * (([Row+1:Y]-[Y])/[H] - ([Y]-[Row-1:Y])/[Row-1:H]) / ([H]+[Row-1:H])
)
```

![Solve the Tridiagonal System](/assets/spline/solve_tridiagonal.png)

Next up we need to solve the produced system. In order to save on storage, I chose to use a pair of multi-row formula tools to update *C* and *D*:

```plaintext
C=IIF(ISNULL([Row-1:X]),[C]/[B],IIF(ISNULL([Row+1:X]),0,[C]/([B]-[Row-1:C]*[Row-1:A])))
D=IIF(ISNULL([Row-1:X]),[D]/[B],([D]-[Row-1:D]*[Row-1:A])/([B]-[Row-1:C]*[Row-1:A]))
```

In order to compute the solution vector, *M*, we need to reverse the direction of the data. While you can use `Row+1` to access the next row in a multi-row formula tool it won't allow a complete iteration backwards. In order to do this, I added a [Record ID](https://help.alteryx.com/current/designer/record-id-tool) and then sorted the data on it descending. After which *M* is given by:

```plaintext
M=IIF(IsNull([Row-1:X]),[D],[D]-[C]*[Row-1:M])
```

We now have all the inputs, and so can move to compute the coefficients for each piece of the spline:

![Compute Coefficients](/assets/spline/compute_coefficients.png)

One small trick here is to skip the first row and join back to the same stream. This gives *x*, *y* and *M* for the next row. The coefficients are then computed using a normal formula tool:

```plaintext
CubeCoefficient=([EndM]-[M])*[H]*[H]/6
SquareCoefficient=[M]*[H]*[H]/2
LinearCoefficient=([EndY]-[Y]-([EndM]+2*[M])*[H]*[H]/6)
Constant=[Y]
```

The final challenge is to reproduce the *bisect* to find the row for each wanted X.