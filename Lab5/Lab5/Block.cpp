#include "Block.h"

Block::Block(DWORD adress, DWORD size, DWORD _state, DWORD _protect) {
	dwAdress = adress;
	dwSize = size;
	state = _state;
	protect = _protect;
}
