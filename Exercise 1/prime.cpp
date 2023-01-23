#include <cmath>
#include <iostream>
#include <set>

#include "prime.hpp"

void find_prime(std::set<int> &prime_number, const int lower, const int upper, const int num_threads, std::mutex &list_lock) {
    bool prime;
    for (int i = lower; i < upper; i += num_threads) {
        prime = true;
        if (i == 2) {
            list_lock.lock();
            prime_number.insert(i);
            list_lock.unlock();
            continue;
        } else if (i <= 1 || i % 2 == 0) {
            continue;
        }
        for (int j = 3; j < std::sqrt(i) + 1; j+=2) {
            if (i % j == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            list_lock.lock();
            prime_number.insert(i);
            list_lock.unlock();
        }
    }

}