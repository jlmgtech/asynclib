# asynclib
https://nullprogram.com/blog/2019/03/28/
^^ for windows support alternative to makecontext

An async/await library for C

## TODO

* use region-based allocation strategies for the basic data structures
* implement a general purpose region library for memory management.  (think about regions like memory for programs.. if you commpose your program of sub-programs, make sure when a sub-program exits, it cleans up all its memory)
* async and await macros (we're so close to PoC!)
* ensure support for windows, IOS, Linux, Android, MacOS, x86, ARM (basically, all major platforms)
* garbage strategy for promises (perhaps using the number of "then" callbacks as a reference counter? Remove if it has been resolved AND has no more callbacks perhaps)
* make generators work more like generators found in python, javascript, php, etc...
* not everything should be malloc'd.  Switch to initializers instead of allocators.  This way, the user can put things on the stack.
* find some sane way of simulating closures for convenience.  Generators might work as a stand-in for encapsulating scope, but they might also be expensive.
* allow user to specify stack size for generators.
* add reference-counting structure for convenience sake
* make things more "C" like (e.g. generator_t instead of Generator)



## region example?

    void do_thing() {
        REGION* rd = ropen();
        rclear(rd); // 0 initializes whole region
        person_t* people = ralloc(rd, 1024, sizeof person_t);
        int* pids = ralloc(rd, 100, sizeof int);
        person_t* me = ralloc(rd, 1, sizeof person_t);
        for (int i = 0; i < 1024; i++) {
            person_t person = &people[i];
            person->name = "johnny";
        }
        rclose(rd); // people and pids are freed.
    }

    int main() {
        do_thing();
        return 0;
    }
