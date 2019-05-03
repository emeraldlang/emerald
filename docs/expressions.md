# Expressions
This page outlines all the valid literals and operators in the Emerald Language.

##### Syntax
```
expression = assignment;
assignment = IDENT "=" assignment | logic_or;
logic_or = logic_and {"||" logic_and};
logic_and = bit_or {"&&" bit_or};
bit_or = bit_xor {"|" bit_xor};
bit_xor = bit_and {"^" bit_and};
bit_and = equality {"&" equality};
equality = comparison {("==" | "!=") comparison};
comparison = shift {("<" | ">" | "<=" | ">=") shift};
shift = addition {("<<" | ">>") addition};
addition = multiplication {("+" | "-") multiplication};
multiplication = unary {("*" | "/") unary};
unary = ("!" | "-" | "~") unary | call;
call = primary ["("[arguments]")"];
primary = IDENT | STRING | NUMBER | "True" | "False" | "None" | "(" expression ")";

arguments = expression {"," expression};
```

## Literals
Used to represent values in Emerald.

### Boolean Literals
`True` and `False`.

### Null Literal
`None`

### Number Literals
Numbers can be expressed in base 10 or base 16.

##### Examples
```emerald
16
1.5
0xA
```

### String Literals
A string literal is zero or more characters enclosed in single or double quotation marks.

##### Examples
```emerald
'im a string'
'im also a string,

but i go on for many

many lines'
```

## Operators
Emerald has the following types of operators:

### Unary Operators
Unary operators have one operand after the operator.

| TOKEN   |Description  | Value |
| ------- |-------------|----- |
| NOT     |             |`!`   |
| SUB     |             |`-`   |
| BIT_NOT |             |`~`   |

### Binary Operators
Binary operators have two operands, one before the operator and one after.

| Token     | Value                       | Precedence | Description    |Associativity |
|-----------| ----------------------------|------------|----------------|--------------|
| ASSIGN    | `=`                         | 1          |                |RIGHT         |
| LOGIC_OR  | <code>&#124;&#124;</code>   | 2          |                |LEFT          |
| LOGIC_AND | `&&`                        | 3          |                |LEFT          |
| BIT_OR    | <code>&#124;</code>         | 4          |                |LEFT          |
| BIT_XOR   | `^`                         | 5          |                |LEFT          |
| BIT_AND   | `&`                         | 6          |                |LEFT          |
| EQ        | `==`                        | 7          |                |LEFT          |
| NEQ       | `!=`                        | 7          |                |LEFT          |
| LT        | `<`                         | 8          |                |LEFT          |
| GT        | `>`                         | 8          |                |LEFT          |
| LTE       | `<=`                        | 8          |                |LEFT          |
| GTE       | `>=`                        | 8          |                |LEFT          |
| SHL       | `<<`                        | 9          |                |LEFT          |
| SHR       | `>>`                        | 9          |                |LEFT          |
| ADD       | `+`                         | 10         |                |LEFT          |
| SUB       | `-`                         | 10         |                |LEFT          |
| MUL       | `*`                         | 11         |                |LEFT          |
| DIV       | `/`                         | 11         |                |LEFT          |