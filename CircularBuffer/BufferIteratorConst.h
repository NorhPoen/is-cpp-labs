#pragma once

template<typename T>
class BufferIteratorConst{
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using size_type = std::size_t;

private:
    pointer begin_;
    int position_ = 0;
    size_type begin_id_ = 0;
    bool begin_used_ = false;
    size_type end_id_ = 0;
    bool end_used_ = false;
    size_type size_ = 0;
    size_type capacity_ = 0;

public:
    BufferIteratorConst() = default;

    BufferIteratorConst(pointer begin, int position, size_type begin_id, bool begin_used,
                   size_type end_id, bool end_used, size_type size, size_type capacity){
        begin_ = begin;
        position_ = position;
        begin_id_ = begin_id;
        begin_used_ = begin_used;
        end_id_ = end_id;
        end_used_ = end_used;
        size_ = size;
        capacity_ = capacity;
    }

    BufferIteratorConst operator+(difference_type id){
        BufferIteratorConst<T> result = *this;
        result.begin_used_ = false;
        result.position_ = this->position_ + id;
        return result;
    }

    BufferIteratorConst& operator+=(difference_type id){
        begin_used_ = false;
        position_ += id;
        return *this;
    }

    BufferIteratorConst operator-(difference_type id){
        BufferIteratorConst<T> result = *this;
        result.begin_used_ = false;
        result.position_ = this->position_ - id;
        return result;
    }

    BufferIteratorConst& operator-=(difference_type id){
        begin_used_ = false;
        position_ -= id;
        return *this;
    }

    friend difference_type operator-( BufferIteratorConst& lhs,  BufferIteratorConst& rhs) {
        return static_cast<difference_type>(lhs.position_ - rhs.position_);
    }

    friend difference_type operator+( BufferIteratorConst& lhs,  BufferIteratorConst& rhs) {
        return static_cast<difference_type>(lhs.position_ + rhs.position_);
    }

    reference operator[](difference_type id){
        return begin_[(position_ + begin_id_ + id + (std::abs(id + position_) + 1) * capacity_) % capacity_];
    }

    friend bool operator==( BufferIteratorConst& lhs,  BufferIteratorConst& rhs) {
        if (lhs.capacity_ == 0 || rhs.capacity_ == 0) {
            if (lhs.capacity_ == rhs.capacity_) {
                return true;
            }
            return false;
        }
        return ((lhs.position_ + lhs.begin_id_ + (abs(lhs.position_) / lhs.capacity_ + 1) * lhs.capacity_) % lhs.capacity_ ==
                (rhs.position_ + rhs.begin_id_ + (abs(rhs.position_) / rhs.capacity_ + 1) * rhs.capacity_) % rhs.capacity_ &&
                !(lhs.begin_used_ && rhs.end_used_) || (lhs.size_ == 0 && rhs.size_ == 0));
    }

    friend bool operator!=( BufferIteratorConst& lhs,  BufferIteratorConst& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator<( BufferIteratorConst& lhs,  BufferIteratorConst& rhs){
        return (lhs.position_ + lhs.begin_id_ + (std::abs(lhs.position_) / lhs.capacity_ + 1) * lhs.capacity_) % lhs.capacity_ <
               (rhs.position_ + rhs.begin_id_ + (std::abs(rhs.position_) / rhs.capacity_ + 1) * rhs.capacity_) % rhs.capacity_;
    }

    friend bool operator>( BufferIteratorConst& lhs,  BufferIteratorConst& rhs){
        return (rhs < lhs);
    }

    friend bool operator>=( BufferIteratorConst& lhs,  BufferIteratorConst& rhs){
        return !(lhs < rhs);
    }

    friend bool operator<=( BufferIteratorConst& lhs,  BufferIteratorConst& rhs){
        return !(lhs > rhs);
    }

    BufferIteratorConst<T>& operator++() {
        begin_used_ = false;
        position_++;
        return *this;
    }

    BufferIteratorConst<T>& operator++(int){
        return ++(*this);
    }

    BufferIteratorConst<T>& operator--() {
        begin_used_ = false;
        position_--;
        return *this;
    }

    BufferIteratorConst<T>& operator--(int){
        return --(*this);
    }

    reference operator*() {
        return begin_[(position_ + begin_id_ + (std::abs(position_) / capacity_ + 1) * capacity_) % capacity_];
    }

    pointer operator->() {
        return *(begin_[(position_ + begin_id_ + (std::abs(position_) / capacity_ + 1) * capacity_) % capacity_]);
    }
};