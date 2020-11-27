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
