#include <pthread.h>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

struct twonum {
    int from, to, counter;
    twonum(int f, int t, int c): from(f), to(t), counter(c) { }
};

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
long *sum;

// The adder thread that will add the number from 1 to n
void *adder(void * number);

int main(int argc, char *argv[])
{
    pthread_t tid, tid1, tid2, tid3; // the thread identifierier
    pthread_attr_t attr; // set of attributes for the thread
    //int pthread_mutex_init(pthread_mutex_t* mtx, const pthread_mutexattr_t *attr);
    
    // get the default attributes
    pthread_attr_init(&attr);

    twonum *p = new twonum(1,250000, 0);
    twonum *q = new twonum(250001,500000, 0);
    twonum *r = new twonum(500001,750000, 0);
    twonum *s = new twonum(750001,1000000, 0);

    // create the thread
    pthread_create(&tid,&attr,adder,(void *)p);
    pthread_create(&tid1,&attr,adder,(void *)q);
    pthread_create(&tid2,&attr,adder,(void *)r);
    pthread_create(&tid3,&attr,adder,(void *)s);

    
    // Wait for the thread to exit and save off results to sum
    long total = 0;

    pthread_join(tid,(void**)&sum);
    total = total + *sum;

    pthread_join(tid1,(void**)&sum);
    total = total + *sum;
    
    pthread_join(tid2,(void**)&sum);
    total = total + *sum;
    
    pthread_join(tid3,(void**)&sum);
    total = total + *sum;

    cout << "Total:  " << total << endl;
    delete sum;

    std::cout << "Exiting Main" << std::endl;
    pthread_mutex_destroy(&mtx);
    return 0;
}

//worker thread
#pragma warning(default:4716)
void *adder(void * number) 
{
    int i;
    long count;
    twonum *p = (twonum*)number;
    pthread_mutex_lock(&mtx);
    //cout << "Thread ID = " << pthread_self() << endl;
    cout << "Total between " << p->from << " and " << p->to << ": ";
    
    count = 0;
    for (i = p->from; i <= p->to; i++)  
    {
      count = count + i;
    }
    cout << count << endl;
    long *result = new long;
    *result = count;
    pthread_mutex_unlock(&mtx); 
    delete p;
    pthread_exit((void*)result);
}
