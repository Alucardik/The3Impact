#include <iostream>
#include <vector>

template <typename T>
class Polynomial {
private:
    std::vector<T> degrees;

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
        Polynomial new_p;
        new_p.degrees.resize(p1.degrees.size() + p2.degrees.size() - 1);
        for (size_t i = 0; i < p1.degrees.size(); ++i) {
            if (p1.degrees[i] == 0) {
                continue;
            }
            for (size_t j = 0; j < p2.degrees.size(); ++j) {
                if (p2.degrees[j] == 0) {
                    continue;
                }
                new_p.degrees[i + j] += p1.degrees[i] * p2.degrees[j];
            }
        }
        return new_p;
    }

    friend Polynomial operator + (const Polynomial& p1, const Polynomial& p2) {
        Polynomial new_p;
        return new_p;
    }

    friend Polynomial operator - (const Polynomial& p1, const Polynomial& p2) {
        Polynomial new_p;
        return new_p;

    }

    friend Polynomial& operator *= (Polynomial& p1, const Polynomial& p2) {
        return p1;
    }

    friend Polynomial& operator += (Polynomial& p1, const Polynomial& p2) {
        return p1;
    }

    friend Polynomial& operator -= (Polynomial& p1, const Polynomial& p2) {
        return p1;
    }

    friend std::ostream& operator << (std::ostream& out, const Polynomial& p) {
        for (int i = p.degrees.size() - 1; i >= 0; --i) {
            out << p.degrees[i] << "x^" << i << " + ";
        }
        out << '\n';
        return out;
    }
};

int main() {
    std::vector<int> v = {5, 1, 2, 3}, v2 = {5, 6, 0, 8};
    Polynomial p(v2);
    Polynomial pr = Polynomial(v) * p;
//    std::cout << ka << std::endl;
    std::cout << pr;
}
