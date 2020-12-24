//
// Created by Vladimir on 21.12.2020.
//
#ifndef string_h
#define string_h

#include <iostream>
#include <cstring>

class String {
private:
    size_t sz = 0;
    char* str = nullptr;
    size_t max_size = 0;

public:
//constructors
    String() = default;

    String(size_t sz, char c = '\0'):sz (sz), str(new char[sz]), max_size (sz) {
        //std::cerr << "Sz + c\n";
        memset(str, c, sz);
    }
    String(const String& s):String(s.sz, '\0') {
        //std::cerr << "String\n";
        memcpy(str, s.str, sz);
    }
    String(std::initializer_list<char> lst): sz(lst.size()), str(new char[sz]), max_size(lst.size()) {
        std::copy(lst.begin(), lst.end(), str);
    }
    String(const char* input):sz(strlen(input)), str(new char[sz]), max_size(strlen(input)) {
        std::cerr << "String from char*\n";
        memcpy(str, input, sz);
    }

//methods
    size_t length() const{
        std::cerr << "length\n";
        return sz;
    }

    bool empty(){
        std::cerr << "empty\n";
        if (sz == 0) return true;
        else return false;
    }
    //Метод clear(), позволяющий очистить строку.
    void clear(){
        std::cerr << "clear\n";
        sz = 0;
        max_size = 0;
        delete [] str;
        str = new char[0];
        //*this = String();
    }

    char& front(){
        //std::cerr << "front\n";
        return (str[0]);
    }

    char front() const{
        //std::cerr << "frontc\n";
        return (str[0]);
    }

    char& back(){
        //std::cerr << "back\n";
        return (str[sz-1]);
    }

    char back() const{
        //std::cerr << "back\n";
        return (str[sz-1]);
    }

    void push_back(char s){
        //std::cerr << "push_back\n";
        *this += s;
    }

    void pop_back(){
        //std::cerr << "pop_back\n";
        sz--;
    }

    size_t rfind(const String& s) const{
        std::cerr << "rfind\n";
        String ans;
        if (s.sz > sz) return (sz);
        else {
            size_t i = sz - 1;
            while (i != 0){
                if (str[i] == s.str[0] && i + s.sz <= sz) {
                    int temp = 0;
                    bool no_difference = true;
                    for (size_t j = i; j < i + s.sz; ++j) {
                        if (s.str[temp] == str[j]) temp++;
                        else {
                            no_difference = false;
                            break;
                        }
                    }
                    if (no_difference) return (i);
                    else --i;
                }
                else {
                    --i;
                    continue;
                }
            }
            return (sz);
        }
    }

    size_t find(const String &s) const{
        std::cerr << "find\n";
        String ans;
        if (s.sz > sz) return (sz);
        else {
            size_t i = 0;
            while (i != sz - s.sz){
                if (str[i] != s.str[0]) {
                    i++;
                    continue;
                }
                else{
                    int temp = 0;
                    bool no_difference = true;
                    for (size_t j = i; j < i + s.sz; ++j) {
                        if (s.str[temp] == str[j]) temp++;
                        else {
                            no_difference = false;
                            break;
                        }
                    }
                    if (no_difference) return (i);
                    else ++i;
                }
            }
            return (sz);
        }
    }

    String substr(size_t start, size_t count) const{
        std::cerr << "substr\n";
        String ans = (count);
        for (size_t i = start; i < start + count; ++i){
            ans[i-start] = str[i];
        }
        return(ans);
    }

    //operators
    String& operator=(String s){
        std::cerr << "=\n";
        swap(s);
        return *this;
    }

    String& operator+=(const char& ch){
        //std::cerr << "+=\n";
        bool f = false;
        while (sz + 1 >= max_size) {
            if (max_size == 0){
                max_size = 1;
            }
            else max_size *= 2;
            f = true;
        }
        if (f){
            char* new_str = new char[max_size];
            memcpy (new_str, str, sz);
            delete[] str;
            str = new_str;
        }
        str[sz] = ch;
        ++sz;
        return (*this);
    }

    String& operator+=(const String& s){
        std::cerr << "+= id 1\n";
        if (s.sz > sz){
            size_t temp = sz;
            sz = sz + s.sz;
            max_size = s.max_size;
            while (s.sz + temp > max_size){
                max_size *= 2;
            }
            for (size_t i = temp; i < s.sz + temp; ++i){
                str[i] = s.str[i - temp];
            }
            return (*this);
        }
        else {
            for (size_t i = 0; i < s.sz; ++i) {
                *this += s.str[i];
            }
            std::cerr << "+= id 2\n";
            return (*this);
        }
    }

    char& operator[](size_t index) {
        std::cerr << "[]\n";
        return str[index];
    }

    char operator[](size_t index) const{
        std::cerr << "[]c\n";
        return str[index];
    }

    bool operator==(const String& s) const{
        std::cerr << "==\n";
        if (s.sz != sz) return false;
        else {
            for (size_t i = 0; i < sz; ++i){
                if (str[i] == s.str[i]) continue;
                else return false;
            }
            return true;
        }
    }

    void swap(String& s){
        //std::cerr << "swap\n";
        std::swap(s.sz, sz);
        std::swap(s.max_size, max_size);
        std::swap(s.str, str);
    }
//destructor
    ~String(){
        delete[] str;
    }

    friend std::ostream& operator<< (std::ostream &out, const String& st);
    friend std::istream& operator>> (std::istream &in, String& st);
};

std::ostream& operator<<(std::ostream& out, const String& s) {
    std::cerr << "Cout\n";
    for (size_t i = 0; i < s.length(); ++i){
        out << s[i];
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& s) {
    std::cerr << "Cin\n";
    s = String();
    char temp;
    while (in >> std::noskipws >> temp && temp != ' ' && temp != '\n'){
        s.push_back(temp);
    }
    return in;
}

//Оператор +, позволяющий складывать строки с символами, символы со строками и строки со строками.
String operator+(const String& s, const String& s_1){
    std::cerr << "+ \n";
    String ans = s;
    ans += s_1;
    return(ans);
}

String operator+(const String& s, const char& s_1){
    std::cerr << "+ \n";
    String ans = s;
    ans += s_1;
    return(ans);
}

String operator+(const char& s, const String& s_1){
    std::cerr << "+ \n";
    String ans = {s};
    ans += s_1;
    return(ans);
}

#endif //string_h
