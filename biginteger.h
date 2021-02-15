//
// Created by Яфаров Владимир on 01.02.2021.
//
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

class BigInteger {
private:
    static const long long size_of_block = 1000000000;
    static const int num_size = 9; //зависит от size_of_block (кол-во цифр)
    bool sign_neg;
    std::vector<int> digits;
    static void plus(std::vector<int>& res, const std::vector<int>& a, const std::vector<int>& b) {
        int memory = 0;
        for (size_t i = 0; (i < std::max(a.size(), b.size())) || memory != 0; ++i) {
            if (i == a.size()) res.push_back(0);
            res[i] = a[i] + memory + (i < b.size() ? b[i] : 0);
            memory = res[i] >= size_of_block;
            if (memory != 0) res[i] -= size_of_block;
        }
    }
    static void minus(std::vector<int>& res, const std::vector<int>& a, const std::vector<int>& b) {
        int memory = 0;
        for(size_t i = 0; i < b.size() || memory != 0; ++i) {
            res[i] = a[i] - memory - (i < b.size() ? b[i] : 0);
            memory = res[i] < 0;
            if(memory != 0) res[i] += size_of_block;
        }
    }
    static void multi(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& res) {
        for(size_t i = 0; i < a.size(); ++i){
            int memory = 0;
            for (long long j = 0; j < static_cast<long long>(b.size()) || memory != 0; ++j) {
                long long curr = res[i + j] + a[i] * 1ll * (j < static_cast<long long>(b.size()) ? b[j] : 0) + memory;
                res[i + j] = int(curr % size_of_block);
                memory = curr / size_of_block;
            }
        }
        while(res.size() > 1 && res.back() == 0)
            res.pop_back();
    }
public:
    BigInteger(int a);
    BigInteger(std::string str);
    BigInteger();
    void clear();

    friend std::ostream& operator <<(std::ostream& os, const BigInteger& a);
    friend std::istream& operator >>(std::istream& in, const BigInteger& a);

    std::string toString() const;
    void remove_zeros();
    void shift_right();

    BigInteger& operator=(const BigInteger&);

    const BigInteger operator +() const;
    const BigInteger operator -() const;
    BigInteger& operator ++();
    BigInteger operator ++(int);
    BigInteger operator --();
    BigInteger operator --(int);

    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator!=(const BigInteger&, const BigInteger&);
    friend bool operator <(const BigInteger&, const BigInteger&);
    friend bool operator<=(const BigInteger&, const BigInteger&);
    friend bool operator >(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);

    friend BigInteger operator +(BigInteger left, const BigInteger& right);
    friend BigInteger operator -(BigInteger left, const BigInteger& right);
    BigInteger& operator +=(const BigInteger &);
    BigInteger& operator -=(const BigInteger&);
    friend BigInteger operator *(const BigInteger&, const BigInteger&);
    friend BigInteger operator /(const BigInteger&, const BigInteger&);
    BigInteger& operator *=(const BigInteger&);
    BigInteger& operator /=(const BigInteger&);
    friend BigInteger operator %(const BigInteger&, const BigInteger&);
    BigInteger& operator %=(const BigInteger&);

    ~BigInteger() = default;
    explicit operator bool();
};
BigInteger::operator bool() {
    return *this != BigInteger(0);
}

//конструкторы
BigInteger::BigInteger() {
    //std::cerr << "create Big ()" << '\n';
    this->sign_neg = false;
    this->digits.push_back(0);
    this->remove_zeros();
}
BigInteger::BigInteger(int a){
    //std::cerr << "create Big int" << '\n';
    std::string str = std::to_string(a);
    *this = BigInteger(str);
    this->remove_zeros();
}
BigInteger::BigInteger(std::string str) {
    //std::cerr << "create Big string" << '\n';
    if (str.empty()) this->sign_neg = false;
    else {
        if (str[0] == '-') {
            this->sign_neg = true;
            str = str.substr(1);
        }
        else {
            this->sign_neg = false;
        }
        int num = 0;
        std::string temp_str;
        for (long long i = str.size(); i > 0; i -= num_size) {
            if (i < num_size) temp_str = str.substr(0, i);
            else temp_str = str.substr(i - num_size, num_size);
            for (int j = int(temp_str.size()) - 1; j >= 0; --j) {
                num += pow(10,temp_str.size() - 1 - j) * (temp_str[j] - 48);
                if (j == 0) break;
            }
            this->digits.push_back(num);
            num = 0;
        }
    }
    this->remove_zeros();
}

//присвоение
BigInteger& BigInteger::operator=(const BigInteger& a) = default;

//вывод и ввод числа
std::ostream& operator <<(std::ostream& os, const BigInteger& a){
    //std::cerr << "create Big <<" << '\n';
    os << a.toString();
    return os;
}
std::istream& operator >>(std::istream& in, BigInteger& a) {
    //std::cerr << "create Big >>" << '\n';
    std::string s;
    a.clear();
    in >> s;
    a = s;
    return in;
}

