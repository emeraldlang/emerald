# Statements

Compound statements are statements composed of many compound or
simple statements.

## Do Statements
Used to limit the scope of variables.

##### Syntax
```
do_statement = "do" , block , "end";;
```

##### Example
```emerald
do
  let x = 1 # i can only be accessed in this block!
  print x # 1
end
print x # error!
```

## Functions
A named set of callable instructions. Functions are first class objects, which means
they themselves can be assigned to variables and passed as parameters to other functions.

##### Syntax
```
function_statement = "def" , IDENT , parameter_list , block , "end";
```

##### Example
```emerald
def my_function : param1, param2
  let res = param1 + param2
  return res
end
```

# If Statement
Used for conditional execution of a set of instructions.

##### Syntax
```
ite_statement = "if" , expression , "then" , block , "end" | ("else" , block , "end") | ("else" , ite_statement);
```

##### Example
```emerald
let a = 5
let b = 7
if a > b then
    print 'a is greater than b'
else if a < b then
    print 'a is less than b'
else
    print 'a and b are equal'
end
```

# While Loops
Used to repeat the execution of a set of instructions as long as an expression is true.

##### Syntax
```
while_statement = "while" , expression , "do" , block , "end";
```

##### Example
```emerald
let i = 0
while i < 10 do
    i = i + 1
end
```

# For Loops
A syntatic sugar for looping.

##### Syntax
```
for_statement = "for" , declaration , "to" , NUMBER , [ "by" , NUMBER ] , "do" , block , "end";
```

##### Example
```emerald
for let i = 0 to 10 do
  print i
end
```
