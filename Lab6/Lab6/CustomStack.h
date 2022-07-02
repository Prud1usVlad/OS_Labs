#pragma once

#include <Windows.h>
#include <iostream>

class CustomStack
{
public:
	CustomStack(SIZE_T heapSize = 0);

	~CustomStack();
	
	// Removes and returns top element of the stack, INT16_MIN otherwise
	int pop();

	// Returnes top element of the stack, INT16_MIN otherwise
	int top();

	// Adds element to the stack
	void push(int item);

	int count; 

private: 
	HANDLE _heapHandle;
	LPVOID _allocatedMem;
	SIZE_T _heapSize;
	SIZE_T _allocatedSize;
};

