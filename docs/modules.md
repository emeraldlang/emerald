# Modules
This sections describes the modules that are built into the language.

## bytecode
This module contains functions to view the bytecode of functions.

### bytecode
Returns a `BytecodeIterator` object for the supplied function.

#### Arguments
- `f` - The function.

### BytecodeIterator
Iterator object for iterating over the bytecode of a function.

#### Methods
- `__clone__` - Creates and returns a `BytecodeIterator` object.
- `__init__ : f` - Initializes the `BytecodeIterator` with the supplied function `f`.

## collections
This module contains objects that store a collection of objects.

### Queue
A first in, first out collection of objects.

#### Example
```emerald
import collections

let q = clone collections.Queue
q.empty() # True
q.enqueue('hello world')
q.enqueue(5)
q.size() # 2
q.dequeue() # 'hello world'
q.size() # 1
```

### Set
A unique and unordered collection of objects.

#### Example
```emerald
import collections

let s = clone collections.Set
s.empty() # True
s.add('hello world')
s.size() # 1
s.add(5)
s.add(5)
s.size() # 2
s.contains('hello world') # True
s.remove('hello world')
s.contains('hello world') # False
```

### Stack
A last in, first out collection of objects.

#### Example
```emerald
import collections

let q = clone collections.Queue
q.empty() # True
q.push('hello world')
q.push(5)
q.size() # 2
q.pop() # 5
q.size() # 1
```

## io
This module contains objects used for io.

### FileStream
An object used for read and write operations on a file.

#### Example
```emerald
import core
import io

let file_name = 'examples/helloworld.em'
let fs = clone io.FileStream
if fs.open(file_name, io.FileAccess.read) then
    core.print(fs.read())
else
    core.print('could not open file: {0}'.format(file_name))
end
```

### StringStream
An object used for read and write operations on a string.
