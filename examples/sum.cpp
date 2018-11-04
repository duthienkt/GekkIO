#include <cstdio>
#include <cstdlib>
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

long long sum(int from, int to){
    long long res = 0;
    while(from<= to) {
        res +=from;
        ++from;
    };
    return res;
}

void onMessCallback(void *data, int size)
{
    Task *tsk = (Task *)data;
    Result rslt;
    rslt.id = tsk->id;
    rslt.sum = sum(tsk->from, tsk->to);
    gkio_send("result", &rslt, sizeof(Result));
}
/***
 * Test in console :
 * 
 * [task](AgAAAAFlzR2AF7Qs)
 * 
 * */

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