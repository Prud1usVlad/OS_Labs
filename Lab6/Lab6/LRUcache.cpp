#include "LRUcache.h"
#include <list>

int LRUcache::get(int key) {
    int value = INT16_MIN;
    // Search for the element
    list<CacheFrame>::iterator iterator = queue.begin();
    for (int i = 0; i < queue.size(); ++i, ++iterator) {
        // Delete from queue if found
        if ((*iterator).key == key) {
            queue.erase(iterator);
            value = (*iterator).value;
            break;
        }
    }

    if (value != INT16_MIN) {
        // Add Frame 
        queue.push_back(CacheFrame(key, value));
    }

    return value;

}

void LRUcache::put(int key, int value) {

    // Search for the element
    list<CacheFrame>::iterator iterator = queue.begin();
    for (int i = 0; i < queue.size(); ++i, ++iterator) {
        // Delete from queue if found
        if ((*iterator).key == key) {
            queue.erase(iterator);
            break;
        }
    }

    // If there is no empty frames
    if (frames == queue.size()) {
        // Erase least recently used (at the begining)
        queue.erase(queue.begin());
    }

    // Add Frame 
    queue.push_back(CacheFrame(key, value));
}