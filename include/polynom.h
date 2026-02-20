#include <iostream>
#include <string>
#include <forward_list>
#include <vector>

struct monom {
	int degree = 0;
	double coef = 0.0;

    monom(int deg, double c) : degree(deg), coef(c) {
        int xDeg = degree / 100;
        int yDeg = (degree / 10) % 10;
        int zDeg = degree % 10;

        if (xDeg > 9 || yDeg > 9 || zDeg > 9) {
            throw "Error";
        }
    }

    friend monom operator+(const monom& m1, const monom& m2) {
        if (m1.degree != m2.degree) {
            throw "Error";
        }
        return monom(m1.degree, m1.coef + m2.coef);
    }

    friend monom operator-(const monom& m1, const monom& m2) {
        if (m1.degree != m2.degree) {
            throw "Error";
        }
        return monom(m1.degree, m1.coef - m2.coef);
    }

    friend monom operator*(const monom& m1, double value) {
        if (value == 0) {
            return monom(0, 0.0);
        }
        return monom(m1.degree, m1.coef * value);
    }

    friend monom operator/(const monom& m1, double value) {
        if (value == 0) {
            throw "Error";
        }
        return monom(m1.degree, m1.coef / value);
    }

    friend monom operator*(const monom& m1, const monom& m2) {
        return monom(m1.degree + m2.degree, m1.coef * m2.coef);
    }

    friend bool operator==(const monom& m1, const monom& m2) {
        return m1.coef == m2.coef && m1.degree == m2.degree;
    }

    friend bool operator!=(const monom& m1, const monom& m2) {
        return !(m1 == m2);
    }

    friend bool operator>(const monom& m1, const monom& m2) {
        if (m1.degree != m2.degree) return m1.degree > m2.degree;
        return m1.coef > m2.coef;
    }

    friend bool operator<(const monom& m1, const monom& m2) {
        if (m1.degree != m2.degree) return m1.degree < m2.degree;
        return m1.coef < m2.coef;
    }

    explicit monom(const std::string& str) {
        if (str.empty()) {
            coef = 0.0;
            degree = 0;
            return;
        }
        size_t pos = 0;
        bool isNegative = false;

        if (str[0] == '-') {
            isNegative = true;
            pos = 1;
        }
        
        std::string coeffStr;
        bool hasDot = false;
        bool hasDigit = false;

        while (pos < str.length()) {
            if (isdigit(str[pos])) {
                coeffStr += str[pos];
                hasDigit = true;
                pos++;
            }
            else if (str[pos] == '.' || str[pos] == ',') {
                if (hasDot) {
                    throw "Error";
                }
                hasDot = true;
                coeffStr += '.';
                pos++;
            }
            else {
                break;
            }
        }

        if (!hasDigit && pos < str.length() && str[pos] != 'x' && str[pos] != 'y' && str[pos] != 'z') {
            throw "Error";
        }

        if (coeffStr.empty()) {
            coef = 1.0;
        }
        else {
            try {
                coef = std::stod(coeffStr);
            }
            catch (const std::exception&) {
                throw "Error";
            }
        }

        degree = 0;
        bool hasX = false, hasY = false, hasZ = false;
        int count = 0;

        while (pos < str.length()) {
            char var = str[pos];

            if (var != 'x' && var != 'y' && var != 'z') {
                throw "Error";
            }

            if (var == 'y' && !hasX) {
                throw "Error";
            }

            if (var == 'z' && (!hasX || !hasY)) {
                throw "Error";
            }

            if ((var == 'x' && hasX) || (var == 'y' && hasY) || (var == 'z' && hasZ)) {
                throw "Error";
            }
        
            pos++;

            int deg = 1;

            if (pos < str.length() && str[pos] == '^') {
                pos++;

                if (pos >= str.length() || !isdigit(str[pos])) {
                    throw "Error";
                }

                std::string degStr;
                while (pos < str.length() && isdigit(str[pos])) {
                    degStr += str[pos];
                    pos++;
                }

                try {
                    deg = std::stoi(degStr);
                }
                catch (const std::exception&) {
                    throw "Error";
                }

                if (deg <= 0) {
                    throw "Error";
                }
            }

            if (var == 'x') {
                hasX = true;
                degree += deg * 100;
            }
            else if (var == 'y') {
                hasY = true;
                degree += deg * 10;
            }
            else if (var == 'z') {
                hasZ = true;
                degree += deg;
            }
            count++;
        }

        if (count > 3) {
            throw "Error";
        }

        if (isNegative) {
            coef = -coef;
        }


        if (coef == 0.0) {
            degree = 0;
        }
        
    }
};

