//class ASYNC {
//    constructor(func) {
//        this.async_routine = func();
//        this.master_promise = new Promise((resolve) => {
//            this.resolve = resolve;
//        });
//    }
//
//    iterate(awaited_result) {
//        const iteration = this.async_routine.next(awaited_result);
//        if (iteration.done) {
//            this.resolve(iteration.value);
//        } else {
//            const awaited_promise = iteration.value;
//            awaited_promise.then((awaited_result) => {
//                this.iterate(awaited_result);
//            });
//        }
//        return this.master_promise;
//    }
//}

//function _async(func) {
//const a = new ASYNC(func);
//return a.iterate(null);
//}

function loadRecord() {
    return new Promise((resolveRecord) => {
        setTimeout(resolveRecord, 500);
    });
}

async function loadRecords() {
    let i;
    for (i = 0; i < 5; i++) {
        await loadRecord();
        console.log("loaded %d records...", i+1);
    }
    console.log("done");
    return i;
}

setTimeout(() => {
    console.log("Hello world 1");
}, 1700);

setTimeout(() => {
    console.log("Hello world 2");
}, 1000);

loadRecords().then((result) => {
    console.log("received records: %d", result);
});
