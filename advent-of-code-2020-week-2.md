# Alteryxing The Advent of Code 2020 - Week 1

So [week 1](https://jdunkerley.co.uk/2020/12/05/alteryxing-the-advent-of-code-2020-week-1/) was well suited to Alteryx, lets see how week 2 unfolded! A nice and gentle Sunday puzzle lulled me into the belief that it was going to be an easy week, followed by the first needed use of an iterative macro, and then something that looked far too much like the dreaded [IntCode of 2019](https://adventofcode.com/2019/day/2)...

As with last week, I've picked some examples from around the community for different approaches to my own. This week also saw a [useful macro](https://github.com/NedHarding/Advent2020/blob/main/AdventOfCodeInput.yxmc) by Ned Harding which will download and parse the input from the Advent of Code site.

Some of the puzzles this week involve some complicate workflows so I will do my best to explain them as clearly as I can.

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

##