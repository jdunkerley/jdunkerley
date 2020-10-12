# Creating a Mandelbrot Set in Alteryx

![Alteryx Mandelbrot Set](assets/alteryx-mandelbrot/alteryx-mandelbrot.png)

When I was young and starting to program, one of the things that fascinated me was generating images with fractals. Given an often simple algorithm, you can create incredible images with very little code. The Mandelbrot set is a one such. As an experiment, and to test out the AMP engine, I wanted to experiment with creating one in Alteryx. As usual [BaseA](https://jdunkerley.co.uk/2019/11/29/lets-alteryx-the-advent-of-code-2019/) rules apply!

I thought it would also be interesting for people to see some of the iterations I went through rather than just the end product. This first post is only going to concentrate on the computation of the data for the set, the way I chose to produce the image, rendering as bitmap file, is one for another day.

## Quick Overview of How to Calculate the Mandelbrot Set

The Mandelbrot set starts with a simple equation that:

![Mandelbrot Equation](assets/alteryx-mandelbrot/mandelbrot_equation.svg)

With *z<sub>0</sub>=0* and *c* being a [complex number](https://en.wikipedia.org/wiki/Complex_number). This means that *z<sub>1</sub>=c*. 

Expanding this into real and imaginary parts:

Let ![Real and Imaginary](assets/alteryx-mandelbrot/z_n.svg) where *i* is the square root of -1

![Mandelbrot Equation Real](assets/alteryx-mandelbrot/mandelbrot_equation_real.svg)

![Mandelbrot Equation Imaginary](assets/alteryx-mandelbrot/mandelbrot_equation_imag.svg)

To produce the picture of the Mandelbrot set, you scan through different values of *c* and see if the iterative equations tends to infinity or remains bounded. This is normally tested by if the absolute value of *z* exceeds 2 within a certain number of iterations. The absolute of *z<sub>n</sub>* is:

![Absolute of Z](assets/alteryx-mandelbrot/absolute_z.svg)

In order to speed up the computation slightly, I chose to check when the absolute of z squared exceeded 4.

The colour of the point at *c* can be produced by how fast the iteration exceeded the limit. I chose to use a colour scheme from [D3](https://observablehq.com/@d3/color-schemes) - specifically the Rainbow one.

## Computing the Iterative Equation in Alteryx

So there a few options of how to create the set within Alteryx. Lets start with the simple part - creating the grid of points we will evaluate across. 

![Create Grid](assets/alteryx-mandelbrot/create-grid.jpg)

The input I chose for this was:

|Dim|Min|Max|Points|
|---|---|---|---|
|Real|-2|1|800|
|Imaginary|-1.1|1|600|

Using a formula tool, can compute the step size (`([Max]-[Min]) / (Points - 1)`). Feeding this into a generate row tools to make the 800 real points and 600 imaginary points. After this, using a Append Fields tool set to allow all appends it creates the complete grid of points.

![Full Grid](assets/alteryx-mandelbrot/grid-data.jpg)

The complication for the iterative equation is that we need to iterate 2 variables and track the numbers of steps. I tried a few different approaches to this.

### Approach 1: Iterative Macro

Probably the most obvious is to use an iterative macro. These can be sluggish but as each iteration will act on the entire grid and a maximum of about 80 steps will be needed its not a bad approach. First lets add a couple of columns to the grid:

![Compute Grid](assets/alteryx-mandelbrot/compute-grid.jpg)

1. *Thres*: The threshold to declare it is tending to infintiy
1. *MaxSteps*: Number of steps to test the data on
1. *ri*: Current real value
1. *ii*: Current imaginary value

![Iterative Macro](assets/alteryx-mandelbrot/iterative-macro.jpg)

Each iteration of the macro does the following:

1. Compute *di* the current value of the *|z|*
2. Compute the new values for *ri* and *ii*
3. Split the data set looking at those points which have *di > thres* or if maximum iterations has been reached.
4. Return those rows to the outer workflow, loop the others with the updated *ri* and *ii* values.

This approach was pretty straight forward and produces a 1080p image in about 19 seconds on my laptop. In this approach the difference between AMP and the old E1 engine was minimal.

### Approach 2: String States

### Approach 3: Spatial Objects

### Approach 4: Dynamic Formula

## Assigning Colours and Rendering the Output

## Wrapping Up

