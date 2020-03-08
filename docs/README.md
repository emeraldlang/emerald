# Emerald Programming Language
Emerald is an object oriented language that uses prototypal based
inheritance.

## Installation
```
git clone https://github.com/emeraldlang/emerald.git
cd emerald
mkdir build
cd build
conan install ..
cmake ..
make
```

## Compiling
The compile command will compile the specified source files and creates an `.emc` file.
```
./build/bin/emerald compile some_folder/some_file.em
```

## Running
The run command takes the name of the module you want to execute. It looks for
a `.emc` file so be sure to run the `compile` command before.
```
./build/bin/emerald run some_folder.some_file
```

## A Few Simple Examples

### Hello World
```emerald
import core

core.print('hello world')
```

### Fibonacci
```emerald
def fib : n
  return additive_sequence(n, 0, 1)
end

def additive_sequence : n, t0, t1
  if (n == 0) return t0
  if (n == 1) return t1
  return additive_sequence(n - 1, t1, t0 + t1)
end
```

### Collatz Sequence
```emerald
def collatz : n
  let c = 1
  while (n != 1) do
    if (n % 2 == 0) then
      n = n / 2
    else
      n = 3 * n + 1
    end
    c += 1
  end
  return c
end
```

### Inheritance
```emerald
let foo = {
  name: 'foo',
  one: 1,
  two: 2
}

let bar = clone foo
bar.two = 'two'
bar.three = 3

bar.one # 1
bar.three # 3
bar.two # 'two'
bar.name # 'foo'
foo.name # 'foo'
```
