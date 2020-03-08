#include <iostream>
#include <map>
#include <vector>

template <typename T>
class Polynomial {
private:
    std::map<size_t, T> degrees;
    static void null_res(std::map<size_t, T>& deg) {
        if (deg.empty()) {
            return;
        }
        std::map<size_t, T> deg2;
        for (auto it = deg.rbegin(); it != deg.rend(); ++it) {
            if ((*it).second != static_cast<T>(0)) {
                deg2.insert(*it);
            }
        }
        deg = deg2;
    }

public:
    Polynomial() = default;

    Polynomial(const std::vector<T>& d) {
        for (size_t i = 0; i < d.size(); ++i) {
            if (d[i] != static_cast<T>(0)) {
                degrees.insert({i, d[i]});
            }
        }
    }

    Polynomial(const T& t) {
        if (t != static_cast<T>(0))
            degrees.insert({0, t});
    }

    template <typename Iter>
    Polynomial(Iter beg, Iter end) {
        std::vector<T> d(beg, end);
        for (size_t i = 0; i < d.size(); ++i) {
            if (d[i] != static_cast<T>(0)) {
                degrees.insert({i, d[i]});
            }
        }
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
        if (p1.Degree() == -1 || p2.Degree() == -1) {
            return Polynomial();
        }
        std::map<size_t, T> new_p;
        for (const auto& [it1, elem1] : p1.degrees) {
            for (const auto& [it2, elem2] : p2.degrees) {
                if (new_p.find(it1 + it2) != new_p.end()) {
                    new_p[it1 + it2] += elem1 * elem2;
                } else {
                    new_p.insert({it1 + it2, elem1 * elem2});
                }
            }
        }
        null_res(new_p);
        Polynomial ans;
        ans.degrees = new_p;
        return ans;
    }

    friend Polynomial operator + (const Polynomial& p1, const Polynomial& p2) {
        std::map<size_t, T> new_p(p1.degrees.begin(), p1.degrees.end());
        for (const auto& [it, elem] : p2.degrees) {
            if (new_p.find(it) != new_p.end()) {
                new_p[it] += elem;
            } else {
                new_p.insert({it, elem});
            }
        }
        null_res(new_p);
        Polynomial ans;
        ans.degrees = new_p;
        return ans;
    }

    friend Polynomial operator - (const Polynomial& p1, const Polynomial& p2) {
        if (p1 == p2) {
            return Polynomial();
        }
        std::map<size_t, T> new_p(p1.degrees.begin(), p1.degrees.end());
        for (const auto& [it, elem] : p2.degrees) {
            if (new_p.find(it) != new_p.end()) {
                new_p[it] -= elem;
            } else {
                new_p.insert({it, -elem});
            }
        }
        null_res(new_p);
        Polynomial ans;
        ans.degrees = new_p;
        return ans;
    }

    Polynomial& operator *= (const Polynomial& p2) {
        Polynomial new_p = *this * p2;
        *this = new_p;
        return *this;
    }

    Polynomial& operator += (const Polynomial& p2) {
        Polynomial new_p = *this + p2;
        *this = new_p;
        return *this;
    }

    Polynomial& operator -= (const Polynomial& p2) {
        Polynomial new_p = *this - p2;
        *this = new_p;
        return *this;
    }

    int Degree() const {
        if (degrees.empty()) {
            return -1;
        }
        return (*(--degrees.end())).first;
    }

    T operator[] (const size_t i) const {
        if (degrees.find(i) != degrees.end()) {
            return degrees.at(i);
        } else {
            return 0;
        }
    }

    T operator() (const T& point) const {
        T curr_sum = static_cast<T>(0);
        for (const auto& [it, elem] : degrees) {
            T curr_pow = static_cast<T>(1);
            for (size_t j = 0; j < it; ++j) {
                curr_pow *= point;
            }
            curr_sum += elem * curr_pow;
        }
        return curr_sum;
    }

    typename std::map<size_t, T>::const_iterator begin() const {
        return degrees.cbegin();
    }

    typename std::map<size_t, T>::const_iterator end() const {
        return degrees.cend();
    }

    friend Polynomial operator / (const Polynomial& p1, const Polynomial& p2) {
        if (p1.Degree() < p2.Degree()) {
            return Polynomial();
        } else {
            Polynomial new_p, left_over;
            new_p.degrees = p1.degrees;
            int gap = new_p.Degree() - p2.Degree();
            T max_deg_p2 = p2.degrees.at(p2.Degree());
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
        if (p1.Degree() == -1 || p1 == p2) {
            return p2;
        } else if (p2.Degree() == -1) {
            return p1;
        } else {
            Polynomial p1_c, p2_c, lag_d;
            p1_c.degrees = p1.degrees;
            p2_c.degrees = p2.degrees;
            for (; p2_c.Degree() != -1; ) {
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
        for (const auto& [it, elem] : p1) {
            Polynomial curr_p(1);
            for (size_t j = 0; j < it; ++j) {
                curr_p *= p2;
            }
            curr_p *= p1.degrees.at(it);
            new_p += curr_p;
        }
        return new_p;
    }

    friend std::ostream& operator << (std::ostream& out, const Polynomial& p) {
        if (p.Degree() == -1) {
            out << '0';
        } else if (p.Degree() == 0) {
            out << p.degrees.at(0);
        } else {
            for (auto i = p.degrees.rbegin(); i != p.degrees.rend(); ++i) {
                if (p.degrees.at((*i).first) != static_cast<T>(0)) {
                    if (i == p.degrees.rbegin()) {
                        if (p.degrees.at((*i).first) == static_cast<T>(1)) {
                            if ((*i).first == 1) {
                                out << "x";
                            } else {
                                out << "x^" << (*i).first;
                            }
                        } else if (p.degrees.at((*i).first) == static_cast<T>(-1)) {
                            if ((*i).first == 1) {
                                out << "-x";
                            } else {
                                out << "-x^" << (*i).first;
                            }
                        } else {
                            if ((*i).first == 1) {
                                out << p.degrees.at((*i).first) << "*x";
                            } else {
                                out << p.degrees.at((*i).first) << "*x^" << (*i).first;
                            }
                        }
                    } else {
                        if (p.degrees.at((*i).first) == static_cast<T>(1)) {
                            if ((*i).first == 1) {
                                out << "+x";
                            } else if ((*i).first != 0) {
                                out << "+x^" << (*i).first;
                            } else {
                                out << "+1";
                            }
                        } else if (p.degrees.at((*i).first) == static_cast<T>(-1)) {
                            if ((*i).first == 1) {
                                out << "-x";
                            } else if ((*i).first != 0) {
                                out << "-x^" << (*i).first;
                            } else {
                                out << "-1";
                            }
                        } else {
                            if (p.degrees.at((*i).first) < static_cast<T>(0)) {
                                if ((*i).first == 1) {
                                    out << p.degrees.at((*i).first) << "*x";
                                } else if ((*i).first != 0) {
                                    out << p.degrees.at((*i).first) << "*x^" << (*i).first;
                                } else {
                                    out << p.degrees.at((*i).first);
                                }
                            } else {
                                if ((*i).first == 1) {
                                    out << '+' << p.degrees.at((*i).first) << "*x";
                                } else if ((*i).first != 0) {
                                    out << '+' << p.degrees.at((*i).first) << "*x^" << (*i).first;
                                } else {
                                    out << '+' << p.degrees.at((*i).first);
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