//перевод в строку
std::string BigInteger::toString() const {
    //std::cerr << "create Big toString" << '\n';
    std::string s;
    if (digits.empty()) return "0";
    if (sign_neg) {
        s += '-';
    }
    int nul_det = 1;
    for (int i = int(digits.size()) - 1; i >= 0; --i) {
        if (i != int(digits.size()) - 1 && digits[i] < size_of_block){
            while (digits[i] * pow(10, nul_det) < size_of_block){
                s += "0";
                nul_det++;
                if (nul_det == num_size) break;
            }
        }
        nul_det = 1;
        s += std::to_string(digits[i]);
    }
    return s;
}

//унарный + и -
const BigInteger BigInteger::operator -() const {
    //std::cerr << "create Big gay -" << '\n';
    BigInteger temp = *this;
    if (*this == 0) return temp;
    if (this->sign_neg) temp.sign_neg = false;
    else temp.sign_neg = true;
    return temp;
}

//сравнение чисел
bool operator ==(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big ==" << '\n';
    if (left.sign_neg != right.sign_neg) return false;
    if (left.digits.size() != right.digits.size()) return false;
    if (left.digits.empty()) {
        if (right.digits.empty() || (right.digits.size() == 1 && right.digits[0] == 0)) return true;
        else return false;
    }
    if (right.digits.empty()) {
        if (left.digits.size() == 1 && left.digits[0] == 0) return true;
        else return false;
    }
    for (size_t i = 0; i < left.digits.size(); ++i) {
        if (left.digits[i] != right.digits[i]) return false;
    }
    return true;
}
bool operator !=(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big !=" << '\n';
    return !(left == right);
}
bool operator  <(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big <" << '\n';
    if (left == right) return false;
    if (left.sign_neg) {
        if (right.sign_neg) {
            if (left.digits.size() != right.digits.size()) {
                return left.digits.size() > right.digits.size();
            }
            else {
                for (int i = int(left.digits.size() - 1); i >= 0; --i) {
                    if (left.digits[i] != right.digits[i]) return left.digits[i] > right.digits[i];
                }
                return false;
            }
        }
        else return true;
    }
    else if (right.sign_neg) return false;
    else {
        if (left.digits.size() != right.digits.size()) {
            return left.digits.size() < right.digits.size();
        }
        else {
            for (int i = int(left.digits.size() - 1); i >= 0; --i) {
                if (left.digits[i] != right.digits[i]) return left.digits[i] < right.digits[i];
            }
            return false;
        }
    }
}
bool operator  >(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big >" << '\n';
    if (left == right) return false;
    if (left < right)  return false;
    return true;
}
bool operator >=(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big >=" << '\n';
    if (left > right)   return true;
    if (left == right)  return true;
    else return false;
}
bool operator <=(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big <=" << '\n';
    if (left < right)   return true;
    if (left == right)  return true;
    else return false;
}

