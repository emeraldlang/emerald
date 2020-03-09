# Language Reference
This section describes the syntax and semantics of the language.

## Declaring Variables
The `let` keyword is used to declare variables. If you do not declare a variable before
its usage, the compiler will complain and display an error message.
```emerald
let hello = 'world!'
```

It is important to note that blocks do not create a new scope, for example:
```emerald
if True then
    let x = 5
end

core.print(x) # 5, no error
```

All variables are mutable, however, the `const` keyword may be added
in the near future to support compile time checking.

## Creating Functions
The `def` keyword is used to define a function. `def` is followed by an
identifier and the `end` keyword is used to indicate that the end of the
function body.

```emerald
def my_func
end
```

If you want your function to take arguments:
```emerald
def my_func : arg1, arg2
end
```

You can also specify default arguments:
```emerald
def my_func : arg1, arg2=5
end
```

If if you want to call the function:
```emerald
my_func(5, 6)
```
In this instance, the body of our function does not contain a `return`
statement, so it will return `None`.
If you want your function to return a result:
```emerald
def my_func : arg1, arg2
    return arg1 + arg2
end
```

Functions are first class objects, this means that functions can
be passed as arguments and returned by other functions:
```emerald
def cb
    # something here
end

def my_func : cb
    # do something
    cb()
end
```

## Native Types

### Object

#### Example
```emerald
{ 5: 6, 'hello': 6, [4 + 4]: 'hey' }
```

#### Methods
- `__eq__ : other`  
Tests whether the `Object` is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the `Object` is not equal to other. Invoked when used with the `!=` operator.
- `__str__`  
Returns a `String` representation of the `Object`.
- `__boolean__`  
Returns a `Boolean` representation of the `Object`.
- `__clone__`  
Creates and returns a `Object`.
- `__init__`  
Initializes the `Object`.
- `__keys__`  
Returns an `Array` of all of the `Object`'s property names.
- `get_prop : key`  
Returns the value for the property.
- `set_prop : key, val`  
Sets the property to the provided value.

### Array

#### Example
```emerald
[1, 'hello world', [1, 2, 3]]
```

#### Methods
- `__eq__ : other`  
Tests whether the array is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the array is not equal to other. Invoked when used with the `!=` operator.
- `at : i`  
Returns the *i*th element of the array.
- `front`  
Returns the first element of the array.
- `back`  
Returns the last element of the array.
- `empty`  
Tests whether the array is empty.
- `size`  
Returns the size of the array.
- `clear`  
Clears the contents of the array.
- `push : ...args`  
Appends args to the array.
- `pop`  
Removes and returns the last element of the array.

### Boolean
`True` and `False`

#### Methods
- `__eq__ : other`  
Tests whether the boolean is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the boolean is not equal to other. Invoked when used with the `!=` operator.

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
- `__neq__`  
Returns a negation of the number. Invoked when used with the `-` unary operator.
- `__add__ : other`  
Returns the sum of the number and other. Invoked when used with the `+` operator.
- `__sub__ : other`  
Returns the difference of the number and other. Invoked when used with the `-` operator.
- `__mul__ : other`  
Returns the product of the number and other. Invoked when used with the `*` operator.
- `__div__ : other`  
Returns the quotient of the number and other. Invoked when used with the `/` operator.
- `__mod__ : other`  
Returns the remainder of number `/` other. Invoked when used with the `%` operator.
- `__iadd__ : other`  
Adds other to the number in place. Invoked when used with the `+=` operator.
- `__isub__ : other`  
Subtracts other from the number in place. Invoked when used with the `-=` operator.
- `__imul__ : other`  
Multiplies other by the number in place. Invoked when used with the `*=` operator.
- `__idiv__ : other`  
Divides other by the number in place. Invoked when used with the `/=` operator.
- `__imod__ : other`  
Modulo of the number and other in place. Invoked when used with the `%=` operator.
- `__eq__ : other`  
Tests whether the number is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the number is not equal to other. Invoked when used with the `!=` operator.
- `__lt__ : other`  
Tests whether the number is less than other. Invoked when used with the `<` operator.
- `__gt__ : other`  
Tests whether the number is greater than other. Invoked when used with the `>` operator.
- `__lte__ : other`  
Tests whether the number is equal to or less than other. Invoked when used with the `<=` operator.
- `__gte__ : other`  
Tests whether the number is equal to or greater than other. Invoked when used with the `>=` operator.
- `__bit_or__ : other`  
Performs a bitwise or on number and other. Invoked when used with the `|` operator.
- `__bit_xor__ : other`  
Performs a bitwise xor on number and other. Invoked when used with the `^` operator.
- `__bit_and__ : other`  
Performs a bitwise and on number and other. Invoked when used with the `&` operator.
- `__bit_shl__ : other`  
Shifts the bits of the number to the left by other. Invoked when used with the `<<` operator.
- `__bit_shr__ : other`  
Shifts the bits of the number to the right by other. Invoked when used with the `>>` operator.

