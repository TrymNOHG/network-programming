#include <iostream>
#include <vector>
#include <set>

void find_prime(std::set<int> &prime_num, int lower, int upper, int num_threads, std::mutex &list_lock);
