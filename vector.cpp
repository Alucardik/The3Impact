#include <cstddef>
#include <iostream>

class Vector {
private:
    int *head = nullptr, *tail = nullptr;
    size_t vec_size = 0, reserved_size = 0;

public:
    Vector() : reserved_size(2) {
        head = new int[reserved_size];
        tail = &head[reserved_size - 1];
    }

    Vector(const Vector& v) : vec_size(v.vec_size), reserved_size(v.reserved_size) {
        head = new int[reserved_size];
        for (int i = 0, *it = v.head; it != v.tail; ++i, ++it) {
            head[i] = *it;
        }
        tail = &head[vec_size];
    }

    Vector& operator = (const Vector& v) {
        tail = nullptr;
        delete head;
        reserved_size = v.reserved_size;
        vec_size = v.vec_size;
        head = new int[reserved_size];
        for (int i = 0, *it = v.head; it != v.tail; ++i, ++it) {
            head[i] = *it;
        }
        tail = &head[vec_size];
        return *this;
    }

    size_t size() const {
        return vec_size;
    }

    void push_back(int inp_num) {
        ++vec_size;
        if (reserved_size >= vec_size) {
            head[vec_size - 1] = inp_num;
        } else {
            reserved_size *= 2;
            int *copy_head = head;
            head = new int[reserved_size];
            for (int i = 0, *it = copy_head; it != tail; ++i, ++it) {
                head[i] = *it;
            }
            head[vec_size - 1] = inp_num;
        }
        tail = &head[vec_size];
    }

    void pop_back() {
        --vec_size;
        tail = &head[vec_size];
    }

    int operator[] (size_t i) const {
        return head[i];
    }

    int& operator[] (size_t i) {
        return head[i];
    }

    ~Vector() {
        tail = nullptr;
        delete head;
    }
};
