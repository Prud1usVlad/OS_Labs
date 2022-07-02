#pragma once
#include <iostream>
#include <windows.h>
#include <list>
#include <algorithm>

using namespace std;

// Data structure for storing data about cache frame in the list  
class CacheFrame {
public:
    int key;
    int value;

    CacheFrame(int key_, int value_) {
        key = key_;
        value = value_;
    }
};

// This class uses std::list container for queue implementation,
// because of its constant time insertion and removal operations
class LRUcache {
public:
    LRUcache(int capacity) {
        frames = capacity;
    }

    // Returns value by key
    int get(int key);

    // Puts value using key
    void put(int key, int value);

    list<CacheFrame> queue;
private:
    int frames;
};