//операции с числами
BigInteger operator +(BigInteger left, const BigInteger& right) {
    return left += right;
}
BigInteger operator -(BigInteger left, const BigInteger& right) {
    std::cerr << "create Big -" << '\n';
    return left -= right;
}
BigInteger& BigInteger::operator +=(const BigInteger &a) {
    //std::cerr << "create Big +=" << '\n';
    if(sign_neg == a.sign_neg) plus(digits, digits, a.digits);
    else {
        if(sign_neg) {
            sign_neg = false;
            if(*this < a) {
                std::vector<int> temp = a.digits;
                std::swap(digits, temp);
                minus(digits, digits, temp);
            }
            else {
                minus(digits, digits, a.digits);
                if(*this != 0) sign_neg = true;
            }
        }
        else {
            sign_neg = true;
            if(*this > a) {
                std::vector<int> temp = a.digits;
                std::swap(digits, temp);
                minus(digits, digits, temp);
            }
            else {
                minus(digits, digits, a.digits);
                sign_neg = false;
            }
        }
    }
    return *this;
}
BigInteger& BigInteger::operator -=(const BigInteger& a) {
    //std::cerr << "create Big -=" << '\n';
    if(!sign_neg && !a.sign_neg) {
        if(*this < a) {
            sign_neg = true;
            std::vector<int> temp = a.digits;
            std::swap(digits, temp);
            minus(digits, digits, temp);
        }
        else minus(digits, digits, a.digits);
    }
    else if(sign_neg && a.sign_neg) {
        if(*this > a) {
            sign_neg = false;
            std::vector<int> tmp = a.digits;
            std::swap(digits, tmp);
            minus(digits, digits, tmp);
        }
        else if(*this < a) {
            sign_neg = true;
            minus(digits, digits, a.digits);
        }
        else if(*this == a) {
            *this = 0;
            sign_neg = false;
        }
    }
    else {
        if(sign_neg && !a.sign_neg) {
            sign_neg = false;
            *this += a;
            sign_neg = true;
        }
        else {
            plus(digits, digits, a.digits);
        }
    }
    this->remove_zeros();
    return *this;
}
BigInteger operator *(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big *" << '\n';
    BigInteger a = left;
    return a *= right;
} //ok
BigInteger operator /(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big /" << '\n';
    BigInteger b = right;
    b.sign_neg = false;
    BigInteger result, current;
    result.digits.resize(left.digits.size());
    for (long long i = static_cast<long long>(left.digits.size()) - 1; i >= 0; --i) {
        current.shift_right();
        current.digits[0] = left.digits[i];
        current.remove_zeros();
        int x = 0, l = 0, r = BigInteger::size_of_block;
        while (l <= r) {
            int m = (l + r) / 2;
            BigInteger t = b * m;
            if (t <= current) {
                x = m;
                l = m + 1;
            }
            else r = m - 1;
        }
        result.digits[i] = x;
        current = current - b * x;
    }
    result.sign_neg = left.sign_neg != right.sign_neg;
    result.remove_zeros();
    return result;
} //ok
BigInteger& BigInteger::operator *=(const BigInteger& a) {
    //std::cerr << "create Big *=" << '\n';
    std::vector<int> res(digits.size() + a.digits.size(), 0);
    multi(digits, a.digits, res);
    digits = res;
    if(digits.size() == 1 && digits[0] == 0)
        sign_neg = false;
    else {
        if (sign_neg != a.sign_neg) sign_neg = true;
        else sign_neg = false;
    }
    return *this;
} //ok
BigInteger& BigInteger::operator /=(const BigInteger& a) {
    //std::cerr << "create Big /=" << '\n';
    *this = *this / a;
    return (*this);
} //ok
BigInteger operator %(const BigInteger& left, const BigInteger& right) {
    //std::cerr << "create Big %" << '\n';
    BigInteger res = left;
    return res %= right;
}
BigInteger& BigInteger::operator %=(const BigInteger& a) {
    //std::cerr << "create Big %=" << '\n';
    return *this -= *this / a * a;
}


//сдвиг
void BigInteger::shift_right() {
    if (digits.empty()) {
        this->digits.push_back(0);
        return;
    }
    this->digits.push_back(this->digits[this->digits.size() - 1]);
    for (size_t i = this->digits.size() - 2; i > 0; --i) this->digits[i] = this->digits[i - 1];
    this->digits[0] = 0;
}

//префиксный и постфиксный инкремент и декремент
BigInteger& BigInteger::operator++() {
    //std::cerr << "create Big i++" << '\n';
    return *this += 1;
}
BigInteger BigInteger::operator ++(int) {
    //std::cerr << "create Big ++i" << '\n';
    BigInteger temp = *this;
    ++*this;
    return temp;
}
BigInteger BigInteger::operator --() {
    //std::cerr << "create Big i--" << '\n';
    return *this -= 1;
} //ok
BigInteger BigInteger::operator --(int) {
    BigInteger temp = *this;
    --*this;
    return temp;
}

void BigInteger::remove_zeros() {
    //std::cerr << "create Big rem_zer" << '\n';
    while (this->digits.size() > 1 && this->digits.back() == 0) {
        this->digits.pop_back();
    }
    if (this->digits.size() == 1 && this->digits[0] == 0) this->sign_neg = false;
} //ok
void BigInteger::clear(){
    //std::cerr << "create Big clear" << '\n';
    digits.clear();
    sign_neg = false;
} //ok

// Числа Rational должны представляться в виде несократимых обыкновенных дробей,
// где числитель и знаменатель – сколь угодно длинные целые числа.
// Должны поддерживаться операции:
//конструктор из BigInteger, из int
//сложение, вычитание, умножение, деление, составное присваивание с этими операциями
//унарный минус
//операторы сравнения == != < > <= >=
//метод toString(), возвращающий строковое представление числа (вида [минус]числитель/знаменатель), где числитель и знаменатель - взаимно простые числа; если число на самом деле целое, то знаменатель выводить не надо
//метод asDecimal(sizet precision=0), возвращающий строковое представление числа в виде десятичной дроби с precision знаками после запятой
//оператор приведения к double

class Rational {
public:
    Rational(const Rational& q);
    Rational(int n = 0);
    Rational(const BigInteger& numerator);

    Rational operator-() const;

    friend bool operator==(const Rational&, const Rational&);
    friend bool operator <(const Rational&, const Rational&);
    friend bool operator!=(const Rational&, const Rational&);
    friend bool operator<=(const Rational&, const Rational&);
    friend bool operator >(const Rational&, const Rational&);
    friend bool operator>=(const Rational&, const Rational&);

