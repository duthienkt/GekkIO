## Nodejs call C++

Define your struct in nodejs

```js
const { GekkIO, GKIOLiteral, GKIOStruct } = require('gekkio');

var Task = GKIOStruct()
    .property(GKIOLiteral.Int32, "id")
    .property(GKIOLiteral.Int32, "a")
    .property(GKIOLiteral.Int32, "b")
    .declare();

var Result = GKIOStruct()
    .property(GKIOLiteral.Int32, "id")
    .property(GKIOLiteral.Int64, "sum")
    .declare();

```

Define in C++

```c++
#include "gekkio.h"

// be careful, without pack will make bug
#pragma pack(1)
struct Task
{
    int id;
    int from;
    int to;
} ;

struct Result{
    int id;
    long long sum;
} ;
#pragma pack()
```


Handler task in C++

```c++
void onMessCallback(void *data, int size)
{
    Task *tsk = (Task *)data;

    //process task
    Result rslt;
    rslt.id = tsk->id;
    rslt.sum = sum(tsk->from, tsk->to);
    
    //send result
    gkio_send("result", &rslt, sizeof(Result));
}
```


Init and wait for task

```c++

int main()
{
    gkio_init(1024);
    gkio_on("task", &onMessCallback);
    int i = 0;
    while (true)
    {
        gkio_loop();
    }
    return 0;
}
```

Send task to C++ processor in nodejs

```js
SumWorkers[i].send('task', Task({ id: uid, a: range[0], b: range[1] }).toBuffer());
```

Listen result from C++ processor

```js
SumWorkers[i].on('result', data => {
        var mResult = Result.fromBuffer(data);
        ResultEmitor.emit('result_' + mResult.id.value, mResult.sum.value);
});

```

