#pragma once
#include <vector>
#include <string>

void __stdcall GetKeys(unsigned __int64* n, unsigned long* e, unsigned __int64* d);
unsigned __int64 __stdcall GetE(unsigned __int64 t);
unsigned __int64 __stdcall GetD(unsigned __int64 e, unsigned __int64 t);
unsigned long __stdcall EncryptChar(long i, unsigned long e, unsigned __int64 n);
unsigned long __stdcall DecryptChar(long i, unsigned __int64 d, unsigned __int64 n);
std::vector<unsigned long> __stdcall EncryptStr(std::string message, unsigned long e, unsigned __int64 n);
std::string __stdcall DecryptStr(std::vector<unsigned long> e_message, unsigned long d, unsigned __int64 n);

unsigned long __stdcall Prime(unsigned long n);
bool __stdcall Ferma(unsigned long x);
unsigned long __stdcall GCD(unsigned long a, unsigned long b);
unsigned __int64 __stdcall Mul(unsigned __int64 a, unsigned __int64 b, unsigned __int64 m);
unsigned __int64 __stdcall Pows(unsigned __int64 a, unsigned __int64 b, unsigned __int64 m);