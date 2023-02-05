#include <iostream>
#include <thread>
#include <set>

#include "prime.hpp"

//#define uint unsigned

using std::cout;
using std::cin;

int main(int argc, char **argv) {
    std::mutex list_lock;
    std::string threadQuest ("How many threads do you want?\n");
    int num_threads, lower, upper;

    cout << threadQuest;
    getline(std::cin, threadQuest);

    num_threads = stoi(threadQuest);

    cout << "From what number?\n";
    cin >> lower;

    cout << "To what number?\n";
    cin >> upper;

    std::set<int> prime_numbers;


    std::thread threads[num_threads];

    for (int i = 0; i < sizeof(threads)/sizeof(std::thread); i++) {
        threads[i] = std::thread ([&, lower, upper, num_threads, i] {
            find_prime(prime_numbers, lower + i, upper, num_threads, list_lock);
        });
    }


    for (auto& t : threads) {
        t.join();
    }

    for (int prime : prime_numbers) {
        std::cout << prime << " ";
    }

    return 0;
}
