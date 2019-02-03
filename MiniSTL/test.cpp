#include <iostream>
#include "String.h"

// using namespace std;

int main() {
    string s;
    char c = 'h';
    s.push_back(c);
    std::cout << s.back() << std::endl;
}