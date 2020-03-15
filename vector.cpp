#include <cstddef>
#include <iostream>

class Vector {
private:
    int *head = nullptr;
    size_t vec_size = 0, reserved_size = 0;

public:
    Vector() : reserved_size(2) {
        head = new int[reserved_size];
    }

    Vector(const Vector& v) : vec_size(v.vec_size), reserved_size(v.reserved_size) {
        head = new int[reserved_size];
        for (int i = 0; i != v.size(); ++i) {
            head[i] = v.head[i];
        }
    }

    Vector& operator = (const Vector& v) {
        delete []head;
        reserved_size = v.reserved_size;
        vec_size = v.vec_size;
        head = new int[reserved_size];
        for (int i = 0; i != v.size(); ++i) {
            head[i] = v.head[i];
        }
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
            for (int i = 0; i != size(); ++i) {
                head[i] = copy_head[i];
            }
            head[vec_size - 1] = inp_num;
        }
    }

    void pop_back() {
        --vec_size;
    }

    int operator[] (size_t i) const {
        return head[i];
    }

    int& operator[] (size_t i) {
        return head[i];
    }

    ~Vector() {
        vec_size = 0;
        reserved_size = 0;
        delete []head;
    }
};

int main() {
    Vector v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    Vector v2;
    v2 = v;
    v.pop_back();
    std::cout << sizeof(v2) << std::endl;
    for (size_t i = 0; i < v2.size(); ++i) {
        std::cout << v2[i] << std::endl;
    }

    return 0;
}
