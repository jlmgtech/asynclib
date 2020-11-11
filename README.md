# asynclib
An async/await library for C

## TODO

* async and await macros (we're so close to PoC!)
* ensure support for windows, IOS, Linux, Android, MacOS, x86, ARM (basically, all major platforms)
* garbage strategy for promises (perhaps using the number of "then" callbacks as a reference counter? Remove if it has been resolved AND has no more callbacks perhaps)
* make generators work more like generators found in python, javascript, php, etc...
* not everything should be malloc'd.  Switch to initializers instead of allocators.  This way, the user can put things on the stack.
* find some sane way of simulating closures for convenience.  Generators might work as a stand-in for encapsulating scope, but they might also be expensive.
* allow user to specify stack size for generators.
