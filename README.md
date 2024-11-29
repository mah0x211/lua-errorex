# lua-error

[![test](https://github.com/mah0x211/lua-error/actions/workflows/test.yml/badge.svg)](https://github.com/mah0x211/lua-error/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/mah0x211/lua-error/branch/master/graph/badge.svg)](https://codecov.io/gh/mah0x211/lua-error)

additional features to the error module.

## Installation

```sh
luarocks install error
```


## Module Structure

```lua
local error = require('error')
```

`error` module contains the following functions and submodules;

- `__call` metamethod that equivalent to Lua's built-in `error` function
- functions of `error` module
- `error.check` submodule
- `error.fatalf` submodule
- `error.is` submodule
- `error.message` submodule
- `error.type` submodule


## `error` Module

## error( message [, level] )

calls the `__call` metamethod, which is equivalent to Lua's built-in function `error`.

```lua
local error = require('error')
error('error occurred')
```


## error.debug( mode )

forces the `traceback` argument of the error creation function to be set to `true`.

```lua
local error = require('error')
error.debug(true)
```

**Params**

- `mode:boolean`: `true` to enable the debug mode, or `false` to disable it.


## err = error.new( message [, werr [, level [, traceback]]] )

creates a new error object.

```lua
local error = require('error')
-- create new error
local err = error.new('my new error object')
print(err)
```

**Params**

- `message:error.message|any`: a message value.  
  - if it is not `error.message`, create `error.message` with that message.
- `werr:error`: `nil` or other error objects to be included in the new error object.
- `level:integer`: specifies how to get the error position. (default: `1`)
- `traceback:boolean`: get the stack traceback and keep it in the error object. (default: `false`)

**Returns**

- `err:error`: error object that contains the `__tostring` metamethod.


### Accessing the properties of an `error` object

```lua
local error = require('error')
local err = error.new('my new error object')
print(err.message)
print(err.type)
print(err.code) -- equivalent to err.type.code (default: -1)
print(err.op) -- equivalent to err.message.op
```

- `message:error.message`: the message of the `error` object.
- `type:error.type`: the type of the `error` object.
- `code:integer`: the `code` property of the `error.type` object. (default: `-1`)
- `op:string`: the `op` property of the `error.message` object.


## err = error.toerror( message [, werr [, level [, traceback]]] )

equivalent to the `error.new` function, but if the `message` is an `error` object, it will be returned.

```lua
local error = require('error')
local err = error.new('my error')
-- returns an err
print(err == error.toerror(err)) -- true
-- create new error
print(err == error.toerror('my error2')) -- false
```


## err = error.format( fmt [, ..., [, werr [, level [, traceback]]]] )

equivalent to the `error.new` function, but the `message` argument is formatted with `lua-string-format` module. And, if the `werr` is not an `error` object, it will be concatenated with the formatted message.

please see the https://github.com/mah0x211/lua-string-format for more details.

```lua
local error = require('error')
local err1 = error.format('my error: %s', 'hello')
print(err) -- ./example.lua:2: in main chunk: my error: hello
local err2 = error.format('%dnd error', 2, err1)
print(err2) -- ./example.lua:4: in main chunk: 2nd error
            -- ./example.lua:2: in main chunk: my error: hello
```


## msg, is_msg = error.cause( err )

if `err` is `error` object, return an `error.message` object associated with `err`. otherwise, return first argument.


```lua
local error = require('error')

-- just return first argument
local msg = 'my error'
local err = msg
local cause = error.cause(err)
print(cause, type(cause), cause == msg) -- my error string true

-- get a error.message of error
msg = 'my error'
err = error.new('my error')
cause = error.cause(err)
print(cause, type(cause), cause == msg) -- my error userdata false

msg = error.message.new('my error')
err = error.new(msg)
cause = error.cause(err)
print(cause, type(cause), cause == msg) -- my error userdata true
```

**Params**

- `err:any`: error value.

**Returns**

- `msg:any`: an `error.message` object associated with `err`, or an `err` argument.
- `is_msg:boolean`: `true` if the returned `msg` is `error.message`.


## err = error.unwrap( err )

return a wrapped error object.

```lua
local error = require('error')
-- create new error object
local err1 = error.new('first error')
local err2 = error.new('second error', err1)

print(err2) -- ./example.lua:4: in main chunk: second error\n./example.lua:3: in main chunk: first error

-- extract first error
local err = error.unwrap(err2)
print(err) -- ./example.lua:3: in main chunk: first error
print(err == err1) -- true
```

**Params**

- `err:error`: error object.

**Returns**

- `err:error`: a wrapped error object or `nil`.


## errt = error.typeof( err )

get the `error.type` object associated with the `error` object.

if `err` is `error.type` object, return it. or, if `err` is `error` object, get the `error.type` object associated with the `err`. otherwise, return `nil`.

```lua
local error = require('error')
-- create error object from errtype
local err1 = error.new('my error')
local errtype = error.type.new('my_error_type')
local err2 = errtype:new('my typed error')

-- get the error.type of error
print(error.typeof(err1)) -- nil
print(error.typeof(err2) == errtype) -- true
print(error.typeof(errtype)) -- my_error_type: 0x004ad8c0
print(error.typeof('hello')) -- nil
```

**Params**

- `err:any`: error value.

**Returns**

- `errt:error.type`: `error.type` object, or `nil`.


## `error.fatalf` Submodule

## error.fatalf( [level ,] fmt, ... )

throw an error with a formatted message.

- the formatting rules are the same as `error.format` function.
- if the first argument is a number, it is used as the `level` argument. 
    - the `level` represents the location on the call stack where the error occurred. (default: `1`)
- if the `fmt` argument is a string, it is used as the format string and the `...` arguments are used as the arguments of the format string. 
- if the `fmt` argument is not a string, all arguments will be converted to a string and concatenated with a space character.

**Parameters**

- `level:integer`: specifies how to get the error position. (default: `1`)
- `fmt:string`: a format string.
- `...:any`: arguments to be formatted.


## `error.is` Submodule

## err = error.is( err, target )

extract the `error` object that **strictly matches** the `target` from the `error` object.  
the target object is converted to `uintptr_t` and compared with the error object or internal value.

```lua
local error = require('error')
-- create error object
local err1 = error.new('first error')
local err2type = error.type.new('seond_error_type')
local err2 = err2type:new('second error', err1)
local err3 = error.new('third error', err2)

-- get the err1 by message
print(error.is(err3, 'first error')) -- ./example.lua:3: in main chunk: first error

-- get the err2 by error type
print(error.is(err3, err2type)) -- ./example.lua:6: in main chunk: [seond_error_type:-1] second error\n./example.lua:3: in main chunk: first error

-- get the err2 by error type name
print(error.is(err3, 'seond_error_type')) -- ./example.lua:6: in main chunk: [seond_error_type:-1] second error\n./example.lua:3: in main chunk: first error

-- it returns nil if message does not matches with any of error message
print(error.is(err3, 'unknown message')) -- nil
```

**Params**

- `err:error`: `error` object.
- `target:error|error.type|string|table`: `error` object, `error.type` object, or error message.
    - if `target` is a `string`, search for the error type by name and use it if is found.

**Returns**

- `err:error`: matched error object, or `nil`. (even if the `err` is not an error object or the `target` is not specified.)


## `error.type` Submodule

## errt = error.type.new( name [, code [, message]] )

creates a new `error.type` object.

the created `error.type` object will be kept in `the registry table` that cannot be accessed directly.  

**NOTE:** the registry table is set up as a weak reference table `__mode = 'v'`.

```lua
local error = require('error')
-- create new error.type object
local errt = error.type.new('my_error_type')
print(error.typeof(errt)) -- my_error_type: 0x09b928e8

-- error.type is registered as a weak reference
error.type.new('my_gced_type')
print(error.type.get('my_error_type')) -- my_error_type: 0x09b928e8
print(error.type.get('my_gced_type')) -- my_gced_type: 0x09b92978

collectgarbage('collect')
print(error.type.get('my_error_type')) -- my_error_type: 0x09b928e8
print(error.type.get('my_gced_type')) -- nil
```

**Params**

- `name:string`: name of the `error.type` to use for stringification.
  - length must be between `1-127` characters.
  - first character must be an `a-zA-Z` character.
  - only the following characters can be used: `a-zA-Z0-9`, `.` and `_`.
- `code:integer`: error code. (default: `-1`).
- `message:any`: error message.

**Returns**

- `errt:error.type`: `error.type` object.


### Accessing the properties of an `error.type` object

```lua
local error = require('error')
local errt = error.type.new('my_error_type')
print(errt.name) -- my_error_type
print(errt.code) -- -1 (default)
print(errt.message) -- nil
```

- `name:string`: the name of the `error.type` object.
- `code:integer`: the code of the `error.type` object.
- `message:string`: the message of the `error.type` object.


## errt = error.type.get( name )

get a `error.type` object from the registry table.

```lua
local error = require('error')
local errt = error.type.new('my_error_type')
-- get a new error.type object
print(error.type.get('my_error_type') == errt) -- true
```

**Params**

- `name:string`: name of the registered `error.type` object.

**Returns**

- `errt:error.type`: `error.type` object.


## ok = error.type.del( name )

delete a `error.type` object from the registry table.

```lua
local error = require('error')
error.type.new('my_error_type')
print(error.type.get('my_error_type')) -- my_error_type: 0x0f2477e0
-- delete error.type object from registry
print(error.type.del('my_error_type')) -- true
print(error.type.get('my_error_type')) -- nil
```

**Params**

- `name:string`: name of the registered `error.type` object.

**Returns**

- `ok:boolean`: `true` on success.


## err = errt:new( [message [, werr [, level [, traceback]]]] )

equivalent to the `error.new` function except message argument can be set to `nil`.  
it also sets the `error.type` object to the `error` object.

```lua
local error = require('error')
local errt = error.type.new('my_error_type', nil, 'my error message')
print(errt:new()) -- ./example.lua:3: in main chunk: [my_error_type:-1] my error message
print(errt:new('typed error')) -- ./example.lua:4: in main chunk: [my_error_type:-1] my error message (typed error)
```

**Returns**

- `err:error`: a new `error` object that holds the `error.type` object.


## `error.message` Submodule

## msg = error.message.new( message [, op] )

create a new structured message.

```lua
local error = require('error')
local msg = error.message.new('my message', 'my operation')
print(msg) -- [my operation] my message
```

**Params**

- `message:any`: error message.
- `op:string`: a string
- `code:integer`: error code. (default: `-1`).

**Returns**

- `msg:error.message`: a `error.message` object.

**NOTE**

if a `message` has the `__tostring` metamethod, that method called during string conversion.


### Accessing the properties of an `error.message` object

```lua
local error = require('error')
local msg = error.message.new('my error message', 'my operation')
print(msg.message) -- my_error_type
print(msg.op) -- my operation
```

- `message:string`: the message of the `error.message` object.
- `op:string`: the op of the `error.message` object.


## `error.check` Submodule

```lua
local check = require('error').check
```

The following functions checks the type of the first argument, and raises an error if the type is not correct.

| function | description |
|:---|:---|
| `check.file(v)` | checks whether a `v` is `file` or not. |
| `check.noneornil(v)` | checks whether a `v` is `none` or `nil`, or not. |
| `check.boolean(v)` | checks whether a `v` is `boolean` or not. |
| `check.pointer(v)` | checks whether a `v` is `pointer (light userdata)` or not. |
| `check.string(v)` | checks whether a `v` is `string` or not. |
| `check.table(v)` | checks whether a `v` is `table` or not. |
| `check.func(v)` | checks whether a `v` is `function` or not. |
| `check.userdata(v)` | checks whether a `v` is `userdata` or not. |
| `check.thread(v)` | checks whether a `v` is `thread` or not. |
| `check.number(v)` | checks whether a `v` is `number` or not. |
| `check.finite(v)` | checks whether a `v` is `finite number` or not. |
| `check.unsigned(v)` | checks whether a `v` is `unsigned number` or not. |
| `check.int(v)` | checks whether a `v` is `integer` or not. |
| `check.int8(v)` | checks whether a `v` is `int8` or not. |
| `check.int16(v)` | checks whether a `v` is `int16` or not. |
| `check.int32(v)` | checks whether a `v` is `int32` or not. |
| `check.int64(v)` | checks whether a `v` is `int64` or not. |
| `check.uint(v)` | checks whether a `v` is `uint` or not. |
| `check.uint8(v)` | checks whether a `v` is `uint8` or not. |
| `check.uint16(v)` | checks whether a `v` is `uint16` or not. |
| `check.uint32(v)` | checks whether a `v` is `uint32` or not. |
| `check.uint64(v)` | checks whether a `v` is `uint64` or not. |


---

## Use from C module

the `error` module installs `lua_error.h` in the lua include directory.

the following API can be used to create an error object or error type object.

**NOTE:** If the API requires arguments, pass only the index of the first argument, and the remaining arguments will be read from the position following the first index.

## void lua_error_loadlib(lua_State *L, int level)

load the lua-error module.

**NOTE:** you must call this API at least once before using the following API.

**Parameters**

- `L:lua_State*`: a lua state.
- `level:int`: specifies where to get the error position when an error occurs on loading the module.


## int lua_error_new(lua_State *L, int msgidx)

create a new error that equivalent to `error.new(message [, werr [, level [, traceback]]])` function.

**Parameters**

- `L:lua_State*`: a lua state.
- `msgidx:int`: index of the `message` argument.


## int lua_error_format(lua_State *L, int fmtidx)

create a new error that equivalent to `error.format(fmt [, ... [, werr [, level [, traceback]]]])` function.

**Parameters**

- `L:lua_State*`: a lua state.
- `fmtidx:int`: index of the `fmt` argument.


## int lua_error_new_type(lua_State *L, int nameidx)

create a new error type that equivalent to `error.type.new(name [, code [, message]])` function.

**Parameters**

- `L:lua_State*`: a lua state.
- `nameidx:int`: index of the `name` argument.


## int lua_error_new_typed_error(lua_State *L, int typeidx)

create a new typed error that equivalent to `<myerr>:new([msg [, werr [, level [, traceback]]]])` method.

**Parameters**

- `L:lua_State*`: a lua state.
- `typeidx:int`: index of the `error.type` object `myerr`.


## int lua_error_registry_get(lua_State *L, const char *name)

get a error type object from registry that equivalent to `error.type.get(name)` function.

if an error type object found, the object will be pushed on the stack and return `1`, otherwise return `0`.


## int lua_error_registry_del(lua_State *L, const char *name)

delete a error type from registry that equivalent to `error.type.del(name)` function.

if an error type object has deleted, its return `1`,  otherwise return `0`.


## int lua_error_new_message(lua_State *L, int msgidx)

create a new structured message that equivalent to `error.message.new(message [, op])` function.

