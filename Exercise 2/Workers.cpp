// Created by Trym Hamer Gudvangen on 2/5/23.
//
#include "Workers.h"
#include "Task.h"
#include <thread>
#include <condition_variable>

void Workers::post(const std::function<void()>& task) {
    post_timeout(task, 0);
}

void Workers::post_timeout(const std::function<void()>& task, const int timeout) {
    // Thread-safe
    std::unique_lock<std::mutex> lock(this->task_mutex);
    this->task_queue.emplace_back(task, timeout);
    this->cv.notify_one();
}

void Workers::start() {
    for(int i = 0; i < this->num_threads; i++) {
        this->thread_pool.emplace_back([&] {
            while(true) {
                Task task{};
                {
                    std::unique_lock<std::mutex> lock(this->task_mutex);
                    while(this->task_queue.empty()) {
                        if(!running) return;
                        this->cv.wait(lock);
                    }
                    task = *task_queue.begin();
                    task_queue.pop_front();
                }
                this->cv.notify_one();
                if (task.task) {
                    task.perform_task();
                }
            }
        });
    }
}

void Workers::stop() {
    std::unique_lock<std::mutex> lock(this->task_mutex);
    while(!this->task_queue.empty()) {
        this->cv.wait(lock);
    }
    this->running = false;
    cv.notify_all();
}

void Workers::join() {
    std::thread t([&] {
        this->stop();
        for(auto& thread: this->thread_pool) {
            thread.join();
        }
    });
    t.join();
}