class polinom {
private:
    std::forward_list<monom> monoms;

    void insertSorted(const monom& m) {
        if (m.coef == 0.0) return;

        if (monoms.empty()) {
            monoms.push_front(m);
            return;
        }

        auto prev = monoms.before_begin();
        auto curr = monoms.begin();

        while (curr != monoms.end() && curr->degree > m.degree) {
            prev = curr;
            ++curr;
        }

        if (curr != monoms.end() && curr->degree == m.degree) {
            curr->coef += m.coef;
            if (curr->coef == 0.0) {
                monoms.erase_after(prev);
            }
        }
        else {
            monoms.insert_after(prev, m);
        }
    }

    void parsePolinom(const std::string& str) {
        if (str.empty()) return;

        std::vector<std::string> monomStr;
        std::string current;

        for (size_t i = 0; i < str.length(); i++) {
            if (isspace(str[i])) continue;

            if (str[i] == '+') {
                if (!current.empty()) {
                    monomStr.push_back(current);
                    current.clear();
                }
            }
            else if (str[i] == '-') {
                if (!current.empty()) {
                    monomStr.push_back(current);
                }
                current = "-";
            }
            else {
                current += str[i];
            }
        }

        if (!current.empty()) {
            monomStr.push_back(current);
        }

        for (const auto& monomStr : monomStr) {
            if (!monomStr.empty()) {
                try {
                    insertSorted(monom(monomStr));
                }
                catch (const std::exception& e) {
                    throw "Error";
                }
            }
        }
    }
public:
    using iterator = std::forward_list<monom>::iterator;

    polinom() = default;

    explicit polinom(const std::string& str) {
        parsePolinom(str);
    }

    polinom(const monom& m) {
        insertSorted(m);
    }

    iterator begin() { return monoms.begin(); }
    iterator end() { return monoms.end(); }

    bool empty() const { return monoms.empty(); }

    void clear() { monoms.clear(); }

    void print() const {
        if (monoms.empty()) {
            std::cout << "0" << std::endl;
            return;
        }

        bool first = true;
        for (const monom& m : monoms) {
            if (!first && m.coef > 0) {
                std::cout << "+";
            }

            if (m.degree == 0) {
                std::cout << m.coef;
            }
            else {
                if (m.coef == -1.0) {
                    std::cout << "-";
                }
                else if (m.coef != 1.0) {
                    std::cout << m.coef;
                }

                int xDeg = m.degree / 100;
                int yDeg = (m.degree / 10) % 10;
                int zDeg = m.degree % 10;

                if (xDeg > 0) {
                    std::cout << "x";
                    if (xDeg > 1) std::cout << "^" << xDeg;
                }
                if (yDeg > 0) {
                    std::cout << "y";
                    if (yDeg > 1) std::cout << "^" << yDeg;
                }
                if (zDeg > 0) {
                    std::cout << "z";
                    if (zDeg > 1) std::cout << "^" << zDeg;
                }
            }
            first = false;
        }
        std::cout << std::endl;
    }

