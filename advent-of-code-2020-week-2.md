# Alteryxing The Advent of Code 2020 - Week 2

![Week 2 Graphic](assets/advent-2020-2/logo.jpg)

So [week 1](https://jdunkerley.co.uk/2020/12/05/alteryxing-the-advent-of-code-2020-week-1/) was well suited to Alteryx, lets see how week 2 unfolded! A nice and gentle Sunday puzzle lulled me into the belief that it was going to be an easy week, followed by the first needed use of an iterative macro, and then something that looked far too much like the dreaded [IntCode of 2019](https://adventofcode.com/2019/day/2)...

As with last week, I've picked some examples from around the community for different approaches to my own. This week also saw a [useful macro](https://github.com/NedHarding/Advent2020/blob/main/AdventOfCodeInput.yxmc) by Ned Harding which will download and parse the input from the Advent of Code site. I also played with a version of this, which will [download the leaderboard](https://github.com/jdunkerley/adventofcode/blob/master/Build%20Leaderboard.yxmd) so I could play with the results - and see if anyone had beaten Nicole Johnson yet!

Some of the puzzles this week involve some complicate workflows so I will do my best to explain them as clearly as I can. Where I can't find a substantially different approach (or don't understand the other one!) I haven't included below.

# [Day 6 - Custom Customs](https://adventofcode.com/2020/day/6)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-6/m-p/676470)

![My solution day 6](assets/advent-2020-2/day6.jd.jpg)
*Tools used: 6, run-time: 0.2s*

A well suited problem for Alteryx. First, using a multi-row formula tool to identify each group, with the `null` rows delimiting when a group ends. The old trick of a Regex tool in tokenise mode with expression of `.` will break each character into a separate record. A summarise tool grouping by `Group` and `Char` will produce a record for each meaning the answer for part 1 is just the row count.

For part 2, you need to know how many people each group and then join this with those characters within that group which has the same count. This can easily be done using a Join tool on `Group` and `Count`, with the `J` output record count giving the answer for part 2.

There weren't any real big alternative approaches to this one. A few people used a Unique tool for part 1 to produce the unique set.

# [Day 7 - Handy Haversacks](https://adventofcode.com/2020/day/7)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-7/m-p/676704)

![My solution day 7](assets/advent-2020-2/day7.jd.jpg)

![My inner macro](assets/advent-2020-2/day7.jd.macro.jpg)
*Tools used: 22 (including iterative macro), run-time: 3.6s*

This is a fairly classic problem of needing to build and walk a heirarchy. For my solution I chose to use an Iterative macro to fold up leaf nodes into the parent and then remove. So the first task as always is to parse the input - so back to the usual Regex and Text to Columns tools to produce something like:

![Parsed day 7 input](assets/advent-2020-2/day7.jd.parsed.jpg)

Each row represent a parent and child (with the count for the child). My iterative macro selects the `Outer` colours whose `InnerColour` are not in the Outer list (the leaf of the network as it stands). These rows are then written out to the output. Additionally, their children are copied into their parent nodes and scaled by the `InnerNumber` so for example:

```
shiny gold	1	dark olive
shiny gold	2	vibrant plum
dark olive	3	faded blue 
dark olive	4	dotted black
vibrant plum	5	faded blue
vibrant plum	6	dotted black
```

`Dark olive` and `vibrant plum` are the leaf nodes. The last four lines are written as an output and the input for the next iteration becomes:

```
shiny gold	1	dark olive
shiny gold	2	vibrant plum
shiny gold	3	faded blue
shiny gold	4	dotted black
shiny gold	10	faded blue
shiny gold	12	dotted black
```

On the next iteration, the new leaf node is `shiny gold` so these 6 rows are written to the output. The iterative loop is then empty so the macro exits. As I calculated the count as I went along parts 1 and 2 are both then solved by just filtering and summarising the rows.

## Simpler Hierarchy Macro

