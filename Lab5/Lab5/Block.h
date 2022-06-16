#pragma once
#include <Windows.h>

class Block {
public:
	DWORD dwAdress;
	DWORD dwSize;
	DWORD state;
	DWORD protect;

	Block(DWORD adress = 0, DWORD size = 0, DWORD _state = 0, DWORD _protect = 0);
};

