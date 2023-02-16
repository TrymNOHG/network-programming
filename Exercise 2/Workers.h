//
// Created by Trym Hamer Gudvangen on 2/5/23.
//

#ifndef EXERCISE_2_WORKERS_H
#define EXERCISE_2_WORKERS_H
#include <functional>
#include <iostream>
#include <list>
#include <vector>
#include <thread>
#include "Task.h"

// Use Condition Variables
class Workers{
    
    int num_threads;
    std::list<Task> task_queue;
    std::vector<std::thread> thread_pool;
    std::condition_variable cv;
    std::mutex task_mutex;
    bool running = true;

public:
    explicit Workers(int num_threads){
        this->num_threads = num_threads;
    };

    void post(const std::function<void()>&);
    void post_timeout(const std::function<void()>&, int);
    void start();
    void stop();
    void join();
};
#endif //EXERCISE_2_WORKERS_H