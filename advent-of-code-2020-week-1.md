# Alteryxing The Advent of Code 2020 - Week 1

![Week 1 Graphic](assets/advent-2020-1/logo.jpg)

So it's December and time again for the annual [Advent of Code](https://adventofcode.com/2020). For those not familiar, this is a set of 25 puzzles (each with 2 parts) set by [Eric Wastl](http://was.tl/). They have a Christmas theme and story, and are solvable with just a little programming knowledge and some puzzle solving skills. The puzzles start quite easy and get increasingly more complicated, and part 2 can often be a lot harder.

A couple of years ago, [Adam Riley](https://twitter.com/AlteryxAd/status/1069619208865107969?ref_src=twsrc%5Etfw) suggested we try solving them in Alteryx and so a new annual tradition began. It is worth noting that the puzzles do not necessarily suit Alteryx, but trying to think how to solve them is a great chance to sharpen your skills. We created some rules - solving using BaseA:

![BaseA](assets/advent-2020-1/basea.jpg)

- No RunCommand tool
- No Python tool
- No R tool
- No SDK based Custom Tools (macros are fine)
- No Formula SDK extensions
- Download tool allowed for downloading (no posting to APIs to get answers from!)

If you want to join us, we have an Alteryx leaderboard you can join by going to https://adventofcode.com/2019/leaderboard/private and using the code `453066-ca912f80`. We are chatting on the Alteryx Community under the [Advent of Code label](https://community.alteryx.com/t5/forums/filteredbylabelpage/board-id/general-discussions/label-name/advent%20of%20code). The leaderboard awards points by order in which they are solved. As the puzzles are published at midnight Eastern time, this give those who live on the West Coast an advantage. Those of us in the UK, it's 5am which is not a good time for my brain at least! Generally, this means it is fairest to look at total stars rather than points.

For this year, I thought I would write up my solutions with some alternatives solutions from other people every week (well at least while we succeed at solving them!). When building a workflow, there are a couple of other dimensions we can look at beyond just working: how fast is the workflow and 'Tool Golf' (i.e. how few tools can we use)! So I will try and pick different approaches to my own.

# [Day 1 - Report Repair](https://adventofcode.com/2020/day/1)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-1/m-p/675532)

![My solution day 1](assets/advent-2020-1/day1.jd.jpg)
*Tools used: 10, run-time: 0.3s*

So for this puzzle, you need to find the pair of numbers which totalled 2020. I chose to work out the second number for each one (a new column `Miss`), I can then join this to itself to find the possibilities. I then used a filter tool to pick the `Miss` being greater than the `Field1`. Finally, computed the product with another formula tool.

For part 2, you need to do the same but with three numbers. First, I chose  to make all possible pairs using an Append Fields tool (set to allow all appends). After this, the process is similar.

For best 'tool golf', I could have merged a lot of this into the formula (doing the comparison and only producing a `Miss` if fields will be in order) and join tools.

## All Possible Combinations

![Jean-B's Solution](assets/advent-2020-1/day1.jb.jpg)

This solution belong's to [@JeanBalteryx](https://community.alteryx.com/t5/user/viewprofilepage/user-id/77398). In this case, first you create all possible pairs (again using Append Fields tool) and filter to the pairs where the total is correct. Using a select tool you get a single unique answer which you then compute the Result on. He also tidies up the data using a Select tool and put the results into a Browse tool (I chose to rely on Browse anywhere as should have a single result).

For part 2, the process is similar but by append to the full set of pairs, you get the complete set of triplets and again can perform the same filtering and sampling to produce the result.

# [Day 2 - Password Philosophy](https://adventofcode.com/2020/day/2)

![Count Matches](assets/advent-2020-1/day2.jd.jpg)
*Tools used: 5, run-time: 0.4s*

And so we enter the world of [Regular Expressions](https://xkcd.com/1171/). First, using a Regex tool to parse the input into columns. An expression of `(\d+)-(\d+) (.): (.*)` will break it out into 4 columns:

- `Min`: First number
- `Max`: Second number
- `Char`: Character
- `Password`: Password to check.

I then fed this into the filter tool using `REGEX_CountMatches` to count the number of times the specified character occurs: `REGEX_CountMatches([Password], [Char])` and can then compare with `Min` and `Max` to determine valid matches.

For part 2, I relied on the simple `Substring` function to find the characters to compare. Watch out for the [off by 1 error](https://twitter.com/codinghorror/status/506010907021828096?lang=en) - my brain wasn't working well at the time I was building it!

## Regex For The Win

![ColoradoNed's Solution](assets/advent-2020-1/day2.nh.jpg)

For this puzzle, I had to choose Ned Harding's solution. He won the 'tool golf' challenge with a score of 2 tools! **But this is not for the faint of heart!** Using `REGEX_Replace`, he constructs the regular expression for a `REGEX_Match`:

```
REGEX_Match(REGEX_Replace([Field1], ".*:", ""),
	REGEX_Replace([Field1], "^(\d+)-(\d+) (.).*$", "^[^$3]*\($3[^$3]*\){$1,$2}$"))
```

The first `REGEX_Replace` replaces up to and including the `:`. The second takes the `1-8 n` and turns it into `^[^n]*(n[^n]*){1,8}$`. This will ignore everything until the first `n` and then match between 1 and 8 blocks with an `n` at the start followed by some random characters.

For part 2, the formula becomes:

```
REGEX_Match(REGEX_Replace([Field1], ".*:", ""), 
	REGEX_Replace([Field1], "^(\d+)-(\d+) (.).*$", "\(^\(?=.{$1}[^$3]\)\(?=.{$2}$3\).*$\)|\(^\(?=.{$1}$3\)\(?=.{$2}[^$3]\).*$\)"))
```

In this case, the input `1-8 n` is turned into `(^(?=.{1}[^n])(?=.{8}n).*$)|(^(?=.{1}n)(?=.{8}[^n]).*$)`. This is is a substantially more complicated Regex. It consists of 2 scenarios separated by a `|`. The first case is when the 1st character is not `n` and the 8th character is `n`. The `(?=.{1}[^n])` is a non-capturing group checking that the character at second character is not `n`. Note in order to correct for the off by 1 problem Ned, kept a leading space. The second block `(?=.{8}n)` is a non-capturing group checking that character 8 is an `n`.

Some very clever regular expressions in here but not simple!
# Day 3

# Day 4

# Day 5 ...

# Wrapping Up

So thats week one (well first 5 days) down. Generally, these puzzles have been well suited to Alteryx. If you want to take a look at my solutions they are posted to [GitHub](https://github.com/jdunkerley/adventofcode/).

As it stands the leaderboard looks like:

