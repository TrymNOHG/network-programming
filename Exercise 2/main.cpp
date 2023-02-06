#include <iostream>
#include "Workers.h"
int main() {

    Workers worker_threads(4);
    Workers event_loop(1);

    worker_threads.start(); // Create 4 internal threads
    event_loop.start();     // Create 1 internal thread

    worker_threads.post([]{
        std::cout << "Task A\n";
        // Task A
    });
    worker_threads.post([]{
        std::cout << "Task B\n";
        // Task B
        // Might run in parallel with task A
    });

    event_loop.post([]{
        std::cout << "Task C\n";
        // Task C
        // Might run in parallel with task A and B
    });

    event_loop.post([] {
        std::cout << "Task D\n";
        // Task D
        // Will run after task C
        // Might run in parallel with task A and B
    });


    worker_threads.join(); // Calls join() on the worker threads
    event_loop.join();     // Calls join() on the event thread


    return 0;
}
