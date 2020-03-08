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
        if (p1.degrees.size() == p2.degrees.size())
            return p1.degrees == p2.degrees;
        else
            return false;
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

    Polynomial& operator = (const Polynomial& p2) {
        degrees = p2.degrees;
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

    friend Polynomial operator / (const Polynomial& p1, const Polynomial& p2) {
        if (p1.Degree() < p2.Degree()) {
            return Polynomial();
        } else {
            Polynomial new_p(p1.degrees), left_over;
            int gap = new_p.Degree() - p2.Degree();
            T max_deg_p2 = p2.degrees[p2.Degree()];
            while (gap >= 0 && !new_p.degrees.empty()) {
                T new_deg = new_p.degrees[new_p.Degree()] / max_deg_p2;
                std::vector<T> tmp_v(gap + 1);
                tmp_v[gap] = new_deg;
                left_over += Polynomial(tmp_v);
                new_p -= Polynomial(tmp_v) * p2;
                gap = new_p.Degree() - p2.Degree();
            }
            null_res(left_over.degrees);
            return left_over;
        }
    }

    friend Polynomial operator % (const Polynomial& p1, const Polynomial& p2) {
        Polynomial ans_p = p1 - (p1 / p2) * p2;
        null_res(ans_p.degrees);
        return ans_p;
    }

    friend Polynomial operator , (const Polynomial& p1, const Polynomial& p2) {
        if (static_cast<int>(p1.Degree()) == -1 || p1 == p2) {
            return p2;
        } else if (static_cast<int>(p2.Degree()) == -1) {
            return p1;
        } else {
            Polynomial p1_c(p1.degrees), p2_c(p2.degrees), lag_d;
            for (; !p2_c.degrees.empty(); ) {
                lag_d = p1_c % p2_c;
                p1_c = p2_c;
                p2_c = lag_d;
            }
            p1_c = p1_c / p1_c.degrees[p1_c.Degree()];
            null_res(p1_c.degrees);
            return p1_c;
        }
    }

    friend Polynomial operator & (const Polynomial& p1, const Polynomial& p2) {
        Polynomial new_p;
        for (size_t i = 0; i < p1.degrees.size(); ++i) {
            Polynomial curr_p(1);
            for (size_t j = 0; j < i; ++j) {
                curr_p *= p2;
            }
            curr_p *= p1.degrees[i];
            new_p += curr_p;
        }
        return new_p;
    }

    friend std::ostream& operator << (std::ostream& out, const Polynomial& p) {
        if (p.degrees.size() == 0) {
            out << '0';
        } else if (p.degrees.size() == 1) {
            out << p.degrees[0];
        } else if (p.degrees.size() != 0) {
            for (int i = static_cast<int>(p.degrees.size()) - 1; i >= 0; --i) {
                if (p.degrees[i] != static_cast<T>(0)) {
                    if (i == static_cast<int>(p.degrees.size()) - 1) {
                        if (p.degrees[i] == static_cast<T>(1)) {
                            if (i == 1) {
                                out << "x";
                            } else {
                                out << "x^" << i;
                            }
                        } else if (p.degrees[i] == static_cast<T>(-1)) {
                            if (i == 1) {
                                out << "-x";
                            } else {
                                out << "-x^" << i;
                            }
                        } else {
                            if (i == 1) {
                                out << p.degrees[i] << "*x";
                            } else {
                                out << p.degrees[i] << "*x^" << i;
                            }
                        }
                    } else {
                        if (p.degrees[i] == static_cast<T>(1)) {
                            if (i == 1) {
                                out << "+x";
                            } else if (i != 0) {
                                out << "+x^" << i;
                            } else {
                                out << "+1";
                            }
                        } else if (p.degrees[i] == static_cast<T>(-1)) {
                            if (i == 1) {
                                out << "-x";
                            } else if (i != 0) {
                                out << "-x^" << i;
                            } else {
                                out << "-1";
                            }
                        } else {
                            if (p.degrees[i] < static_cast<T>(0)) {
                                if (i == 1) {
                                    out << p.degrees[i] << "*x";
                                } else if (i != 0) {
                                    out << p.degrees[i] << "*x^" << i;
                                } else {
                                    out << p.degrees[i];
                                }
                            } else {
                                if (i == 1) {
                                    out << '+' << p.degrees[i] << "*x";
                                } else if (i != 0) {
                                    out << '+' << p.degrees[i] << "*x^" << i;
                                } else {
                                    out << '+' << p.degrees[i];
                                }
                            }
                        }
                    }
                }
            }
        }
        return out;
    }
};
