#include <iostream>
#include <vector>

template <typename T>
class Polynomial {
private:
    std::vector<T> degrees;

    static void null_res(std::vector<T>& deg) {
        if (deg.empty()) {
            return;
        }
        size_t counter = 0;
        for (auto it = deg.rbegin(); it != deg.rend(); ++it) {
            if (*it != static_cast<T>(0)) {
                break;
            }
            ++counter;
        }
        deg.resize(deg.size() - counter);
    }

public:
    Polynomial() = default;

    Polynomial(const std::vector<T>& d) : degrees(d) {
        null_res(degrees);
    }

    Polynomial(const T& t) {
        degrees.push_back(t);
        null_res(degrees);
    }

    template <typename Iter>
    Polynomial(Iter beg, Iter end) : degrees(std::vector<T>(beg, end)) {
        null_res(degrees);
    }

    friend bool operator == (const Polynomial& p1, const Polynomial& p2) {
        if (p1.degrees.size() < p2.degrees.size()) {
            std::vector<T> c_copy = p1.degrees;
            c_copy.resize(p2.degrees.size());
            return c_copy == p2.degrees;

        } else if (p1.degrees.size() > p2.degrees.size()) {
            std::vector<T> c_copy = p2.degrees;
            c_copy.resize(p1.degrees.size());
            return c_copy == p1.degrees;
        } else {
            return p1.degrees == p2.degrees;
        }
    }

    friend bool operator != (const Polynomial& p1, const Polynomial& p2) {
        return !(p1 == p2);
    }

    friend Polynomial operator * (const Polynomial& p1, const Polynomial& p2) {
        std::vector<T> new_p;
        new_p.resize(p1.degrees.size() + p2.degrees.size() - 1);
        for (size_t i = 0; i < p1.degrees.size(); ++i) {
            if (p1.degrees[i] == static_cast<T>(0)) {
                continue;
            }
            for (size_t j = 0; j < p2.degrees.size(); ++j) {
                if (p2.degrees[j] == static_cast<T>(0)) {
                    continue;
                }
                new_p[i + j] += p1.degrees[i] * p2.degrees[j];
            }
        }
        null_res(new_p);
        return Polynomial(new_p);
    }

    friend Polynomial operator + (const Polynomial& p1, const Polynomial& p2) {
        std::vector<T> new_p(p1.degrees.begin(), p1.degrees.end());
        if (p1.degrees.size() < p2.degrees.size()) {
            new_p.resize(p2.degrees.size());
        }
        for (size_t i = 0; i < p2.degrees.size(); ++i) {
            new_p[i] += p2.degrees[i];
        }
        null_res(new_p);
        return Polynomial(new_p);
    }

    friend Polynomial operator - (const Polynomial& p1, const Polynomial& p2) {
        std::vector<T> new_p(p1.degrees.begin(), p1.degrees.end());
        if (p1.degrees.size() < p2.degrees.size()) {
            new_p.resize(p2.degrees.size());
        }
        for (size_t i = 0; i < p2.degrees.size(); ++i) {
            new_p[i] -= p2.degrees[i];
        }
        null_res(new_p);
        return Polynomial(new_p);
    }

    Polynomial& operator *= (const Polynomial& p2) {
        Polynomial new_p = *this * p2;
        null_res(new_p.degrees);
        *this = new_p;
        return *this;
    }

    Polynomial& operator += (const Polynomial& p2) {
        Polynomial new_p = *this + p2;
        null_res(new_p.degrees);
        *this = new_p;
        return *this;
    }

    Polynomial& operator -= (const Polynomial& p2) {
        Polynomial new_p = *this - p2;
        null_res(new_p.degrees);
        *this = new_p;
        return *this;
    }

    size_t Degree() const {
        if (degrees.empty()) {
            return -1;
        }
        return degrees.size() - 1;
    }

    T operator[] (const size_t i) const {
        if (i >= degrees.size()) {
            return 0;
        } else {
            return degrees[i];
        }
    }

    T operator() (const T& point) const {
        T curr_pow = static_cast<T>(1), curr_sum = static_cast<T>(0);
        for (size_t i = 0; i < degrees.size(); curr_pow *= point, ++i) {
            curr_sum += degrees[i] * curr_pow;
        }
        return curr_sum;
    }

    typename std::vector<T>::const_iterator begin() const {
        return degrees.cbegin();
    }

    typename std::vector<T>::const_iterator end() const {
        return degrees.cend();
    }

    friend std::ostream& operator << (std::ostream& out, const Polynomial& p) {
        if (p.degrees.size() == 0) {
            return out;
        } else if (p.degrees.size() == 1) {
            out << p.degrees[0];
            return out;
        } else {
            for (int i = p.degrees.size() - 2; i >= 0; --i) {
                if (p.degrees[i] != 0) {
                    if (p.degrees[i] != static_cast<T>(1) && p.degrees[i] != static_cast<T>(-1)) {
                        out << p.degrees[i];
                    } else if (p.degrees[i] != static_cast<T>(-1)) {
                        out << '-';
                    }
                }
            }
        }
        return out;
    }
};
