#pragma once

#include <iostream>
#include <algorithm>
#include <fstream>

#include "Pool.h"

template<typename T>
class Comparator {
private:
    size_t size_expect_;

public:
    explicit Comparator(size_t n){
        size_expect_ = n;
    }

    bool operator()(const Pool<T>* lhs, const Pool<T>* rhs){
        if (lhs->size() < size_expect_) {
            return false;
        }
        if (rhs->size() < size_expect_) {
            return true;
        }
        size_t overflow1 = lhs->chunk_size() - (size_expect_ % lhs->chunk_size());
        size_t overflow2 = rhs->chunk_size() - (size_expect_ % rhs->chunk_size());

        if (overflow1 == overflow2) {
            return lhs->chunk_size() > rhs->chunk_size();
        } else {
            return overflow1 < overflow2;
        }
    }
};

template<typename T>
class MemoryPoolsAllocator {
public:

    using pointer = T*;
    using const_pointer = const T*;
    using void_pointer = void*;
    using const_void_pointer = const void*;
    using value_type = T;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;

    template <typename U>
    struct rebind {
        using other = MemoryPoolsAllocator<U>;
    };

private:
    std::vector<Pool<value_type>*> pools_;

public:
    explicit MemoryPoolsAllocator() {
        char c;
        std::string str = "";
        size_t cnt = 0;
        size_t value;
        size_t amount_of_pools_;
        std::ifstream in("Config.txt", std::ios::binary);

        std::vector<size_t> chunks_per_pool_;
        std::vector<size_t> chunk_size_;

        while(true){
            in.get(c);
            if(c != '\r'){
                str += c;
            } else {
                value = static_cast<size_t>(std::atoi(str.c_str()));
                amount_of_pools_ = value;
                str = "";
                in.get(c);
                break;
            }
        }

        while(!in.eof()) {
            in.get(c);
            if(!isspace(c) && (c != '\r')){
                str += c;
            } else {
                value = static_cast<size_t>(std::atoi(str.c_str()));
                if(cnt < amount_of_pools_){
                    chunks_per_pool_.push_back(value);
                } else {
                    chunk_size_.push_back(value);
                }
                cnt++;
                str = "";
                if(c == '\r') {
                    in.get(c);
                }
            }
        }
        for(size_t i = 0; i < amount_of_pools_; i++) {
            pools_.push_back(new Pool<T>(chunks_per_pool_[i], chunk_size_[i]));
        }
    }

    MemoryPoolsAllocator(const MemoryPoolsAllocator& other) = default;

    pointer allocate(size_t n) {
        std::sort(pools_.begin(), pools_.end(), Comparator<T>(n));
        for(Pool<value_type>* i : pools_){
            pointer p = i->allocate(n);
            if (p != nullptr) {
                return p;
            }
        }
        throw std::bad_alloc{};
    }

    void deallocate(pointer p, size_t n) {
        for(Pool<value_type>* i : pools_){
            if (i->deallocate(p, n)) {
                return;
            }
        }
    }

    ~MemoryPoolsAllocator() {
        for(Pool<value_type>* i : pools_){
            delete i;
        }
    }
};

template<typename T, typename U>
bool operator==(const MemoryPoolsAllocator<T>& lhs, const MemoryPoolsAllocator<U>& rhs) {
    return false;
}

template<typename T, typename U>
bool operator!=(const MemoryPoolsAllocator<T>& lhs, const MemoryPoolsAllocator<U>& rhs) {
    return !(lhs == rhs);
}
