# TAP - Table Access Program

A utility to read/write values in tables over a common protocol.
There are 4 tables, indexed from 0 and each table supports 2 attributes.
The value of each attribute is any value in the range `0 .. 9`

The following commands are supported:

 * `GET <table> <attr>` - retreives a single attribute
 * `SET <table> <attr> <value>` - assign `<value>` to `<attr>` in `<table>`
 * `RESET <table>` - Set the value of all ttributes to `0`
 * `QUIT` - Exit the program


## Compiling

Compile with GNU Make / GCC:

```
$ make
```

## Running

```
$ dist/tap

GET 1 0
ATTR 1
SET 1 0 9
```
