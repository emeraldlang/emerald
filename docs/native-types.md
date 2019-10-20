# Native Types

This section describes the types that are built into the interpreter.

### Array

#### Example
```emerald
[1, 'hello world', [1, 2, 3]]
```

#### Methods
- `__eq__ : self, other`
    - Tests whether the array is equal to other. Invoked when used with the `==` operator.
- `__neq__ : self, other`
    - Tests whether the array is not equal to other. Invoked when used with the `!=` operator.
- `at : self, i`
    - Returns the *i*th element of the array.
- `front : self`
    - Returns the first element of the array.
- `back : self`
    - Returns the last element of the array.
- `empty : self`
    - Tests whether the array is empty.
- `size : self`
    - Returns the size of the array.
- `clear : self`
    - Clears the contents of the array.
- `push : self, ...args`
    - Appends args to the array.
- `pop : self`
    - Removes and returns the last element of the array.

### Boolean
`True` and `False`

#### Methods
- `__eq__ : self, other`
    - Tests whether the boolean is equal to other. Invoked when used with the `==` operator.
- `__neq__ : self, other`
    - Tests whether the boolean is not equal to other. Invoked when used with the `!=` operator.

### None
`None`

### Number
Numbers can be expressed in either base 10 or base 16.

#### Example
```emerald
16
1.5
0xA # or 0XA
```

#### Methods
- `__neq__ : self`
    - Returns a negation of the number. Invoked when used with the `-` unary operator.
- `__add__ : self, other`
    - Returns the sum of the number and other. Invoked when used with the `+` operator.
- `__sub__ : self, other`
    - Returns the difference of the number and other. Invoked when used with the `-` operator.
- `__mul__ : self, other`
    - Returns the product of the number and other. Invoked when used with the `*` operator.
- `__div__ : self, other`
    - Returns the quotient of the number and other. Invoked when used with the `/` operator.
- `__mod__ : self, other`
    - Returns the remainder of number `/` other. Invoked when used with the `%` operator.
- `__iadd__ : self, other`
    - Adds other to the number in place. Invoked when used with the `+=` operator.
- `__isub__ : self, other`
    - Subtracts other from the number in place. Invoked when used with the `-=` operator.
- `__imul__ : self, other`
    - Multiplies other by the number in place. Invoked when used with the `*=` operator.
- `__idiv__ : self, other`
    - Divides other by the number in place. Invoked when used with the `/=` operator.
- `__imod__ : self, other`
    - Modulo of the number and other in place. Invoked when used with the `%=` operator.
- `__eq__ : self, other`
    - Tests whether the number is equal to other. Invoked when used with the `==` operator.
- `__neq__ : self, other`
    - Tests whether the number is not equal to other. Invoked when used with the `!=` operator.
- `__lt__ : self, other`
    - Tests whether the number is less than other. Invoked when used with the `<` operator.
- `__gt__ : self, other`
    - Tests whether the number is greater than other. Invoked when used with the `>` operator.
- `__lte__ : self, other`
    - Tests whether the number is equal to or less than other. Invoked when used with the `<=` operator.
- `__gte__ : self, other`
    - Tests whether the number is equal to or greater than other. Invoked when used with the `>=` operator.
- `__bit_or__ : self, other`
    - Performs a bitwise or on number and other. Invoked when used with the `|` operator.
- `__bit_xor__ : self, other`
    - Performs a bitwise xor on number and other. Invoked when used with the `^` operator.
- `__bit_and__ : self, other`
    - Performs a bitwise and on number and other. Invoked when used with the `&` operator.
- `__bit_shl__ : self, other`
    - Shifts the bits of the number to the left by other. Invoked when used with the `<<` operator.
- `__bit_shr__ : self, other`
    - Shifts the bits of the number to the right by other. Invoked when used with the `>>` operator.

### String
Strings can be surrounded by single or double quotes.

#### Example
```emerald
'hello world'
"hello world"
```

#### Methods
- `add : self, other`
    - Returns the concaentation of the string and other. Invoked when used with `+` operator.
- `__eq__ : self, other`
    - Tests whether the string is equal to other. Invoked when used with the `==` operator.
- `__neq__ : self, other`
    - Tests whether the string is not equal to other. Invoked when used with the `!=` operator.
- `__lt__ : self, other`
    - Tests whether the string is less than other. Invoked when used with the `<` operator.
- `__gt__ : self, other`
    - Tests whether the string is greater than other. Invoked when used with the `>` operator.
- `__lte__ : self, other`
    - Tests whether the string is equal to or less than other. Invoked when used with the `<=` operator.
- `__gte__ : self, other`
    - Tests whether the string is equal to or greater than other. Invoked when used with the `>=` operator.
- `empty : self`
    - Tests wheter the string is empty.
- `length : self`
    - Returns the number of characters in the string.
- `at(self, i)`
    - Returns the *i*th character in the string.
- `back : self`
    - Returns the last character in the string.
- `front : self`
    - Returns the first character in the string.
- `compare : self, other`
    - Compares the string with other. Returns 1 if the string is greater than other, 0 if they are equal and -1 otherwise.
- `find : self, substr`
    - Returns the position of the first character of the match.
- `format : self, ...args`
    - Returns the formatted string using args.
- `substr : self, pos, len`
    - Returns a substring spanning from *pos* to *pos + len*.
