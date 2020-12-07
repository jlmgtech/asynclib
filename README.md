# asynclib
https://nullprogram.com/blog/2019/03/28/
^^ for windows support alternative to makecontext

An async/await library for C, inspired by JS concurrency model

## FEATURES

* async and await
* promises
* generators
* event loop
* timeout callbacks
* convenient manual reference counting
* event emitters (modelled after JS event patterns)
* hashmap and dynamic array convenience types
* a usable alternative to closures and partial application (think "bind" in javascript)

## USABILITY

Even though I've written many tests, used extended static linters, and analyzed each test with valgrind, you can't know about all possible bugs until a program is distributed in real production environments.  I would say, for this reason, this library is not yet production ready.  There are a great many efficiency improvements I want to add, but currently this exists as a sort of proof-of-concept.  Use at your own risk!

## ROADMAP

* use region-based allocation strategies for the basic data structures
* implement a general purpose region library for memory management.
* async and await macros (we're so close to PoC!)
* ensure support for windows, IOS, Linux, Android, MacOS, x86, ARM (basically, all major platforms)
* make generators work more like generators found in python, javascript, php, etc...
* reduce heap allocation everywhere you can.
* allow user to specify stack size for generators.
* write documentation and tutorials of building a concurrent app using the asynclib

## INSTALLATION
todo

## EXAMPLES
todo

## API REFERENCE
todo

## GETTING STARTED
todo

## TUTORIALS
todo

## PROJECTS USING ASYNCLIB
todo
