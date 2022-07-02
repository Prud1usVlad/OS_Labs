#include "CustomStack.h"


CustomStack::CustomStack(SIZE_T heapSize) {

	_heapHandle = HeapCreate(HEAP_GENERATE_EXCEPTIONS, heapSize, heapSize);
	_allocatedMem = HeapAlloc(_heapHandle, HEAP_ZERO_MEMORY, 4096);

	count = 0;
	_heapSize = (heapSize) ? heapSize : 4096;
	_allocatedSize = 4096;
}

CustomStack::~CustomStack() {
	HeapFree(_heapHandle, 0, _allocatedMem);
	HeapDestroy(_heapHandle);
}

void CustomStack::push(int item) {

	if (sizeof(int) * count >= _allocatedSize) {
		_allocatedMem = HeapReAlloc(_heapHandle, HEAP_ZERO_MEMORY, _allocatedMem, _allocatedSize * 2);
		_allocatedSize *= 2;
		std::cout << "Memory expanded" << std::endl;
	}

	memcpy((int *)_allocatedMem + count, &item, sizeof(int));
	count++;


}

int CustomStack::pop() {
	if (count != 0) {
		return ((int*)_allocatedMem)[--count];
	}
	else {
		return INT16_MIN;
	}
}

int CustomStack::top() {
	if (count != 0) {
		return ((int*)_allocatedMem)[--count];
	}
	else {
		return INT16_MIN;
	}
}

