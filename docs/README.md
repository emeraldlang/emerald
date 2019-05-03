# Emerald Programming Language

Emerald is an open source interpreted programming language.
It follows the procedural and object oriented programming paradigms.
The language was designed to be as simple as possible and this is
reflected in the language's grammar.

## Installation

## A Few Simple Examples

#### Hello World
```emerald
print ('hello world')
```

#### Fibonacci
```emerald
func fib : n
  return additiveSequence(n, 0, 1)
end

func additiveSequence : n, t0, t1
  if (n == 0) return t0
  if (n == 1) return t1
  return additiveSequence(n - 1, t1, t0 + t1)
end

print (fib(8))
```

#### Collatz Sequence
```emerald
func collatz : n
  var c = 1
  while (n != 1) begin
    if (n % 2 == 0)
      n = n / 2
    else
      n = 3 * n + 1
  end
  return c
end

print (collatz(13))
```