    std::string toString() const;
    std::string asDecimal(size_t precision = 0) const;

    Rational& operator+=(const Rational&);
    Rational& operator-=(const Rational&);
    Rational& operator*=(const Rational&);
    Rational& operator/=(const Rational&);
    Rational& operator =(const Rational&);

private:
    BigInteger numerator = 0;
    BigInteger denominator = 1;

    void update();
};
Rational operator +(Rational a, const Rational& b);
Rational operator -(Rational a, const Rational& b);
Rational operator *(Rational a, const Rational& b);
Rational operator /(Rational a, const Rational& b);


//конструктор
Rational::Rational(int x): numerator(x), denominator(1) {};
Rational::Rational(const BigInteger& x): numerator(x), denominator(1) {};
Rational::Rational(const Rational &a) = default;

//to_string
std::string Rational::toString() const {
    //std::cerr << "create Rational to_str" << '\n';
    if (denominator == 1) return numerator.toString();
    if (numerator == 0) return "0";
    return numerator.toString() + "/" + denominator.toString();
}//ok

//сравнение чисел
bool operator==(const Rational &a, const Rational &b) {
    //std::cerr << "create Rational ==" << '\n';
    return (a.numerator == b.numerator) && (a.denominator == b.denominator);
}
bool operator!=(const Rational &a, const Rational &b) {
    //std::cerr << "create Rational !=" << '\n';
    return !(a == b);
}
bool operator >(const Rational &a, const Rational &b) {
    //std::cerr << "create Rational >" << '\n';
    return !(a <= b);
}
bool operator <(const Rational &a, const Rational &b) {
    //std::cerr << "create Rational <" << '\n';
    return (a.numerator * b.denominator < b.numerator * a.denominator);
}
bool operator>=(const Rational &a, const Rational &b) {
    //std::cerr << "create Rational >=" << '\n';
    return !(a < b);
}
bool operator<=(const Rational &a, const Rational &b) {
    //std::cerr << "create Rational <=" << '\n';
    return !(a > b);
}

//унарный -
Rational Rational::operator-() const{
    std::cerr << "create Rational -()" << '\n';
    Rational copy = *this;
    copy.numerator *= -1;
    return copy;
}//оптимизм

//операции с числами
Rational& Rational::operator +=(const Rational& a) {
    std::cerr << "create Rational +=" << '\n';
    numerator = numerator * a.denominator + a.numerator * denominator;
    denominator *= a.denominator;
    update();
    return *this;
}//
Rational& Rational::operator -=(const Rational& a) {
    std::cerr << "create Rational -=" << '\n';
    numerator = numerator * a.denominator - a.numerator * denominator;
    denominator *= a.denominator;
    update();
    return *this;
}//
Rational operator-(Rational a, const Rational& b) {
    std::cerr << "create Rational -" << '\n';
    a -= b;
    return a;
}//ok
Rational operator+(Rational a, const Rational& b) {
    a += b;
    return a;
}//ok
Rational& Rational::operator *=(const Rational& a) {
    std::cerr << "create Rational *=" << '\n';
    numerator *= a.numerator;
    denominator *= a.denominator;
    update();
    return *this;
}//
Rational& Rational::operator /=(const Rational& a) {
    std::cerr << "create Rational /=" << '\n';
    numerator *= a.denominator;
    denominator *= a.numerator;
    update();
    return *this;
}//
Rational operator*(Rational a, const Rational& b) {
    a *= b;
    return a;
}//ok
Rational operator/(Rational a, const Rational& b) {
    std::cerr << "create Rational /" << '\n';
    a /= b;
    return a;
}//ok

//присваивание
Rational &Rational::operator=(const Rational &a) {
    std::cerr << "create Rational =" << '\n';
    numerator   = a.numerator;
    denominator = a.denominator;
    return *this;
}

//add func
std::string Rational::asDecimal(size_t precision) const {
    std::string s;
    if (numerator < 0)
        s += '-';
    BigInteger number = numerator;
    if (numerator < 0) number = -number;
    BigInteger temp = number / denominator;
    BigInteger rest = number % denominator;
    s += temp.toString();
    if (precision > 0) {
        s += '.';
        for (size_t i = 0; i < precision; ++i) {
            rest *= 10;
            BigInteger news = rest / denominator;
            s += news.toString();
            rest %= denominator;
        }
    }
    return s;
}

//hard func
void Rational::update() {
    //std::cerr << "create Rational update" << '\n';
    BigInteger a = numerator;
    BigInteger b = denominator;
    if (a < 0) a *= -1;
    if (b < 0) b *= -1;
    while (b) {
        a %= b;
        std::swap(a, b);
    }
    if (denominator < 0) a *= -1;
    numerator /= a;
    denominator /= a;
}
