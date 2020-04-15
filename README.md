# Concurrency-Programing
Multithreading and Parallel Programming

Unlike Java, C++ does not have a built-in Semaphore class.  I implement one by using the C++ mutex and condition_variable. 

In semaphore::wait(), you don't see lck.unlock() because unique_lock follows RAII programming idiom.  When the function wait() returns, the destructor of unique_lock is called to unlock the lock.  This is true even if an exception happens in the function.  So I don't need to explicitly call the unlock() function.

Can we replace while(permit <= 0) with if(permit <= 0)?  No. We must use while() here to handle spurious wakeup.  When the function calls cv.wait(), internally it calls system call futex() in Linux (Other OS is similiar).  If the OS sends a signal to this thread, it can make cv.wait() to exit -- This is called spirous wakeup.  To handle the spurious wakeup, we must recheck the permit value one more time in the while loop. 

In semaphore::post(), I use notify_one() to notify one blocked thread for my situation.  In my experience, notify_one() is faster than notify_all() for most situations.  Certainly, if you need to notify all the blocked thread, you need to use notify_all().
