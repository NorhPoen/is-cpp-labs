#include <iostream>
#include <unordered_set>
#include <vector>
#include <list>

#include "Allocator.h"

int main(){
    std::list<int, MemoryPoolsAllocator<int>> a;
    size_t size = 100000000;
    for (size_t i = 0; i <= size; i++) {
        a.push_back(1);
        if (i % 1000000 == 0) {
           std::cout << clock() << std::endl;
        }
    }
    return 0;
}
