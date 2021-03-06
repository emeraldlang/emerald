# Modules
This section describes the modules that are built into the language.

## bytecode
This module contains functions to view the bytecode of functions.

### *function* bytecode
Returns a `BytecodeIterator` object for the supplied function.

#### Arguments
- `f`  
The function.

#### Example
```emerald
import bytecode
import core

def some_func
    let x = 'hello'
    let y = 5
end

for let opcode in bytecode.bytecode(some_func) do
    core.print(opcode.opname)
end
```

### *object* BytecodeIterator
Iterator object for iterating over the bytecode of a function.

#### Methods
- `__clone__`  
Creates and returns a `BytecodeIterator` object.
- `__init__ : f`  
Initializes the `BytecodeIterator` with the supplied function `f`.
- `__cur__`  
Returns the current element, see [Bytecode Object Properties](#bytecode-object-properties) for the properties available on the returned `Object`.
- `__next__`  
Returns the next element, see [Bytecode Object Properties](#bytecode-object-properties) for the properties available on the returned `Object`.
- `__done__`  
Indicates whether the iterator has iterated through all elements.

#### Bytecode Object Properties
- `op`  
Numeric code for the op.
- `opname`  
The name of the operation.
- `args`  
`Array` of numberic arguments.

## collections
This module contains objects that store a collection of objects.

### *object* Queue
A first in, first out collection of objects.

#### Methods
- `__eq__ : other`  
Tests whether the `Queue` is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the `Queue` is not equal to other. Invoked when used with the `!=` operator.
- `__clone__`  
Creates and returns a `Queue` object.
- `peek`  
Returns the object at the beginning of the `Queue`.
- `dequeue`  
Removes and returns the object at the beginning of the `Queue`.
- `enqueue : item`  
Adds an object to the end of the `Queue`.
- `empty`  
Returns a `Boolean` that indicates whether the `Queue` has no elements.
- `size`  
Returns the number of elements in the `Queue`.

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

### *object* Set
A unique and unordered collection of objects.

#### Methods
- `__eq__ : other`  
Tests whether the `Set` is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the `Set` is not equal to other. Invoked when used with the `!=` operator.
- `__clone__`  
Creates and returns a `Set` object.
- `add : item`  
Adds an element to the `Set`.
- `contains : item`  
Returns a `Boolean` that indicates whether the `Set` contains a specific element.
- `remove : item`  
Removes the specified item from the `Set`.
- `empty`  
Returns a `Boolean` that indicates whether the `Set` has no elements.
- `size`  
Returns the number of elements in the `Set`.


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

### *object* Stack
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

#### Methods
- `__eq__ : other`  
Tests whether the `Stack` is equal to other. Invoked when used with the `==` operator.
- `__neq__ : other`  
Tests whether the `Stack` is not equal to other. Invoked when used with the `!=` operator.
- `__clone__`  
Creates and returns a `Stack` object.
- `peek`  
Returns the object at the top of the `Stack`.
- `pop`  
Removes and returns the object at the top of the `Stack`.
- `push : item`  
Adds an object to the top of the `Stack`.
- `empty`  
Returns a `Boolean` that indicates whether the `Stack` has no elements.
- `size`  
Returns the number of elements in the `Stack`.

## core
This module contains fundamental and commonly used functions and objects.

### *function* extend
Merges the properties of the objects into the first one.

#### Arguments
- `target`  
The object to merge properties into.
- `sources`  
The objects to merge.

### *function* str
Returns a `String` version of the supplied object.

#### Arguments
- `obj`  
The object to convert to a `String`.

### *function* bool
Returns a `Boolean` version of the supplied object.

#### Arguments
- `obj`  
The object to convert to a `Boolean`.

### *function* range
Returns an `Array` of numbers from 0 to `n`.

#### Arguments
- `n`  
Number of elements.

### *function* super
Returns the parent of the provided object.

#### Arguments
- `obj`  
The object.

### *function* iter
Returns an iterator object for the provided object.

#### Arguments
- `obj`  
The object.

### *function* cur
Returns the current element of the provided iterator.

#### Arguments
- `iter`  
The iterator.

### *function* done
Returns a `Boolean` indicating whether the iterator has iterated through all elements.

#### Arguments
- `iter`  
The iterator.

### *function* next
Returns the next element of the provided iterator.

#### Arguments
- `iter`  
The iterator.

### *function* print
Prints the provided objects to console.

#### Arguments
- `...objs`  
The objects to print to console.

## datetime
The module contains functions and objects for working with dates and times.

### *object* Date
Object that represents a date.

#### Methods
- `__iadd__ : days`  
Adds days to the `Date` in place. Invoked when used with the `+=` operator.
- `__isub__ : days`  
Subtracts days from the `Date` in place. Invoked when used with the `-=` operator.
- `__clone__`  
Creates and returns a `Date` object.
- `__init__`  
Initializes the `Date` object.

#### Properties
- *get* `year`  
The year part of the date.
- *get* `month`  
The month part of the date.
- *get* `day`  
The day part of the date.
- *get* `day_of_week`  
The day of the week (Monday, Thursday, etc).
- *get* `day_of_year`  
The day of the year (1 to 366).
- *get* `end_of_month`  
A `Date` object that set to the last day of this `Date`'s month.

### Example
```emerald
import core
import datetime

let date = clone datetime.Date(2020, 3, 20)
core.print(date) # 2020-Mar-20
```

### *object* TimeDuration
Object that represents a length of time.

#### Methods
- `__add__ : other`  
Returns a new `TimeDuration` that adds the value of other to this `TimeDuration`. Invoked when used with the `+` operator.
- `__sub__ : other`  
Returns a new `TimeDuration` that subtracts the value of other from this `TimeDuration`. Invoked when used with the `-` operator.
- `__iadd__ : other`  
Adds other to the `TimeDuration` in place. Invoked when used with the `+=` operator.
- `__isub__ : other`  
Subtracts other from the `TimeDuration` in place. Invoked when used with the `-=` operator.
- `__clone__`  
Creates and returns a `TimeDuration` object.
- `__init__`  
Initializes the `TimeDuration` object.

#### Properties
- *get* `hours`  
The number of normalized hours.
- *get* `minutes`  
The number of normalized minutes.
- *get* `seconds`  
The number of normalized seconds.
- *get* `milliseconds`  
The number of milliseconds.
- *get* `total_seconds`  
The total number of seconds.
- *get* `total_milliseconds`  
The total number of seconds.

### Example
```emerald
import core
import datetime

let duration = clone datetime.TimeDuration(1, 20, 33, 543)
core.print(duration) # 01:20:33.543
```

### *object* Time
Object that represents a point in time.

#### Methods
- `__iadd__ : time`  
Adds the `TimeDuration` time to the `Time` in place. Invoked when used with the `+=` operator.
- `__isub__ : time`  
Subtracts the `TimeDuration` time from the `Time` in place. Invoked when used with the `-=` operator.
- `__clone__`  
Creates and returns a `Time` object.
- `__init__`  
Initializes the `Time` object.

#### Properties
- *get* `date`  
The date (see [Date](#object-date)).
- *get* `time_of_day`  
The time of day (see [TimeDuration](#object-timeduration)).

### Example
```emerald
import core
import datetime

let date = clone datetime.Date(2020, 3, 20)
let time_of_day = clone datetime.TimeDuration(1, 20, 33, 543)
let time = clone datetime.Time(date, time_of_day)
core.print(time) # 2020-Mar-20 01:20:33.543
```

### *function* universal_time
Gets the UTC time (see [Time](#object-time)).

### *function* local_time
Gets the local time (see [Time](#object-time))

## gc
This module contains functions for garbage collection.

### *function* collect
Runs garbage collection.

### *function* total_allocated_objects
Returns the number of managed objects.

### *function* threshold
Returns the object count at which garbage collection will run.

### *function* set_threshold
Sets the garbage collection threshold.

#### Arguments
- `threshold`  
The threshold.

## http
This module contains objects for working with http.

### *object* HttpVerb

#### Properties
- `GET`
- `POST`

### *object* HttpClient
An object used to send HTTP requests and receive HTTP responses.

### Methods
- `GET : host, url, headers={}`  
Sends a GET to request to the specified host and url.
- `POST : host, url, headers={}`  
Sends a POST to request to the specified host and url.
- `send : verb, host, url, headers={}`  
Sends a request to the specified host and url.

## json
This module contains objects and functions for working with json.

### *object* TokenType

#### Properties
- `LBRACE`
- `RBRACE`
- `LBRACKET`
- `RBRACKET`
- `COMMA`
- `COLON`
- `STRING_LITERAL`
- `TRUE_LITERAL`
- `FALSE_LITERAL`
- `NULL_LITERAL`
- `ILLEGAL:`
- `EOSF:`

### *object* Tokenizer

#### Methods
- `__init__ : json`  
Initializes the `Tokenizer` with the provided JSON string.
- `scan`  
Returns the next token.

### *object* ParseError

### *object* Parser

#### Methods
- `__init__ : json`  
Initializes the `Parser` with the provided JSON string.
- `parse`  
Parses the JSON and returns an object.

### *function* deserialize 
Deserializes the string to an object.

#### Arguments
- `json`  
The JSON string to deserialize.

#### Example
```emerald
import core
import json

let obj = json.deserialize('{"test": "hello"}')
core.print(obj.test) # 'hello'
```

## io
This module contains objects used for io.

### *object* FileStream
An object used for read and write operations on a file.

#### Methods
- `__clone__`  
Creates and returns a `FileStream` object.
- `open : filename, access`  
Opens a file with the specified filename and access, see [FileAccess](#object-fileaccess).
- `is_open`  
Returns a `Boolean` indicating whether the file is open.
- `read : n=None`  
Reads `n` chracters from the file, if `n` is `None`, it will read the entire file.
- `readline`  
Reads a line from the file.
- `write : s`  
Writes the contents of `s` to the file.

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

### *object* StringStream
An object used for read and write operations on a string.

#### Methods
- `__clone__`  
Creates and returns a `StringStream` object.
- `read : n`  
Reads `n` chracters from the stream.
- `readline`  
Reads a line from the stream.
- `write : s`  
Writes the contents of `s` to the stream.

### *object* FileAccess

#### Properties
- `read`
- `write`
- `readwrite`

## net
This module contains objects for networking.

### *object* IPAddress
An object that represents that an IP address.

### Methods
- `__init__ : ip`  
Initializes the `IPAddress` with the provided string.
- `__clone__`  
Creates and returns a `IPAddress` object.
- `is_loopback`  
Returns a `Boolean` that indicates if the `IPAddress` is a loopback address.
- `is_multicast`  
Returns a `Boolean` that indicates if the `IPAddress` is a multicast address.
- `is_unspecified`  
Returns a `Boolean` that indicates if the `IPAddress` is unspecified.
- `is_ipv4`  
Returns a `Boolean` that indicates if the `IPAddress` is ipv4.
- `is_ipv6`  
Returns a `Boolean` that indicates if the `IPAddress` is ipv6.

### *object* IPEndpoint
An object that represents and `IPAddress` and port.

### Methods
- `__init__ : address, port`  
Initializes the `IPEndpoint` with the provided address and port.
- `__clone__`  
Creates and returns a `IPEndpoint` object.
- `get_address`  
Returns the `IPAddress` for the `IPEndpoint`.
-`get_port`  
Returns the port for the `IPEndpoint`.

### *object* TCPClient
An object for TCP connections.

### Methods
- `__clone__`  
Creates and returns a `TCPClient` object.
- `connect : endpoint`  
Connects to the specified endpoint, returns a `Boolean` that indicates whether the connection was successful.
- `read : n`
Reads `n` bytes from the socket.
- `write : s`
Writes the contents of `s` to the socket.

### Example
```emerald
import net

let endpoint = clone net.IPEndpoint(clone net.IPAddress('127.0.0.1'), 8000)
let client = clone net.TCPClient

if client.connect(endpoint) then
    client.write('hello!')
    core.print(client.read(5))
else
    core.print('failed to connect to {0}'.format(endpoint))
end
```

### *object* TCPListener
An object that listens for TCP client connections.

### Methods
- `__init__ : endpoint`  
Initializes the `TCPListener` with the provided `IPEndpoint`.
- `__clone__`  
Creates and returns a `TCPListener` object.
- `start`  
Starts listening for incoming connections.
- `stop`  
Stops listening for incoming connections.
- `is_listening`  
Returns a `Boolean` that indicates whether the `TCPListener` is listening for incoming connections.
- `accept`  
Accepts an incoming connection, returns a `TCPClient` object.
- `get_endpoint`  
Gets the `IPEndpoint` for the `TCPListener`.

### Example
```emerald
import net

let endpoint = clone net.IPEndpoint(clone net.IPAddress('127.0.0.1'), 8000)
let listener = clone net.TcpListener(endpoint)

listener.start()

let client = listener.accept()
let msg = client.read(5)

if msg == 'hello' then
    client.write('hello!')
else
    client.write('i don\'t understand')
end

listener.stop()

```

### *function* resolve
Resolves a DNS hostname, returns an array of `IPAddress`.

## process
This module contains functions for process creation and interprocess communication.

### *function* create
Creates a new process.

### Arguments
- `f` - The callable object.
- `...args` - The arguments to pass to `f`.

### Example
```emerald
import process

def some_work
    # do something
end

let pid = process.create(some_work)
```

### *function* id
Returns the id of the current process.

### *function* join
The function returns when the process identified by `pid` completes execution.

#### Arguments
- `pid`
The id of the process to join.

### *function* receive
Dequeues a message from the current process' mailbox, it will
block if there are no mesages.

### *function* send
Enqueues a message in the mailbox of the process specified by
the provided `pid`.

#### Arguments
- `pid`  
The id of the process to send to.
- `msg`  
The message to send to the process.

### *function* sleep
Blocks execution of the current process for the specified `duration` in
seconds.

#### Arguments
- `duration`
The number of seconds to sleep for.

### *function* state
Returns the state for the process identified by `pid`, see [States](#object-states).

#### Arguments
- `pid`
The id of the process to get the state for.

### *object* States

#### Properties
- `pending`
- `running`
- `completed`
