# Solving Advent of Code 2022 with Enso

<!-- Put an image here -->

Some members of the team building [Enso](https://enso.org/) decided to try and tackle last year's [Advent of Code](https://adventofcode.com/2022/) using the language and see how far we could get. I've previously tried [solving Advent of Code in Alteryx](https://jdunkerley.co.uk/2020/12/05/alteryxing-the-advent-of-code-2020-week-1/) and was personally interested to see whether it easier or harder with what we are building.

For those not familiar with the Advent of Code, every year [Eric Wastl](https://twitter.com/ericwastl) creates a set or 25 programming puzzles posted one a day over December. Each puzzle has two related parts with a set of test values and results for both parts. Often the second part is harder extension of the first part. Generally, there should be a solution which can complete within a few seconds (although we didn't always find that one!).

This post summarises a few of the puzzles we tried and some of the challenges we faced.

## Day 1 - Calorie Counting

- https://adventofcode.com/2022/day/1

Initial task of Advent of Code are usually quite simple. However that's great, especially when starting with new language and/or new IDE. As such, let's take a look at calorie counting and how that could be done with Enso. I'll do my best to share what I learned while going thru the task.

I was not new to the Enso language - e.g. it wasn't a problem to express the algorithm. However I usually coded Enso in text editor. This time I wanted to taste the real Enso IDE experience.

Problem #1 - how do I read a file? When you create a project you get a `src` directory. However, you can also create a `data` directory and put there various files to work with. You can then reference that directory as `enso_project.data` in your code. Reading a textual file as is simple as

```haskell
enso_project.data/"aoc1-test.txt" . read
```

Problem #2 - working the the textual file requires you to split the text to lines. Great, there is `.lines` function. But then there are groups of lines. My original solution just `.fold` over the lines and passed in a `Pair` of two numbers. However there is a much nicer solution: `operator1.split '\n\n'` - split the text on two new lines! Then one gets the needed groups easily.

Problem #3 is to conver the text lines to numbers. It is as simple as `_.map Integer.parse` - however here I learned a really cool trick. You can collapse a graph of nodes into a new function and you can then use the function at multiple pleases. Not just that, when double-clicking on the function name, you can tell the Enso IDE to open up the collapsed function and you can modify it. That's really cool as it help modularity and allows one to keep the program clean, focused on individual tasks and nicely organized inside of a single project.

Problem #4 is to do some statistics on the prepared data. If you are like me, you just do some artimetics yourself, but the cool kids (those who know all the Enso libraries) use `Statistic` library functions. Then it is just about requesting Statistic.Maximum` or `Stastic.Sum`. Really nice.

The calorie counting was a dumb task, but it opened me the doors to the Enso IDE, its concepts and power of the libraries it offers. I really recommend trying this simple task yourself. It is fun and it worths it!

## Day 8 - Treetop Tree House

- https://adventofcode.com/2022/day/8

## Day xx 

## Some of the Challenges

## Conclusion
