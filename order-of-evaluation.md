# Order of Evaluation in an Alteryx Formula

This post is the start of a small series around some of the functionality coming in the Alteryx Abacus 1.4 release. While this post, doesn't directly use anything in the add in understand the sequence of evaluation within an Alteryx expression is critical when working with variables (one of the big new features in the release). It specifically looks at how Alteryx evaluates an expression. While in general you don't care, one your expression has '[side-effects](https://en.wikipedia.org/wiki/Side_effect_(computer_science))', it starts to really matter.

In this post, I'm going to use a special function - `LOG` this allows me to get a line written to a file to tell me what was evaluated. This is a new function in Abacus 1.4. The syntax is:

```none
LOG(<ReturnValue>, <FileName>, <Message>)
```

# Back to School

Lets go back to basics to start with some simple maths. Maths defines an order of operation. I'm English so I learnt it as 'BODMAS':

- **B**rackets (Parenthesis in American)
- **O**rder (Exponents in American)
- **D**ivision / **M**ultiplication
- **A**ddition / **S**ubstraction

If there is an ambiguity then *left-to-right* is used (e.g. `3-4-5` is evaluated as `(3-4)-5` i.e. -6). So lets do some simple tests in Alteryx. 

![Simple Maths 1](assets/order/basic.maths.1.jpg)

Unsurpisingly, this produces the correct result of `-6`. Now, another `10-3+2` should be treated as `(10-3)+2` i.e. 9:

![Simple Maths 2](assets/order/basic.maths.2.jpg)

In Alteryx's case, *Order* is not an operator, it's a function so onto...

# Progamming Languages

Programming languages have way more operators. The table below summarises the order assuming Alteryx follows this standard:

- **B**rackets, Function calls
- Unary operators - `!`, `-`, `+`
- **D**ivision / **M**ultiplication
- **A**ddition / **S**ubstraction
- Comparisons: less-than and greater-than (`<`, `>`, `<=`, `>=`)
- Comparisons: equal and not equal (`==`,`!=`)
- Logical AND, `&&`
- Locical OR, `||`

Please not some of the standard operators (e.g. `%`) are function calls in Alteryx expressions (e.g. `MOD`). Lets consider a simple expression: `4 + 9 > 12`. This will be evaluated as `(4 + 9) > 12`:

![Simple Logic 2](assets/order/logic.case.1.jpg)

Lets look at a bit of a more weird and wonderful example. Let's try `3 < 2 != 3 < 5`. This evaluates to `(3 < 2) != (3 < 5)` which evaluates to `TRUE`:

![Simple Logic 2](assets/order/logic.case.2.jpg)

## Functions

Let's think about another quick example. What about `MOD(7 + 4, 2) + -MOD(7, 4)`? Well following the normal logic we would evaluate each of the `MOD` functions and then evaluate the addition. In other words:

1. Evaluate `7 + 4` ==> `11`
2. Evaluate `MOD(11, 2)` ==> `1`
3. Evaluate `MOD(7, 4)` ==> `3`
4. Evaluate Unary Minus ==> `-3`
5. Evaluate `1 - 3` ==> `-2`

![Simple Logic 2](assets/order/mods.jpg)

## Short Circuits