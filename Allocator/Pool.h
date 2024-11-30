#pragma once

#include <iostream>
#include <vector>

template<typename T>
class Pool {
private:
    size_t chunks_per_pool_;
    size_t chunk_size_;

    T* begin_;

    std::vector<std::pair<size_t, size_t>> free_chunks_;

public:
    using pointer = T*;

    explicit Pool(size_t chunks_per_pool, size_t chunk_size){
        chunks_per_pool_ = chunks_per_pool;
        chunk_size_ = chunk_size;
        begin_ = static_cast<T*>(operator new(size() * sizeof(T)));
        free_chunks_.emplace_back(0, chunks_per_pool);
    }

    T* allocate(size_t n){
        size_t free = 0;
        size_t counter = n / chunk_size_;
        if(n % chunk_size_ != 0) {
            counter++;
        }
        for (int j = 0; j < free_chunks_.size(); j++) {
            auto i = free_chunks_[j];
            if (i.second >= counter) {
                free = i.first;
                if (i.second == counter) {
                    free_chunks_.erase(free_chunks_.begin() + j);
                } else {
                    free_chunks_[j].first += counter;
                    free_chunks_[j].second -= counter;
                }
                return begin_ + free * chunk_size_;
            }
        }
        return nullptr;
    }

    bool deallocate(T* p, size_t n) {
        if (p >= begin_ && p <= (begin_ + size())) {
            size_t id = p - begin_;
            free_chunks_.emplace_back(id, n);
            for (int i = 0; i < free_chunks_.size(); i++) {
                auto j = free_chunks_[i];
                if (j.first == id + n) {
                    free_chunks_.back().second += j.second;
                    free_chunks_.erase(free_chunks_.begin() + i);
                }
                if (j.first == id - j.second) {
                    free_chunks_.back().first = free_chunks_[i].first;
                    free_chunks_.back().second += free_chunks_[i].second;
                    free_chunks_.erase(free_chunks_.begin() + i);
                }
            }
            return true;
        } else {
            return false;
        }
    }

    size_t size() const{
        return chunk_size_ * chunks_per_pool_;
    }
    size_t chunk_size() const{
        return chunk_size_;
    }
    size_t chunk_per_pool() const{
        return chunks_per_pool_;
    }

    ~Pool() {
        operator delete(begin_, size() * sizeof(T));
    }
};

template<typename T, typename U>
bool operator==(const Pool<T>& lhs, const Pool<U>& rhs) {
    return false;
}

template<typename T, typename U>
bool operator!=(const Pool<T>& lhs, const Pool<U>& rhs) {
    return !(lhs == rhs);
}