### String
Strings can be surrounded by single or double quotes.

#### Example
```emerald
'hello world'
"hello world"
```

#### Methods
- `__add__ : other`  
Returns the concaentation of the `String` and other. Invoked when used with `+` operator.
- `__eq__ : other`  
Tests whether the `String` is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the `String` is not equal to other. Invoked when used with the `!=` operator.
- `__lt__ : other`  
Tests whether the `String` is less than other. Invoked when used with the `<` operator.
- `__gt__ : other`  
Tests whether the `String` is greater than other. Invoked when used with the `>` operator.
- `__lte__ : other`  
Tests whether the `String` is equal to or less than other. Invoked when used with the `<=` operator.
- `__gte__ : other`  
Tests whether the `String` is equal to or greater than other. Invoked when used with the `>=` operator.
- `empty`  
Tests wheter the `String` is empty.
- `length`  
Returns the number of characters in the `String`.
- `at : i`  
Returns the *i*th character in the `String`.
- `back`  
Returns the last character in the `String`.
- `front`  
Returns the first character in the `String`.
- `compare : other`  
Compares the `String` with other. Returns 1 if the `String` is greater than other, 0 if they are equal and -1 otherwise.
- `find : substr`  
Returns the position of the first character of the match.
- `format : ...args`  
Returns the formatted `String` using args.
- `substr : pos, len`  
Returns a substring spanning from *pos* to *pos + len*.
- `append : str`  
Appends the provided value to the end of the `String`.
- `isalpha`  
Returns a `Boolean` indicating whether the string only contains only alphabetic characters.
- `isalnum`  
Returns a `Boolean` indicating whether the string only contains alphabetic or numeric characters.
- `isnum`  
Returns a `Boolean` indicating whether the string only contains alphabetic or numeric characters.

## Conditionals
`if` statements in Emerald have a simple syntax:
```emerald
if some_expression then

end
```

To support multiple cases:
```emerald
if some_expression then

else if some_expression then

end
```

Or if you want to the case when all expressions evaluate to  `False`:
```emerald
if some_expression then

else if some_expression then

else

end
```

## Looping
Emerald supports `while`, `do while` and `for` loops.

```emerald
while some_expression do

end
```

```emerald
do

end while some_expression
```

`for` loops come in two different flavors:

This `for` loop can be used to either increment (`to` keyword) or decrement (`downto` keyword) to some number:
```emerald
for let i = 0 to 10 do

end

for let i = 10 downto 0 do

end
```

You can also specify the step with the `by` keyword:
```emerald
for let i = 0 to 10 by 2 do

end

for let i = 10 downto 0 by -2 do

end
```

The other type of `for` loop is for iterating over objects that implement the `Iterator` protocol.
An object implements the `Iterator` protocol if it satifies the following requirements:
- Implements a `__cur__` method.
- Implements a `__next__` method.
- Implements a `__done__` method.

To return an `Iterator` for an object, implement the `__iter__` method. This should return an
object that implements the `Iterator` protocol.

The interpreter will invoke the `__iter__` method on the `Array` object and returns an
`ArrayIterator`, which is an object that implements the `Iterator` protocol.
```emerald
for let i in [1, 2, 3] do

end
```

## Self
`self` is a keyword and not a variable. Its value is equal to the object it belongs to.
```emerald
import core

core.print(self) # <module ...>

def some_func
    core.print(self)
end

let x = {
    s: some_func
}

some_func() # <module ...>, belongs to the module
x.s() # <object> (x), belongs to x
```

## Objects and Inheritance
Emerald uses Prototypal based inheritance. The syntax to `clone` an object is quite simple:
```emerald
let Y = {
    some_prop: 'hello'
}

let X = clone Y
```

If we were to now call `core.super(X)` it would return `Y`. A subsequent call would return
`core.Object` and finally `None` to end the chain.

Because `X` inherits from `Y`, if you access `prop`, it will return the string 'hello'.

Emerald also includes a simpler syntax to add more properties to `X`:
```emerald
object X clones Y
    let some_prop = 'world'

    def some_method
        return 'hello ' + self.some_prop
    end

    prop some_other_prop
        get
            return 'hello {0}!'.format(self.some_prop)
        end
        set
            self.some_prop = '{0}!'.format(self.some_prop)
        end
    end
end
```

The `prop` statement is used to define properties with a getter and setter. The setter
receives the implicit argument `value`, this is the value that the property has been
assigned.

## Modules
