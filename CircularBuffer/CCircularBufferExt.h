#pragma once

#include "CCircularBuffer.h"

template<typename T, typename Allocator = std::allocator<T>>
class CCircularBufferExt : public CCircularBuffer<T, Allocator> {
    using CCircularBuffer<T, Allocator>::CCircularBuffer;

public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    void reserve() {
        CCircularBufferExt<T, std::allocator<T>> tmp(this->capacity_);
        for (auto i = this->begin(); i != this->end(); i++) {
            tmp.push_back(*i);
        }
        this->clear();
        this->allocator_.deallocate(this->begin_, this->size_);
        *this = tmp;
    }

    void resize() {
        while (this->capacity_ < this->size_) {
            if (this->capacity_ == 0) {
                this->capacity_++;
                reserve();
            } else {
                this->capacity_ *= 2;
                reserve();
            }
        }
    }

    void push_back(const T& val) {
        if (this->size_ == this->capacity_) {
            if (this->capacity_ == 0) {
                this->capacity_++;
                this->resize();
            } else {
                this->capacity_ *= 2;
                this->resize();
            }
            this->push_back(val);
        } else {
            this->allocator_.construct(this->begin_ + this->end_id_, val);
            this->end_id_++;
            this->size_++;
        }
    }
};