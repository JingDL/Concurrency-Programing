class ReentrantReadWriteLock{
public:
    ReentrantReadWriteLock(){
        numberReader = 0;
        numberWriterInQueue = 0;
        writing = false;
    }
    void read_lock(){
        unique_lock<mutex> uniqLock(mtx);
        numberReader++;
        while(numberWriterInQueue || writing){
            cdv.wait(uniqLock);
        }
    }
    void read_unlock(){
        unique_lock<mutex> uniqLock(mtx);
        if(0 == --numberReader){
            cdv.notify_one();
        }
    }
    void write_lock(){
        unique_lock<mutex> uniqLock(mtx);
        numberWriterInQueue++;
        while(numberReader > 0 || writing){
            cdv.wait(uniqLock);
        }
        numberWriterInQueue--;
        writing = true;
    }
    void write_unlock(){
        unique_lock<mutex> uniqLock(mtx);
        writing = false;
        cdv.notify_one();
    }
private:
    mutex mtx;
    condition_variable cdv;
    int numberReader;
    int numberWriterInQueue;
    bool writing;
};
