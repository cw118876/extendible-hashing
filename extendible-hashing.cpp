#include "extendible-hashing.hpp"


//template class Directory<string>;

template<class T>
Bucket<T>::Bucket(int b_size): size{b_size}, depth{1}{};

template<class T>
Bucket<T>::Bucket(int b_size, int b_depth): size{b_size}, depth{b_depth}{};

template<class T>
T* Bucket<T>::search(int key){
    if (dict.find(key) != dict.end()){
        return &dict[key];
    }
    return nullptr;
}
template<class T> 
int Bucket<T>::remove(int key){
    if (search(key)){
        dict.erase(key);
        return 0;
    }
    return 1;
}
template<class T>
int Bucket<T>::update(int key, T&& value){
    if (search(key)){
        dict[key] = move(value);
        return 1;
    }
    return 1;
}
template<class T>
int Bucket<T>::insert(int key, T&& value){
    if (search(key)){  
        return 1;
    }
    dict.insert(make_pair(key, move(value)));
    return 0;
}
template<class T>
bool Bucket<T>::isFull(void){
    if (dict.size() < size){
        return false;
    }
    return true;
}
template<class T>
bool Bucket<T>::isEmpty(void){
    if (dict.size() != 0)
        return false;
    return true;       
}
template<class T>
int Bucket<T>::getDepth(void){
    return depth;
}

template<class T>
int Bucket<T>::getSize(void){
    return size;
}
template<class T>
int Bucket<T>::getDictSize(void){
    return dict.size();
}
template<class T>
map<int, T> Bucket<T>::copy(void){
    map<int, T> temp(dict);
    return temp;
}
template<class T>
void Bucket<T>::clear(void){
    dict.clear();
}
template<class T>
int Bucket<T>::incrDepth(void){
    depth++;
    return depth;
}
template<class T>
int Bucket<T>::decrDepth(void){
    depth--;
    return depth;
}
template<class T>
Bucket<T>::~Bucket(void){
    clear();
}
template<class T>
void Bucket<T>::display(void){
    cout << "local depth: " << depth << " size: " << size << endl;
    for (auto it = dict.begin(); it != dict.end(); ++it)
        cout << it->first << " " << it->second << endl;
}

template<class T>
Directory<T>::Directory():size {2}, depth {1}{
    Bucket<T>* temp = new Bucket<T>(DEFAULT_BUCKET_SIZE, 0);
    for (int i= 0; i < size; ++i){
        buckets.push_back(temp);
    }
}


template<class T>
Directory<T>::Directory(int dir_depth, int bucket_size):size {1<<dir_depth}, depth {dir_depth}{
    Bucket<T>* temp = new Bucket<T>(bucket_size, 0);
    for (int i= 0; i < size; ++i){
        buckets.push_back(temp);
    }
}


template<class T>
T* Directory<T>::search(int key){
    int idx = idx_dir(key);
    return buckets[idx]->search(key);
}
template<class T>
int Directory<T>::update(int key, T&& value){
    int idx = idx_dir(key);
    return buckets[idx]->update(key, move(value));
}

template<class T>
int Directory<T>::insert(int key, T&& value){
    int idx = idx_dir(key);
    // buckets is not full
    if (!buckets[idx]->isFull())
        return buckets[idx]->insert(key, move(value));
    if (depth <= buckets[idx]->getDepth()){
        // double the directory
        double_dir();
        idx = idx_dir(key);
    }
    int result = split_bucket(idx); // split the bucket
    if (result)
        return result;
    return insert(key, move(value)); // insert the value.
}
template<class T>
void Directory<T>::double_dir(){
    int nums = 1<<depth;
    /* just copy the original bucket *  when double the directory */
    for (int i = 0; i <nums; ++i){
        buckets.push_back(buckets[i]);
    }
    depth++;
    size = 1<< depth;
}
template<class T>
int Directory<T>::split_bucket(int idx){
    assert(buckets[idx]->isFull());
    map<int, T> temp = buckets[idx]->copy();

    int depth_bkt = buckets[idx]->incrDepth();
    int result = 0;
    buckets[idx]->clear();
    int idx2 = pair_idx(idx, depth_bkt); // index of bucket of matched bucket
    Bucket<T>* p_bkt = new Bucket<T> (buckets[idx]->getSize(),  depth_bkt);

    /* update the directory entry */
    int step = 1 << depth_bkt;
    int rg_value = 1 << depth;
    for (int i = idx2; i < rg_value; i+= step)
        buckets[i] = p_bkt;
    for (int i = idx2 - step ; i>= 0; i-=step)
        buckets[i] = p_bkt;

    // insert the the bucket's origin residual.
    for (auto it = temp.begin(); it != temp.end(); ++it){
        result = insert(it->first,  move(it->second));
        if (result)
            return result;
    }
    return 0;
}
template<class T>
int Directory<T>::remove(int key){
    int idx = idx_dir(key);
    int result = buckets[idx]->remove(key);
    if (result)
        return result; // not find the matched item
    /* we need merge some bucket when local depth greater than 1 and buckets become empty */
    if (buckets[idx]->isEmpty() && buckets[idx]->getDepth() > 1)
        merge_bucket(idx);  
    return result;
}
template<class T>
void Directory<T>::merge_bucket(int idx){
    int depth_bkt = buckets[idx]->getDepth();
    int idx2 = pair_idx(idx, depth_bkt);
    
    int step = 1 << depth_bkt;
    int rg_value = 1 << depth;
    if (buckets[idx2]->getDepth() == depth_bkt){
        delete(buckets[idx]);
        buckets[idx2]->decrDepth();
        Bucket<T> *temp = buckets[idx2];
        for (int i = idx; i < rg_value; i+=step)
            buckets[i] = temp;
        for (int i = idx - step; i>= 0; i-=step)
            buckets[i] = temp;
        if (depth > 1 ) /* it's better to use more concrete condition 
        scheme to descrease unneccesarry halve_dir call which has O(N) time complexity */
            halve_dir();
    }
}
template<class T>
int Directory<T>::halve_dir(){
    int rg_value = 1<<depth;
    for (int i = 0; i < rg_value; ++i){
        if (buckets[i]->getDepth() == depth)
            return 1;  // local depth = global depth,  we cannot halve directory
    }
    depth--;
    size = 1<< depth;
    for (int i= 0; i < 1<<depth; ++i)
        buckets.pop_back();
    return 0;
}
template<class T>
Directory<T>::~Directory(){
    int rg_value = 1<< depth;
    /* protect us from double or more free memory using set,
       it's better to use the smart pointer */
    unordered_set<Bucket<T>*> ptr_set; 
    for (int i = 0; i <rg_value; i++){
        ptr_set.insert(buckets[i]);
    }
    for (auto it = ptr_set.begin(); it != ptr_set.end(); ++it){
        delete(*it);
    }
    buckets.clear(); // empty the buckets
}
template<class T>
void Directory<T>::display(){
    unordered_set<Bucket<T>*> display_set;
    cout << "global depth: " << depth << " size:  "  << size << endl;
    for (auto ix: buckets)
    {
        if (display_set.find(ix) == display_set.end()){
            ix->display();
            display_set.emplace(ix);
            cout << endl;
        }
    }
}