The approaches were all generally similar, but I thought I would highlight [Nicole Johnson](https://community.alteryx.com/t5/user/viewprofilepage/user-id/3824)'s simpler heirarchy macro. She has answered similar questions to this on the community and has a blog post about [Kevin Bacon](https://community.alteryx.com/t5/Engine-Works/Building-a-Hierarchy-With-Kevin-Bacon/ba-p/453715) on the subject.

![Nicole's macro for part 2](assets/advent-2020-2/day7.nj.macro.jpg)

Unlike my macro, Nicole's takes in 2 inputs - the set of all connections (same basic format as my input) and the immediate children of `shiny gold`. Each iteration moves down to the children and multipies the quantity. These rows are output at each step (`R` output) and looped round to be the input to the next loop. When a leaf is reach, there will be no connections joined so the macro will terminate.

# [Day 8 - Handheld Halting](https://adventofcode.com/2020/day/8)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-8/m-p/677337)

![My solution day 8](assets/advent-2020-2/day8.jd.jpg)

![My inner macro](assets/advent-2020-2/day8.jd.macros.jpg)
*Tools used: 26 (including macros), run-time: 43.8s*

My first reaction was - uh oh this is going to be like Int code and take forever. My approach was fairly straight forward. First, I parsed the instructions and then pass this into the iterative macro. The iterative macro also takes a state input which is looped round in the iteration. This looks like:

```
Ptr: 1     # Current Instruction
Value: 0   # Current Value
Exec: ''   # Set of executed values
```

On each iteration, the instruction at `Ptr` is looked up. The `Exec` string is checked to see if it contains the `Ptr` already (the termination condition for the loop). If it does then the current row is written to the `R` output and the loop terminates. Otherwise, `Ptr` is added to the `Exec` string and new values for `Ptr` and `Value` are computed and passed to the loop output. The result at each step is also outputted (to the third output) as this is needed for Part 2. The answer for part 1 is given in the `R` output.

For part 2, I chose to use a batch macro to vary one instruction at a time and then run the above iterative macro. In this case, the required answer will be when the iterative macro terminates with a `null` result. You only need to test changing the `nop` and `jmp` operations - this gave a limited set (94) of cases to try. Each case is passed in as control parameter and then the instruction set is altered using a formula tool. Ideally, this would terminate on the first successful result but I never got that termination to work wihtin the batch macro.

## Macro Free

![ColoradoNed's day 8](assets/advent-2020-2/day8.nh.jpg)

I had to chose Ned Harding's macro free approach for this one. As he says if you can avoid iterative or batch macros, it is easier to debug and much faster (this version runs in 2.9 seconds).

First, Ned combines the expressions into a long single string with each instruction being 5 characters long. The operation is shortened to a single character and the value is padded with spaces to be 4 characters long. Next, a generate rows tool is used to create as many rows as there are instructions plus 1. This is used to mutate any `jmp` to `nop` and vice versa within the set. A unique tool is then used to remove the records which have not been changed.

Each of these 'programs' is then fed into a generate rows tool which creates up to 300 steps for each. Then a multi-row formula tool traces through which instruction would be processed on each step. A second multi-row formula tool then evaluates the value of the accumulator. Finally, a third multi-row formula tool is track the steps which have been executed. If a repeat is detected, this expression will return a `#error`, if it finds the terminating expression then `#success` is written.

A very clever and very quick way to solve this problem.
# Day 9

# Day 10

# Day 11

# Day 12

# Wrapping Up

A significantly harder week but still a lot of success with BaseA. Many have now passed my total of 18 stars from last week and are still going strong. Maybe this year will be the first year of 50 stars in BaseA.

A slightly increased collection of git repos this week:

- Mine: https://github.com/jdunkerley/adventofcode/
- NicoleJohnson: https://github.com/AlteryxNJ/AdventOfCode_2020
- ColoradoNed: https://github.com/NedHarding/Advent2020
- CGoodman3: https://github.com/ChrisDataBlog/AdventOfCode_2020
- AlteryxAd: https://gitlab.com/adriley/adventofcode2020-alteryx/

Onto week 3!