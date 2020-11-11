function resolver(data) {
    const iteration = data.gen.next(data.value);
    if (!iteration.done) {
        iteration.value.then(resolver.bind(null, data));
    }
}

function _async(func) {
    return new Promise((res, rej) => {
        const gen = func();
        resolver({
            gen: gen,
            value: null,
        });
    });
}

function loadRecord() {
    return new Promise((res) => setTimeout(res, 1000));
}

const loadRecords = _async(function*() {
    for (let i = 0; i < 10; i++) {
        yield loadRecord();
        console.log("loaded %d records...", i+1);
    }
    console.log("done");
});

function main_finish(result) {
    console.log("received records: ", result);
}

function main() {
    loadRecords().then(main_finish);
}

main();
