#pragma once

#include "BufferIterator.h"
#include "BufferIteratorConst.h"

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBuffer {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

protected:
    size_t size_ = 0;
    size_t capacity_ = 0;
    T* begin_;
    Allocator allocator_;
    size_type begin_id_ = 0;
    size_type end_id_ = 0;

public:
    CCircularBuffer(){
        begin_ = nullptr;
        CCircularBuffer(0);
    }

    CCircularBuffer(size_t capacity, const Allocator& allocator = Allocator()){
        capacity_ = capacity;
        allocator_ = allocator;
        begin_ = allocator_.allocate(capacity);
    }

    CCircularBuffer(const CCircularBuffer& copy){
        begin_ = allocator_.allocate(copy.capacity_);
        allocator_ = copy.allocator_;
        size_ = copy.size_;
        capacity_ = copy.capacity_;
        begin_id_ = copy.begin_id_;
        end_id_ = copy.end_id_;
        if (size_ != 0) {
            size_type i = 0;
            do {
                allocator_.construct(begin_ + begin_id_, copy[i]);
                begin_id_ = (begin_id_ + 1) % capacity_;
                i++;
            } while (begin_id_ != end_id_);
            begin_id_ = copy.begin_id_;
        }
    }

    ~CCircularBuffer() {
        clear();
        allocator_.deallocate(begin_, capacity_);
    }

    virtual void push_back(const T& val){
        if (size_ == capacity_) {
            pop_front();
        }
        allocator_.construct(begin_ + end_id_, val);
        end_id_ = (end_id_ + 1) % capacity_;
        size_++;
    }

    void pop_front(){
        allocator_.destroy(begin_ + begin_id_);
        begin_id_ = (begin_id_ + 1) % capacity_;
        size_--;
    }

    void assign(size_t n, T val) {
        CCircularBuffer<T, std::allocator<T>> tmp(n);
        for (int i = 0; i < n; ++i) {
            tmp.push_back(val);
        }
        *this = tmp;
    }

    BufferIterator<T> begin(){
        return BufferIterator<T>(begin_, 0, begin_id_, true, end_id_, false, size_, capacity_);
    }

    BufferIterator<T> end(){
        return BufferIterator<T>(begin_, size_, begin_id_, false, end_id_, true, size_, capacity_);
    }

    BufferIterator<T> cbegin(){
        return BufferIterator<T>(begin_, 0, begin_id_, true, end_id_, false, size_, capacity_);
    }

    BufferIterator<T> cend(){
        return BufferIterator<T>(begin_, size_, begin_id_, false, end_id_, true, size_, capacity_);
    }

    size_t size() {
        return size_;
    }

    bool empty(){
        return size_ == 0;
    }

    void clear(){
        while(!this->empty()){
            pop_front();
        }
    }

    friend bool operator==(CCircularBuffer<T>& lhs, CCircularBuffer<T>& rhs){
        if(!(lhs.size_ == rhs.size_ && lhs.capacity_ == rhs.capacity_)){
            return false;
        } else {
            BufferIterator left = lhs.begin();
            for(BufferIterator right = rhs.begin(); right != rhs.end(); right++){
                if(*left != *right){
                    return false;
                }
                left++;
            }
        }
        return true;
    }

    friend bool operator!=(CCircularBuffer<T>& lhs, CCircularBuffer<T>& rhs){
        return !(lhs == rhs);
    }

    void swap(CCircularBuffer& rhs){
        CCircularBuffer<T, std::allocator<T>> tmp = *this;
        *this = rhs;
        rhs = tmp;
    }

    reference operator[](size_t id) {
        return begin_[(begin_id_ + id + capacity_) % capacity_];
    }
};
