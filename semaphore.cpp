#include <semaphore.h>

semaphore::semaphore(int p){
        permit = p;
    }
    
void semaphore::wait(){
    unique_lock<mutex> lck(mx);
    while(permit <= 0){
        cv.wait(lck);
    }
    permit--;    
}
    
void semaphore::post(){
    unique_lock<mutex> lck(mx);
    permit++;
    cv.notify_one();
}