    polinom operator+(const polinom& other) const {
        polinom res;

        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();

        while (it1 != monoms.end() && it2 != other.monoms.end()) {
            if (it1->degree > it2->degree) {
                res.monoms.push_front(*it1);
                ++it1;
            }
            else if (it1->degree < it2->degree) {
                res.monoms.push_front(*it2);
                ++it2;
            }
            else {
                double sum = it1->coef + it2->coef;
                if (sum != 0.0) {
                    res.monoms.push_front(monom(it1->degree, sum));
                }
                ++it1;
                ++it2;
            }
        }

        while (it1 != monoms.end()) {
            res.monoms.push_front(*it1);
            ++it1;
        }

        while (it2 != other.monoms.end()) {
            res.monoms.push_front(*it2);
            ++it2;
        }

        res.monoms.reverse();

        return res;
    }

    polinom operator-(const polinom& other) const {
        polinom result;

        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();

        while (it1 != monoms.end() && it2 != other.monoms.end()) {
            if (it1->degree > it2->degree) {
                result.insertSorted(*it1);
                ++it1;
            }
            else if (it1->degree < it2->degree) {
                result.insertSorted(monom(it2->degree, -it2->coef));
                ++it2;
            }
            else {
                double diff = it1->coef - it2->coef;
                if (diff != 0.0) {
                    result.insertSorted(monom(it1->degree, diff));
                }
                ++it1;
                ++it2;
            }
        }

        while (it1 != monoms.end()) {
            result.insertSorted(*it1);
            ++it1;
        }

        while (it2 != other.monoms.end()) {
            result.insertSorted(monom(it2->degree, -it2->coef));
            ++it2;
        }

        return result;
    }

    polinom operator*(double c) const {
        polinom result;
        for (const monom& m : monoms) {
            result.insertSorted(m * c);
        }
        return result;
    }

    polinom operator/(double c) const {
        if (c == 0) throw "Error";
        polinom result;
        for (const monom& m : monoms) {
            result.insertSorted(m / c);
        }
        return result;
    }

    bool operator==(const polinom& other) const {
        auto it1 = monoms.begin();
        auto it2 = other.monoms.begin();

        while (it1 != monoms.end() && it2 != other.monoms.end()) {
            if (*it1 != *it2) return false;
            ++it1;
            ++it2;
        }

        return it1 == monoms.end() && it2 == other.monoms.end();
    }

    bool operator!=(const polinom& other) const {
        return !(*this == other);
    }

    polinom operator*(const polinom& other) const {
        if (monoms.empty() || other.monoms.empty()) {
            return polinom();
        }

        polinom res;

        for (const monom& m1 : monoms) {
            for (const monom& m2 : other.monoms) {
                monom m3 = m1 * m2;

                if (m3.coef == 0.0) continue;

                auto prev = res.monoms.before_begin();
                auto curr = res.monoms.begin();

                while (curr != res.monoms.end() && curr->degree > m3.degree) {
                    prev = curr;
                    ++curr;
                }

                if (curr != res.monoms.end() && curr->degree == m3.degree) {
                    curr->coef += m3.coef;
                    if (curr->coef == 0.0) { 
                        res.monoms.erase_after(prev);
                    }
                }
                else {
                    res.monoms.insert_after(prev, m3);
                }
            }
        }

        return res;
    }

    std::pair<polinom, polinom> divide(const polinom& delitel) const {
        if (delitel.monoms.empty()) {
            throw "Error";
        }

        polinom chastnoe;
        polinom ostatok = *this;

        monom ved = delitel.monoms.front();

        while (!ostatok.monoms.empty() && ostatok.monoms.front().degree >= ved.degree) {
            monom ved2 = ostatok.monoms.front();
            if (ved.coef == 0.0) {
                throw "Error";
            }

            int newDegree = ved2.degree - ved.degree;
            double newCoef = ved2.coef / ved.coef;
            monom val(newDegree, newCoef);
            chastnoe.insertSorted(val);

            polinom sub;
            for (const monom& m : delitel.monoms) {
                sub.insertSorted(m * val);
            }
            ostatok = ostatok - sub;
        }

        return { chastnoe, ostatok };
    }

    polinom operator/(const polinom& p) const {
        return divide(p).first;
    }

    polinom operator%(const polinom& p) const {
        return divide(p).second;
    }
};