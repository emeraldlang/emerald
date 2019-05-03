# Simple Statements
Simple statements are single line statements.

## Variable Declaration
Declares a new identifier in the current scope with an optional
initial value.

##### Syntax
```
declaration = "let" , IDENT , ["=" , expression];
```

##### Example
```emerald
let hello = 'world'
```

## Print
Ouputs the result of the expression to the standard output.

##### Syntax
```
print_statement = "print" , expression , { "," , expression };
```

##### Example
```emerald
print "hello world"
```