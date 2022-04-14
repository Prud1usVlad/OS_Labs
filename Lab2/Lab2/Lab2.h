#pragma once

#include <vector>
#include <string>

#ifdef LAB2_EXPORTS
	#define LAB2_API __declspec(dllexport)
#else
	#define LAB2_API __declspec(dllimport)
#endif

LAB2_API void GetKeys(unsigned __int64* n, unsigned long* e, unsigned __int64* d);
LAB2_API unsigned __int64 GetE(unsigned __int64 t);
LAB2_API unsigned __int64 GetD(unsigned __int64 e, unsigned __int64 t);
LAB2_API unsigned long EncryptChar(long i, unsigned long e, unsigned __int64 n);
LAB2_API unsigned long DecryptChar(long i, unsigned __int64 d, unsigned __int64 n);
LAB2_API std::vector<unsigned long> EncryptStr(std::string message, unsigned long e, unsigned __int64 n);
LAB2_API std::string DecryptStr(std::vector<unsigned long> e_message, unsigned long d, unsigned __int64 n);

unsigned long Prime(unsigned long n);
bool Ferma(unsigned long x);
unsigned long GCD(unsigned long a, unsigned long b);
unsigned __int64 Mul(unsigned __int64 a, unsigned __int64 b, unsigned __int64 m);
unsigned __int64 Pows(unsigned __int64 a, unsigned __int64 b, unsigned __int64 m);