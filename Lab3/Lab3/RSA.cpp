#include "pch.h"
#include <iostream>
#include <vector>
#include "RSA.h"
#include <math.h>


// sets open and closed keys
void GetKeys(unsigned __int64* n, unsigned long* e, unsigned __int64* d) {

    // generates prime nums
    srand(time(NULL));
    unsigned long p = Prime(((double)rand() / RAND_MAX) * INT16_MAX);
    unsigned long q = Prime(((double)rand() / RAND_MAX) * INT16_MAX);

    // euler func val
    unsigned __int64 t = (unsigned __int64)(p - 1ul) * (unsigned __int64)(q - 1ul);

    // initializing
    *n = (unsigned __int64)p * (unsigned __int64)q;
    *e = GetE(t);
    *d = GetD(*e, t);
}

// returns prime number less then n
unsigned long Prime(unsigned long n) {
    unsigned long res = 2;

    if (n < 6 && n > 2) {
        return 3;
    }
    else if (n == 2) {
        return 2;
    }

    unsigned long last = n % 6;
    unsigned long i = (last == 0) ? n : n - last;

    while (res < 6) {
        if (Ferma(i + 1)) {
            res = i + 1;
            break;
        }

        if (Ferma(i - 1)) {
            res = i - 1;
            break;
        }
        i -= 6;
    }
    return res;
}

// ferma's test, is x prime
bool Ferma(unsigned long x) {
    if (x == 2)
        return true;
    srand(time(0));
    for (int i = 0; i < 100; i++) {
        unsigned long a = (rand() % (x - 2ul)) + 2ul;
        if (GCD(a, x) != 1)
            return false;
        if (Pows(a, x - 1ul, x) != 1)
            return false;
    }
    return true;
}

// gratest common divisor
unsigned long GCD(unsigned long a, unsigned long b) {
    if (b == 0)
        return a;
    return GCD(b, a % b);
}

// (auxilary) binary multiplication -> (a*b)%m
unsigned __int64 Mul(unsigned __int64 a, unsigned __int64 b, unsigned __int64 m) {
    if (b == 1ull)
        return a;
    if (b % 2ull == 0) {
        unsigned __int64 t = Mul(a, b / 2ull, m);
        return (2ull * t) % m;
    }
    return (Mul(a, b - 1ull, m) + a) % m;
}

// (auxilary) binary power func -> (a^b)%m
unsigned __int64 Pows(unsigned __int64 a, unsigned __int64 b, unsigned __int64 m) {
    if (b == 0)
        return 1;
    if (b % 2ull == 0) {
        unsigned __int64 t = Pows(a, b / 2ull, m);
        return Mul(t, t, m) % m;
    }
    return (Mul(Pows(a, b - 1ull, m), a, m)) % m;
}

// calculates open exponent
unsigned __int64 GetE(unsigned __int64 t)
{
    unsigned __int64 e;

    for (e = 3; e < t; e++)
    {
        if (GCD(e, t) == 1)
        {
            return e;
        }
    }

    return 0;
}

// calculates hiden exponent 
unsigned __int64 GetD(unsigned __int64 e, unsigned __int64 t)
{
    unsigned __int64 res = 0;

    for (int i = 2; i < 100; i++) {
        res = ((t * i) + 1) / e;

        if ((res * e) % t == 1) return res;
    }

    return 0;
}

// encrypts i, with open key {e, n}
unsigned long EncryptChar(long i, unsigned long e, unsigned __int64 n)
{
    return Pows(i, e, n);
}

// decrypts i, with hiden key {d, n}
unsigned long DecryptChar(long i, unsigned __int64 d, unsigned __int64 n)
{
    return Pows(i, d, n);
}

// encrypts input string with open key {e, n}
std::vector<unsigned long> EncryptStr(std::string message, unsigned long e, unsigned __int64 n) {

    std::vector<unsigned long> res;

    for (int i = 0; i < message.size(); i++) {
        res.push_back(EncryptChar(message[i], e, n));
    }

    return res;

}

// decrypts vector of encrypted 
std::string DecryptStr(std::vector<unsigned long> e_message, unsigned long d, unsigned __int64 n) {

    std::string res = "";

    for (int i = 0; i < e_message.size(); i++) {
        res.push_back(DecryptChar(e_message[i], d, n));
    }

    return res;
}