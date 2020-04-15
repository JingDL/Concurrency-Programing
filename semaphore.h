#ifndef __SEMAPHORE_H__
#define __SEMAPHORE_H__
#include <mutex>
#include <condition_variable>
using namespace std;

class semaphore{
public:
    semaphore(int p);
    void wait();
    void post();
    
private:
    mutex mx;
    condition_variable cv;
    int permit;
};
#endif // #ifndef __SEMAPHORE_H__
