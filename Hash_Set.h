#include <iostream>
#include <string> 
#include <sstream>

using namespace std;
const double REHASH_COEFF = 1.5;
const double RESIZE_COEFF = 0.75;

template <typename T>
string toString(T val) {
    ostringstream strs;
    strs << val;
    return strs.str();
}

template <typename T>
int HashFunction(const T& elem, int table_size, const int key) {
    int hash_result = 0;

    string s = toString(elem);

    for (int i = 0; i < s.size(); i++){
        hash_result += (key * hash_result + s[i]) % table_size;
    }

    hash_result = (hash_result * 2 + 1) % table_size;

    return hash_result;

}

template <typename T>
struct HashFunc1{
    int operator()(const T s, size_t table_size) const {
        return HashFunction(s, table_size, table_size + 1);
    }
}; 

template <typename T>
struct HashFunc2{
    int operator()(const T s, size_t table_size) const {
        return HashFunction(s, table_size, table_size + 1);
    }
};

template <typename T, typename Hash1 = HashFunc1<T>, typename Hash2 = HashFunc2<T>>
class HashSet
{
private:
    
    struct Node{
        T _value;
        bool _state;
        Node(const T& value) : _value(value), _state(true) {}
    };
    
    Node **_array;
    size_t _size;
    size_t _capasity;
    size_t _general_size;


    inline void Resize(){
        int old_capasity = _capasity;
        _capasity *= 2;
        _size = 0;
        _general_size = 0;
        Node** new_arr = new Node*[_capasity];
        
        for (int i = 0; i < _capasity; i++)
            new_arr[i] = nullptr;
        
        swap(_array, new_arr);
        
        for (int i = 0; i < old_capasity; i++){
            if (new_arr[i] && new_arr[i]->_state){
                Add(new_arr[i]->_value);
            }
        }

        for (int i = 0; i < old_capasity; ++i)
            if (new_arr[i])
                delete new_arr[i];
        delete [] new_arr;

    }



public:
    
    HashSet(size_t capasity):   _capasity(capasity), _size(0),
                                _general_size(0)
    {   
        _array = new Node*[_capasity];
        for (int i = 0; i < _capasity; ++i)
            _array[i] = nullptr; 
    }

    ~HashSet(){
        for (int i = 0; i < _capasity; i++){
            delete _array[i];
            _array[i] = nullptr;
        }
        delete [] _array;
        _array = nullptr;
    }
    
    inline void Rehash(){
        _size = 0;
        _general_size = 0;
        Node** new_arr = new Node*[_capasity];
        
        for (int i = 0; i < _capasity; i++)
            new_arr[i] = nullptr;
        
        swap(_array, new_arr);
        
        for (int i = 0; i < _capasity; i++){
            if (new_arr[i] && new_arr[i]->_state){
                Add(new_arr[i]->_value);
            }
        }

        for (int i = 0; i < _capasity; ++i)
            if (new_arr[i])
                delete new_arr[i];
        delete [] new_arr;
    }
    
    bool Find(const T& elem, const Hash1 hash1 = Hash1(), const Hash2 hash2 = Hash2()) const {
        int hash = hash1(elem, _capasity);
        int step_hash = hash2(elem, _capasity);

        for (int i = 0; _array[hash] != nullptr && i < _capasity; i++){
            if (_array[hash]->_state && _array[hash]->_value == elem) return true;
            hash = (hash + step_hash) % _capasity;
        } 

        return false;
    }

    size_t GetPos(const T& elem, const Hash1 hash1 = Hash1(), const Hash2 hash2 = Hash2()) const {
        int hash = hash1(elem, _capasity);
        int step_hash = hash2(elem, _capasity);

        for (int i = 0; _array[hash] != nullptr && i < _capasity; i++){
            if (_array[hash]->_state && _array[hash]->_value == elem) return hash;
            hash = (hash + step_hash) % _capasity;
        } 
        
        throw "Element doesn`t exist";

    }


    void Remove(const T& elem, const Hash1 hash1 = Hash1(), const Hash2 hash2 = Hash2()) {
        int hash = hash1(elem, _capasity);
        int step_hash = hash2(elem, _capasity);
        for (int i = 0; _array[hash] != nullptr && i < _capasity; i++){
            if (_array[hash]->_state && _array[hash]->_value == elem) {
                _array[hash]->_state = false;
                _size--;
                return;
            }
            hash = (hash + step_hash) % _capasity;
        }   
        return;
    }

    void Add(const T& elem, const Hash1 hash1 = Hash1(), const Hash2 hash2 = Hash2()) {
        if (_capasity + 1 > (int)(RESIZE_COEFF * _size)) Resize();
        else if (_general_size > int(REHASH_COEFF * _size)) Rehash();
        int hash = hash1(elem, _capasity);
        int step_hash = hash2(elem, _capasity);
        int first_del = -1;
        for (int i = 0; _array[hash] != nullptr && i < _capasity; i++){
            if (_array[hash]->_state && _array[hash]->_value == elem) return;

            if (!(_array[hash]->_state) && _array[hash]->_value == elem) first_del = hash;

            hash = (hash + step_hash) % _capasity;
        }

        if (first_del != -1){
            _array[first_del]->_value = elem;
            _array[first_del]->_state = true;
        }
        
        else {
            _array[hash] = new Node(elem);
            _general_size++;
        }    

        _size++;
        return;
    }

};
