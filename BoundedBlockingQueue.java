class BoundedBlockingQueue {
    private int capacity;
    private Queue<Integer> queue;
    private Lock lock;
    private Condition noSlot;
    private Condition noItem;
    
    public BoundedBlockingQueue(int capacity) {
        this.capacity = capacity;
        queue = new ArrayDeque<Integer>(capacity);
        lock = new ReentrantLock();
        noSlot = lock.newCondition();
        noItem = lock.newCondition();
    }
    
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
    
    public int dequeue() throws InterruptedException {
        lock.lock();
        
        try{
            while(queue.size() == 0){
                noItem.await();
            }
            int result = queue.remove();
            noSlot.signal();
            return result;
        }
        finally{
            lock.unlock();
        }
    }
    
    public int size() {
        return queue.size();
    }
}
