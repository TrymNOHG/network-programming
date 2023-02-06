//
// Created by Trym Hamer Gudvangen on 2/6/23.
//

#ifndef EXERCISE_2_TASK_H
#define EXERCISE_2_TASK_H
#include <functional>
#include <chrono>
#include <thread>
class Task{
public:
    int timeout = 0;
    std::function<void()> task;
    Task() = default;

    Task(const std::function<void()>& task, const int timeout) {
        this->task = task;
        this->timeout = timeout;
    }

    void perform_task() {
        call_timeout();
        task();
    }

private:
    void call_timeout() const {
        std::this_thread::sleep_for(std::chrono::microseconds(timeout));
    }
};
#endif //EXERCISE_2_TASK_H
