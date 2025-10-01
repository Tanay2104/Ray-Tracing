#include "threadsafequeue.h"

void ThreadSafeQueue::push(std::pair<int, int> task) {
            std::lock_guard<std::mutex> lg(mtx);
            task_queue.push(task);
            cv.notify_one();
}


std::optional<std::pair<int, int>> ThreadSafeQueue::pop() {
            std::unique_lock<std::mutex> ul(mtx);
            cv.wait(ul, [this] {return (!(this->task_queue.empty()) || done);});
            if (!task_queue.empty()){
                std::pair<int, int> task = task_queue.front();
                task_queue.pop();
                return task;
            }
            else {
                return std::optional<std::pair<int, int>>(std::nullopt);
            }
        }

void ThreadSafeQueue::shutdown() {
            std::lock_guard<std::mutex> lg(mtx);
            done = true;
            cv.notify_all();
        }