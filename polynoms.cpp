#include <iostream>
#include <vector>

template <typename T>
class Polynomial {
private:
    std::vector<T> degrees;

    static void null_res(std::vector<T>& deg) {
        size_t counter = 0;
        for (auto it = --deg.end(); it != deg.begin(); --it) {
            if (it != static_cast<T>(0)) {
                break;
            }
            ++counter;
        }
        deg.resize(deg.size() - counter);
    }

public:
    Polynomial() {}

    Polynomial(const std::vector<T>& d) : degrees(d) {}

    Polynomial(const T& t) {
        degrees.push_back(t);
    }

    template <typename Iter>
    Polynomial(Iter beg, Iter end) : degrees(std::vector<T>(beg, end)) {}

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
            if (p1.degrees[i] == 0) {
                continue;
            }
            for (size_t j = 0; j < p2.degrees.size(); ++j) {
                if (p2.degrees[j] == 0) {
                    continue;
                }
                new_p[i + j] += p1.degrees[i] * p2.degrees[j];
            }
        }
        null_res(new_p);
        return Polynomial(new_p);
    }

    friend Polynomial operator + (const Polynomial& p1, const Polynomial& p2) {
        if (p1.degrees.size() < p2.degrees.size()) {
            std::vector<T> new_p(p2.degrees.begin(), p2.degrees.end());
            for (size_t i = 0; i < p1.degrees.size(); ++i) {
                new_p[i] += p1.degrees[i];
            }
            null_res(new_p);
            return Polynomial(new_p);
        } else if (p1.degrees.size() > p2.degrees.size()) {
            std::vector<T> new_p(p1.degrees.begin(), p1.degrees.end());
            for (size_t i = 0; i < p2.degrees.size(); ++i) {
                new_p[i] += p2.degrees[i];
            }
            null_res(new_p);
            return Polynomial(new_p);
        } else {
            std::vector<T> new_p;
            new_p.resize(p1.degrees.size());
            for (size_t i = 0; i < p2.degrees.size(); ++i) {
                new_p[i] += p2.degrees[i] + p1.degrees[i];
            }
            null_res(new_p);
            return Polynomial(new_p);
        }
    }

    friend Polynomial operator - (const Polynomial& p1, const Polynomial& p2) {
        if (p1.degrees.size() < p2.degrees.size()) {
            std::vector<T> new_p(p2.degrees.begin(), p2.degrees.end());
            for (size_t i = 0; i < p1.degrees.size(); ++i) {
                new_p[i] -= p1.degrees[i];
            }
            null_res(new_p);
            return Polynomial(new_p);
        } else if (p1.degrees.size() > p2.degrees.size()) {
            std::vector<T> new_p(p1.degrees.begin(), p1.degrees.end());
            for (size_t i = 0; i < p2.degrees.size(); ++i) {
                new_p[i] -= p2.degrees[i];
            }
            null_res(new_p);
            return Polynomial(new_p);
        } else {
            std::vector<T> new_p(p1.degrees.begin(), p1.degrees.end());
            for (size_t i = 0; i < p2.degrees.size(); ++i) {
                new_p[i] -= p2.degrees[i];
            }
            null_res(new_p);
            return Polynomial(new_p);
        }
    }

    Polynomial& operator *= (const Polynomial& p2) {
        Polynomial new_p = *this * p2;
        null_res(new_p);
        *this = new_p;
        return *this;
    }

    Polynomial& operator += (const Polynomial& p2) {
        Polynomial new_p = *this + p2;
        null_res(new_p);
        *this = new_p;
        return *this;
    }

    Polynomial& operator -= (const Polynomial& p2) {
        Polynomial new_p = *this - p2;
        null_res(new_p);
        *this = new_p;
        return *this;
    }

    const size_t Degree() const {
        if (degrees.empty()) {
            return -1;
        }
        return degrees.size() - 1;
    }

    const T operator [] (const size_t i) const {
        if (i >= degrees.size()) {
            return 0;
        } else {
            return degrees[i];
        }
    }

    friend std::ostream& operator << (std::ostream& out, const Polynomial& p) {
        for (int i = p.degrees.size() - 1; i >= 0; --i) {
            if (p.degrees[i] != 0)
                out << p.degrees[i] << "x^" << i << " + ";
                out << p.degrees[i] << "x^" << i << " + ";
        }
        out << '\n';
        return out;
    }
};

int main() {
//    std::vector<int> v = {5, 1, 2, 3}, v2 = {5, 6, 0, 8};
//    Polynomial p(v2);
//    Polynomial pr(v);
//    std::cout << pr * p << std::endl;
//    pr *= p;
//    std::cout << pr << std::endl;
    std::vector<int> v1={0, 9, 2, -1, 1, 1, 2, 0, 0, 0}, v2={-1, 2, 1, 1, 0, 0, 2},
            v3={1, 2, 3}, v4={2, 1}, v0={0,0,0,0,0,0,0}, v5={0, 0, 0, 0, -2};
    Polynomial<int> p1(v1), p2(42), p3, p4(v2.begin()+2, v2.end()), p5(v3), p6(v4),
            p7(v3.rbegin()+1, v3.rend()), p8(v1.begin(), v1.begin()+7), p0(v0), p10(v5);
    std::cout << "p1: " << p1 << "p2: " << p2 << "p3: " << p3 << "p4: " << p4 << "p5: " << p5 << "p6: " << p6 << "p7: " << p7 << "p8: " << p8;
    std::cout << "\nHUI!!!!\n";
    std::cout << "p1 = 2x^6+x^5+x^4-x^3+2x^2+9x\np2 = 42\np3 = 0\np4 = 2x^4+x+1\np5 = 3x^2+2x+1\np6 = x+2\np7 = x+2\np8 = 2x^6+x^5+x^4-x^3+2x^2+9x\n\n";
}
