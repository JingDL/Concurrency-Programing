# Concurrency-Programing
Multithreading and Parallel Programming

#### 1. [A semaphore class for C++](https://github.com/JingDL/Concurrency-Programing/blob/master/semaphore.cpp)

Unlike Java, C++ does not have a built-in Semaphore class.  I implement one by using the C++ mutex and condition_variable. 

In the following `semaphore::wait()`, you don't see `lck.unlock()`.
```
void semaphore::wait(){
    unique_lock<mutex> lck(mx);
    while(permit <= 0){
        cv.wait(lck);
    }
    permit--;    
}
```
unique_lock follows `RAII` programming idiom.  When the function `wait()` returns, the destructor of `unique_lock` is called to unlock the lock.  This is true even if an exception happens in the function.  So I don't need to explicitly call the `unlock()` function.

Can we replace `while(permit <= 0)` with `if(permit <= 0)`?  No. We must use `while()` here to handle spurious wakeup.  When the function calls `cv.wait()`, internally it calls system call `futex()` in Linux (Other OS is similiar).  If the OS sends a signal to this thread, it can make `cv.wait()` to exit -- This is called spirous wakeup.  To handle the spurious wakeup, we must recheck the permit value one more time in the while loop.

Does the thread hold lck from the beginning to the end of `semaphore::wait()`? No. When entering `cv.wat()`, the thread atomically unclocks lck and puts itself on a waiting queue.  When notified by another thread, the thread first locks lck, and then exists from `cv.wait()`. When this thread is inside cv.wait(), anther thread calling `seamphore::wait()` won't be blocked by lck.

In `semaphore::post()`, I use `notify_one()` to notify one blocked thread for my situation.
```
void semaphore::post(){
    unique_lock<mutex> lck(mx);
    permit++;
    cv.notify_one();
}
```
In my experience, `notify_one()` is faster than `notify_all()` for most situations.  You may use `notify_all()` if it is necessary for your situation.

#### 2. [A Bounded Blocking Queue in Java](https://github.com/JingDL/Concurrency-Programing/blob/master/BoundedBlockingQueue.java)

I use an `ArrayDeque` to hold the data.  When we need a `FIFO` queue or a `Stack`, `ArrayDeque` has better performance than `LinkedList` and the built-in `Stack`.  `ArrayDeque` internally use a ring buffer to accomodate the data.  If the data out-grows its capacity, it allocates a new buffer with 2 times of the original size, and copies the data to the new buffer.  For my case, I pre-allocate the buffer for performance consideration.  The buffer relocation does not happen here.

In case that an exception happens in the fuction, we must call `unlock()` under `finally`.  This is different from C++.
```
public void enqueue(int element) throws InterruptedException {
        lock.lock();
        
        try{
            while(queue.size() == capacity){
                noSlot.await();
            }
            queue.add(element);
            noItem.signal();
        }
        finally{
            lock.unlock();
        }
    }
  ```
  
  #### 3.[A ReadWriteLock in C++](https://github.com/JingDL/Concurrency-Programing/blob/master/ReentrantReadWriteLock.cpp)
  Unlike many other readWriteLocks, which usually give readers priority over writers.  This lock gives the equal opportunities to readers and writers.


