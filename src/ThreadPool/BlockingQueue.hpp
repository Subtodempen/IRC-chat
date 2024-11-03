#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>


template <typename T>
class BlockingQueue{
public:
  BlockingQueue() = default;
  
  BlockingQueue(const BlockingQueue<T>& obj){
    const std::lock_guard<std::mutex> localLock(this->queueLock); // wrong change soon
    this->queue = obj.queue; 
  }

  void enqueue(T data){
    const std::lock_guard<std::mutex> localLock(this->queueLock);
    queue.push(data);
  }
  
  T dequeue(){
    const std::lock_guard<std::mutex> localLock(this->queueLock);
    
    T val;

    if(!queue.empty()){
      val = queue.front();
      queue.pop();
    }

    return val;
  }

private:
  std::queue<T> queue;
  std::mutex queueLock;
};

