# Trading Turtles in Enso - Part 1 Gathering Data

We've just released version 2025.2 of [Enso Analytics](https://ensoanalytics.com). In this release, we've introduced several new features and improvements that make it easier than ever to gather, clean and analyze your data.

Back in the 1980s, [Richard Dennis](https://en.wikipedia.org/wiki/Richard_Dennis), a well known commodities trader, and his partner [William Eckhardt](https://en.wikipedia.org/wiki/William_Eckhardt_(trader)), conducted an experiment to see if they could teach a group of people with no trading experience to become successful traders. They developed a simple trend-following trading system with a set of rules and guidelines, and recruited a set of individuals, who became known as Turtles, to follow it. After a two week training program, and then a one month trial period, the traders who successfully implemented the system were then recruited to trade on Dennis's own money for the next 5 years. In the end, the strategy reportedly made $175 million for Dennis.

In this series, I'll look at how to use Enso to gather the price data, process and clean the series and then look at the theoretical results of the trading strategy.

Let's start by reviewing the trading rules that the Turtles followed.

## Trading Rules

### Trade Size

In an advanced feature for the time, the system used a volatility-based position sizing method. The size of each trade was normalised based on the dollar volatility of the instrument being traded. 

The dollar volatility was calculated as: 

### Entry Rules

The Turtles used two related system for entering a position. They are both based on the Channel Breakout system taught by Richard Donchian. A breakout is defined when the price moves outside of the high and low of the previous specified number of days.

#### System 1 Entry - 20-day breakout

System 1 used the breakout system based on a 20-day channel. If the price moved above the highest high of the last 20 days, a long position was entered. Conversely, if the price moved below the lowest low of the last 20 days, a short position was entered.

The entry signal would be ignored if the previous System 1 trade would have resulted in a winning trade.

#### System 2 Entry - 55-day breakout

System 2 used the breakout system based on a 55-day channel. If the price moved above the highest high of the last 55 days, a long position was entered. Conversely, if the price moved below the lowest low of the last 55 days, a short position was entered.

Unlike System 1, the entry for System 2 would always be traded.

### Adding Units