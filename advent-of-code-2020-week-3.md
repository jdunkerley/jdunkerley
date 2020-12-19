# Alteryxing The Advent of Code 2020 - Week 3

![Week 3 Graphic](assets/advent-2020-3/logo.jpg)

So [week 2](https://jdunkerley.co.uk/2020/12/05/alteryxing-the-advent-of-code-2020-week-1/) and [week 2](https://jdunkerley.co.uk/2020/12/13/alteryxing-the-advent-of-code-2020-week-2/) were both possible in BaseA Alteryx, although getting harder as the puzzles progress. Week 3 was the first time I needed to go beyond BaseA to find a solution for a couple of the parts (though in at least one case the community found a BaseA solution).

As with a couple of years ago, doing the Advent of Code, inspired me to do more work on the [Abacus](https://github.com/jdunkerley/AlteryxFormulaAddOns) library. This time I added four functions allowing for 64-bit integer based arithmetic. The numbers need to be passed as strings and are returned as strings. The new functions are:

- `Int64Add(a,b,c...)` - sums all the inputs
- `Int64Mult(a,b,c...)` - products all the inputs
- `Int64Div(a,b)` - computes `a / b` using integer division (i.e. returns `Floor(a / b)`)
- `Int64Mod(a,b)` - computes `a % b` (i.e. returns the remainder of `a / b`)

Anyway onto the puzzles!

## [Day 13 - Shuttle Search](https://adventofcode.com/2020/day/13)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-13/m-p/679903)

![My solution day 13](assets/advent-2020-3/day13.jd.jpg)
*Tools used: 21, run-time: 0.3s*

Part 1 was straight forward a simple formula of `CEIL([Time]/[ID])*[ID]-[Time]` gave the first time the bus would depart post the initial time. Part 2, however, was a lot harder and involved remembering (or [googling](https://www.google.com/search?q=solve+simultaneous+equations+modular)) some maths I had long since forgotten - the Chinese Remainder Theorem. Roughly, it breaks into the following steps:

1. Work out the *Product* of all the divisors (*n<sub>i</sub>*)
1. For each row work out *p<sub>i</sub>*, given by *Product / n<sub>i</sub>*
1. Solve the equation *s<sub>i</sub> p<sub>i</sub> = 1 (mod n<sub>i</sub>)*
1. A solution for the equations, *x* is given by the sum of all the *s<sub>i</sub>*
1. The minimal solution is given by *x mod Product*

I took a look at the python code from [Rosetta Code](https://rosettacode.org/wiki/Chinese_remainder_theorem) to help implement this in Alteryx. This solution worked perfectly for all the examples given in the puzzles. However, for the specific problem (a much larger set), the engine reported the following:

![Day 13 error](assets/advent-2020-3/day13.error.jpg)

The problem is Alteryx formulas are always evaluted as doubles. This is only a problem in step 4, where the values get very high. One feature I didn't know is that the summarise tool will accurately sum fixed decimal types (thanks to Ned for this hint). This allows we to complete step 4 but still leaves me stuck on step 5. This is where I went to the Abacus library and implemented the needed 64-bit integer functions. Having done this a formula of `Int64Mod([Sum_v], [Product])` having converted the values to strings computes the correct answer.

### Other Workarounds in BaseA

[CGoodman3](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-13/m-p/679966/highlight/true#M3456) implemented an additional generate rows to try 300 values around the result produced by Alteryx and see if it solves the equations.

[Ned Harding](https://github.com/NedHarding/Advent2020/blob/main/BigMult.yxmc) produced a macro which will handle big integer multiplication in BaseA. A useful tool if you need to deal with these huge numbers.

### Iterative Macro - Solving One Equation at a Time

![dsmdavid's solution](assets/advent-2020-3/day13.dsm.jpg)

![dsmdavid's inner macro](assets/advent-2020-3/day13.dsm.macro.jpg)

[dsmdavid](https://community.alteryx.com/t5/user/viewprofilepage/user-id/18848)'s solution was a very elegant iterative macro solving each equation one at a time and then increasing the step size to solve the next one. To quote his [post](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-13/m-p/679969/highlight/true#M3457):

```
I began with pen and paper, then some excel:

I started with
- number 3, offset 0
- number 5, offset 1
- number 7, offset 2
- number 11,offset 3

3 & 5 --> the first number that qualifies is 9 (mod(9,3) = 0 && mod(9+1,5)=0).
Then I need to find a number that qualifies for (3,5) and 7. I start with the first number that qualifies for the previous condition, go in increments of (3*5) --will all keep qualifying for mod(z,3) = 0, mod(z+1,5) = 0 -- until I find one that qualifies mod(z+2,7) = 0 (the first number that qualifies is 54)

For the next, I'll start at 54 and increase in increments of 105 (3*5*7) until I find one that qualifies mod(z+3,11) =0. (789).

And so on.
```

A great solution - and BaseA without issue.

## [Day 14 - Docking Data](https://adventofcode.com/2020/day/14)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-14/m-p/680007)

![My solution day 14](assets/advent-2020-3/day14.jd.jpg)
*Tools used: 16, run-time: 4.6s*

Back into a comfortable for Alteryx. This problem involved some binary masking operations. For part 1, you needed to ignore the `X`s and to set the bits of the value to `0` when the mask was `0` and `1` when the mask was `1`. A couple of expressions easily achieved this:

```
BinaryOr([Computed], BinToInt(ReplaceChar([Mask], "X", "0")))  # Set the 1 mask bits
BinaryAnd([Computed],BinToInt(ReplaceChar([Mask], "X", "1")))  # Set the 0 mask bits
```

After this, its just a case of using a summarise tool to pick the final value (this could also have been done with a sample tool), and then a final summarise to total all the values.

For part 2, the meaning of the mask was changed. In this case, it is applied to the address rather than the value. Additionally the `0`s are ignored. The `1`s are set on the address. The `X`s become wildcards mean both `0` and `1` and both should be evaluated. 

For simplicity of diagnosing problems, I chose to use the `IntToBin` function to write the address as a 36 character binary string and applied the `1`s. Then using some generate rows I created a row for every permutation and then using regular expressions created the new address values. Finally, after this the same double summarise tools produced the answer.

## [Day 15 - Rambunctious Recitation](https://adventofcode.com/2020/day/15)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-15/td-p/680525)

![My solution day 15](assets/advent-2020-3/day15.jd.jpg)
*Tools used: 13, run-time: 3m49s*

This puzzle reminded me of [day 9 from 2018](https://adventofcode.com/2018/day/9). You need to hold a lot of state and keep iterating. For part 1, at each step you add a new number to the list. I chose to do this by keeping the current list as a string:

![My solution day 15](assets/advent-2020-3/day15.state.jpg)

A generate rows tool is used to create 2020 rows and then the state is generated using a multi-row formula with expression:

```
[Row-1:State] + " " +
iif(
  Regex_CountMatches(
    REGEX_Replace([Row-1:State], "\d+$",""),
    "\b" + Regex_Replace([Row-1:State],".* ","") +"\b"
  ) == 0,
  "0",
  ToString(
    REGEX_CountMatches(
      REGEX_Replace(
        REGEX_Replace([Row-1:State], "\d+$",""),
        ".*\b" + Regex_Replace([Row-1:State],".* ","") + " ",
        " "
      ),
      " ")
  )
)
```

On each row, the current value is read from the last line of the string. If this value is present in the list (checked using the Regex_CountMatches) more than once, then we count the number of spaces between the last instance and the end of the string. If it wasn't present then a 0 is added.

For part 2, you need to do 30,000,000 iterations. This is clearly not possible with this string state I was keeping. It would involve a string with at least 60,000,000 characters. My first idea was to change to storing the state as key and value in the string for example:

![My solution day 15](assets/advent-2020-3/day15.state2.jpg)

The expression to update this is fairly complicated (some horribly nested regular expressions). This approach was an improvement on my part 1 approach but took nearly 10 minutes to run 100,000 iterations. Doing a little bit of analysis, I estimated it would take at least 2 years to run this to 30,000,000 iterations.

My non-BaseA solution to this was to use the `VarNumExists` and `VarNum` feature of the Abacus library. These allow you to define a variable dynamically and then update it as needed. This makes the iterative formula:

```
iif([RowCount]<=[Init],
  GetWord([Field1], [RowCount]-1)
  + iif([RowCount]>1 AND VarNum([Row-1:Value], RowCount - 1),"",""),
  ToString(
    iif(VarNumExists([Row-1:Value]),
      RowCount-VarNum([Row-1:Value])-1,
      0)
    + VarNum([Row-1:Value], RowCount - 1) * 0
   )
)
```

The row value is just the current value and the last time each row was accessed is stored in the `VarNum` variables. This allows Alteryx to complete the 30 million rows in about 4 minutes.

### Other Solutions

For part 1, a few went with the iterative macro approach to solving this. This was generally successful but significantly slower (4 minutes or so) versus the generate rows approach. That being said [AkimasaKajitani](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-15/m-p/680864/highlight/true#M3486) found a huge performance boost by using the AMP engine.

I do not believe it is possible to do part 2 within BaseA rules in anything resembling a sensible time. Some resorted to the python tool to solve this which allowed for answers in a few seconds.

## [Day 16 - Ticket Translation](https://adventofcode.com/2020/day/16)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-16/m-p/681341)

![My solution day 16](assets/advent-2020-3/day16.jd.jpg)
*Tools used: 30, run-time: 0.7s*

The first task was to parse the input. This input contains a set of rules, each looking like `seat: 2-3 or 7-9`, and a set of tickets each being a comma separated list. Using a Regex tool and Text to columns I ended up with:

![My solution day 16](assets/advent-2020-3/day16.parsedrules.jpg)

![My solution day 16](assets/advent-2020-3/day16.parsedtickets.jpg)
 
Having parsed the input, the next task is to filter the tickets fields to see which rules are valid for which fields. I choose to use an append fields to add every possible rule to every ticket and column. You can then join this to the set of ticket to produce the tickets where no field passed any rule (the unique tool in my workflow is not needed - bad tool golf!).

For part 2, you need to work out which rule applies to which column. I chose to use an iterative macro to solve this. Firstly, I filtered out the invalid tickets (using a join tool). After this I filter down to the valid Tickets, Columns and Rules. For each rule, I count the number of valid tickets and compare this with the distinct count of valid tickets. This gives the set of valid rules for each column:

![My solution day 16](assets/advent-2020-3/day16.validrules.jpg)

The last step is the iterative macro:

![My solution day 16](assets/advent-2020-3/day16.jd.macro.jpg)

In this case, each iteration picks out `ColumnName`s that only occur once in the list. The `Column` associated with this is then returned with the name and all other rows with the same `Column` value are removed and the iteration repeats. This is pretty similar to previous macros. Having solved this the answer to the puzzle is easily obtained with a join and summarise.

Good to be back to pure BaseA!

### Macro Free

![]()

## Wrapping Up

I am really pleased to have passed my total stars from 2018 this week. I did have to solve 2 using Abacus functions, but still great to see how far we have got using Alteryx.

![Leaderboard as of 19/12/2020](assets/advent-2020-3/leaderboard.jpg)

A few more git repositories added this week:

- Mine: https://github.com/jdunkerley/adventofcode/
- NicoleJohnson: https://github.com/AlteryxNJ/AdventOfCode_2020
- ColoradoNed: https://github.com/NedHarding/Advent2020
- CGoodman3: https://github.com/ChrisDataBlog/AdventOfCode_2020
- AlteryxAd: https://gitlab.com/adriley/adventofcode2020-alteryx/
- NiklasEk: https://github.com/NiklasJEk/AdventOfCode_2020
- peter_gb: https://github.com/peter-gb/AdventofCode
- AkimasaKajitani: https://github.com/AkimasaKajitani/AdventOfCode
- dsmdavid: https://github.com/dsmdavid/AdventCode2020

Onto the final six days. Hopefully, someone can get the first 50 stars using Alteryx (with as much as possible in BaseA) this year!