#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include "polynom.h"

inline std::string trimZeros(std::string s)
{
    if (s.find('.') == std::string::npos) return s;
    while (!s.empty() && s.back() == '0') s.pop_back();
    if (!s.empty() && s.back() == '.') s.pop_back();
    if (s == "-0") s = "0";
    if (s.empty()) s = "0";
    return s;
}

inline std::string formatDouble(double value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(6) << value;
    return trimZeros(oss.str());
}

inline std::string polinomToString(polinom p)
{
    if (p.empty()) return "0";

    std::ostringstream out;
    bool first = true;

    for (auto it = p.begin(); it != p.end(); ++it)
    {
        monom m = *it;
        if (!first && m.coef > 0) out << "+";

        if (m.degree == 0)
        {
            out << formatDouble(m.coef);
        }
        else
        {
            if (m.coef == -1.0)
            {
                out << "-";
            }
            else if (m.coef != 1.0)
            {
                out << formatDouble(m.coef);
            }

            int xDeg = m.degree / 100;
            int yDeg = (m.degree / 10) % 10;
            int zDeg = m.degree % 10;

            if (xDeg > 0)
            {
                out << "x";
                if (xDeg > 1) out << "^" << xDeg;
            }
            if (yDeg > 0)
            {
                out << "y";
                if (yDeg > 1) out << "^" << yDeg;
            }
            if (zDeg > 0)
            {
                out << "z";
                if (zDeg > 1) out << "^" << zDeg;
            }
        }

        first = false;
    }

    return out.str();
}