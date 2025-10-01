#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>
class ThreadSafeQueue {
    private:
        std::mutex mtx;
        std::condition_variable cv;
        std::queue<std::pair<int, int>> task_queue;
        bool done = false;

    public:
        void push(std::pair<int, int> task);

        std::optional<std::pair<int, int>> pop();

        void shutdown();
};