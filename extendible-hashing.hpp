#include <iostream>
#include <map>
#include <vector>
#include <unordered_set>
#include <assert.h>
#ifndef EH_H
#define EH_H
#define DEFAULT_BUCKET_SIZE 2
using namespace std;
template<class T>
class Bucket{
public:
    Bucket(int b_size);
    Bucket(int b_size, int b_depth);
    ~Bucket();
    T* search(int key);
    int remove(int key);
    int update(int key, T&& value);
    int insert(int key, T&& value);
    bool isFull(void);
    bool isEmpty(void);
    int getDepth(void);
    int getSize(void);
    int getDictSize(void);
    int incrDepth(void);
    int decrDepth(void);
    map<int, T> copy(void);
    void clear(void);
    void display(void);
private:
    int size, depth;
    std::map<int, T> dict;
};

template<class T>
class Directory{
public:
    Directory();
    Directory(int bucket_size);
    Directory(int size, int bucket_size);
    T* search(int key);
    int remove(int key);
    int update(int key, T&& value);
    int insert(int key, T&& value);
    inline 
    int idx_dir(int key){
        return key & ((1<<depth) -1);
    }
    inline 
    int pair_idx(int key, int local_depth){
        return  key ^ (1 << (local_depth - 1));
    }
    void display(void);
    ~Directory();
private:
    int size, depth;
    vector<Bucket<T>*> buckets;
    void double_dir(void);
    int halve_dir(void);
    int split_bucket(int bucket_no);
    void merge_bucket(int bucket_no);
};
#include "extendible-hashing.cpp"
#endif