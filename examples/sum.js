const { GekkIO, GKIOLiteral, GKIOStruct } = require('../');
const EventEmitter = require('events');

var Task = GKIOStruct()
    .property(GKIOLiteral.Int32, "id")
    .property(GKIOLiteral.Int32, "a")
    .property(GKIOLiteral.Int32, "b")
    .declare();

var Result = GKIOStruct()
    .property(GKIOLiteral.Int32, "id")
    .property(GKIOLiteral.Int64, "sum")
    .declare();


var SumWorkers = Array(4).fill(null).map(_ => new GekkIO({script:'./sum', debug:true}));
var ResultEmitor = new EventEmitter();

SumWorkers.forEach(worker => {
    // worker.debug = true;
    worker.on('result', data => {
        var mResult = Result.fromBuffer(data);
        ResultEmitor.emit('result_' + mResult.id.value, mResult.sum.value);
    });
});

function sum0(a, b) {
    return new Promise((rs, rj) => {
        var res = 0;
        while (a <= b) {
            res += a;
            ++a;
        }
        rs(res);
    });
}

var uid = 0;
function sum1(a, b) {
    var d = (b - a + 1) >> 2;
    var ranges = [[a, a + d], [a + d + 1, a + 2 * d], [a + 2 * d + 1, a + 3 * d], [a + 3 * d + 1, b]];
    var tasks = ranges.map((range, i) => new Promise((rs, rj) => {
        SumWorkers[i].send('task', Task({ id: uid, a: range[0], b: range[1] }).toBuffer());
        ResultEmitor.once('result_' + uid, sum => rs(sum));
        uid++;

    }));

    return Promise.all(tasks).then(results => {
        return results[0] + results[1] + results[2] + results[3];
    });
}


function Test(mess, func, a, b) {
    var now = new Date().getTime();
    return func(a, b).then(res => {
        console.log(mess);
        console.log("Time :", new Date().getTime() - now + '(ms)', "Result :", res);
    })
}

console.log("Expected :", 500000000500000000);
Test("Javascript", sum0, 0, 1000000000);
Test("C++", sum1, 0, 1000000000).then(_ => {
    SumWorkers.forEach(worker => worker.dispose());
}
);



