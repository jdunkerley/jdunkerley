# Alteryxing The Advent of Code 2020 - Week 1

![Week 1 Graphic](/assets/advent-2020-1/logo.jpg)

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

For this year, I thought I would write up my solutions with a selection of solutions from other people every week (well at least while we succeed at solving them!). When building a workflow, there are a couple of other dimensions we can look at beyond just working: how fast is the workflow and 'Tool Golf' (i.e. how few tools can we use)! So I will try and pick different approaches to my own.

# [Day 1 - Report Repair](https://adventofcode.com/2020/day/1)
- [Community Discussion](https://community.alteryx.com/t5/General-Discussions/Advent-of-Code-2020-BaseA-Style-Day-1/m-p/675532)