#pragma once
#include <random>
#include <iostream>
using namespace std;

mt19937_64 eng(time(0)); uniform_int_distribution<long long> distr; // distr(eng)

int int_gcd(int l, int r) {
    if(r == 0)
        return l;
    return int_gcd(r, l%r);
}

using cmp_func = bool (*)(pair <int, int>, pair <int, int>);

ostream& operator <<(ostream& os, const pair <int, int> &p) {
    return os << "{" << p.first << "," << p.second << "}";
}