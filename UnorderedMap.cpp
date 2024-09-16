#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>
using namespace std;
template <typename Key, typename Value>
class UnorderedMap {
private:
    struct KeyValuePair {
        Key key;
        Value value;
        KeyValuePair(const Key& k, const Value& v) : key(k), value(v) {}
    };

    std::vector<std::list<KeyValuePair>>* buckets;
    size_t size_;
    size_t bucket_count_;
    float max_load_factor_;

    // Hash function
    size_t hash(const Key& key) const {
        return std::hash<Key>{}(key) % bucket_count_;
    }

    // this returns an iterator to the key value pair in the bucket
    auto find_in_bucket(const Key& key, size_t bucket_index) {
        auto& bucket = (*buckets)[bucket_index];
        return std::find_if(bucket.begin(), bucket.end(),
                            [&key](const KeyValuePair& pair) { return pair.key == key; });  // find_if(begin iterator, end iterator, predicate lambda function)

                            /*
                            Lambda function syntax  : [capture list] (parameters) -> return_type { function body }
                            here capture list is key, parameters is pair, return type is bool and function body is pair.key == key 
                            */
    }

    // Resize the hash table
    void rehash(size_t new_bucket_count) {
        auto new_buckets = new std::vector<std::list<KeyValuePair>>(new_bucket_count);
        for (const auto& bucket : *buckets) {
            for (const auto& pair : bucket) {
                size_t new_index = std::hash<Key>{}(pair.key) % new_bucket_count; // hash function to determine which bucket a key-value pair goes into
                (*new_buckets)[new_index].push_back(pair); // push_back is a function of list to add an element to the end of the list
            }
        }
        delete buckets;
        buckets = new_buckets; // move is a function of vector to move the contents of one vector to another
        bucket_count_ = new_bucket_count; // update the bucket count
    }

public:
    // 1. Default constructor
    UnorderedMap() : size_(0), bucket_count_(10), max_load_factor_(1.0) { // default bucket count is 10 and max load factor is 1.0 
        buckets = new std::vector<std::list<KeyValuePair>>(bucket_count_); // resize is a function of vector to resize the vector to the specified size
    }

    // 2. Constructor with initial bucket count
    explicit UnorderedMap(size_t bucket_count) : size_(0), bucket_count_(bucket_count), max_load_factor_(1.0) {
        buckets = new std::vector<std::list<KeyValuePair>>(bucket_count_);
    }

    // 3. Insert or update a key-value pair : java equivalent of put
    void insert_or_assign(const Key& key, const Value& value) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        if (it == (*buckets)[index].end()) { // if the key is not found in the bucket
            (*buckets)[index].emplace_back(key, value); // emplace_back is a function of list to add an element to the end of the list
            ++size_;
            if (size_ > bucket_count_ * max_load_factor_) {
                rehash(bucket_count_ * 2);
            }
        } else { // if the key is found in the bucket then update the value
            it->value = value; // why -> ? because it is a pointer to the key value pair
        }
    }
    // 3. Insert a key-value pair 
    bool insert(const Key& key, const Value& value) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        if (it == (*buckets)[index].end()) {
            (*buckets)[index].emplace_back(key, value);
            ++size_;
            if (size_ > bucket_count_ * max_load_factor_) {
                rehash(bucket_count_ * 2);
            }
            return true;  // Insertion successful
        }
        return false;  // Key already exists, insertion failed
    }

    // 4. Remove a key-value pair
    bool erase(const Key& key) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        if (it != (*buckets)[index].end()) { // if the key is found in the bucket
            (*buckets)[index].erase(it); // erase is a function of list to remove an element from the list
            --size_;
            return true;
        }
        return false;
    }

    // 5. Check if a key exists
    bool contains(const Key& key) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        return it != (*buckets)[index].end(); // if the key is found in the bucket
    }

    // 6. Get the value associated with a key also used to update the value for existing keys and insert a new key-value pair if the key doesn't exist 
     Value& operator[](const Key& key) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        if (it != (*buckets)[index].end()) {
            return it->value;
        }
        // If key doesn't exist, insert a new element with a default-constructed value
        auto& list = (*buckets)[index];
        list.emplace_back(key, Value()); // emplace_back is a function of list to add an element to the end of the list
        ++size_;
        if (size_ > bucket_count_ * max_load_factor_) {
            rehash(bucket_count_ * 2);
        }
        return list.back().value; // back - Returns a reference to the last element in the list and value is the value
    }

    // 7. Get the number of elements
    size_t size() const {
        return size_;
    }

    // 8. Check if the map is empty
    bool empty() const {
        return size_ == 0;
    }

    // 9. Clear the map
    void clear() {
        for (auto& bucket : *buckets) {
            bucket.clear();
        }
        size_ = 0;
    }

    // 10. Get the number of buckets
    size_t bucket_count() const {
        return bucket_count_;
    }

    // 11. Get the current load factor
    float load_factor() const {
        return static_cast<float>(size_) / bucket_count_;
    }

    // 12. Set the maximum load factor
    void max_load_factor(float mlf) {
        max_load_factor_ = mlf;
        if (load_factor() > max_load_factor_) {
            rehash(size_ / max_load_factor_ + 1);
        }
    }

    // 13. Copy constructor
    UnorderedMap(const UnorderedMap& other) : size_(other.size_), bucket_count_(other.bucket_count_), max_load_factor_(other.max_load_factor_) {
        buckets = new std::vector<std::list<KeyValuePair>>(*other.buckets);
    }

    // 14. Copy assignment operator
    UnorderedMap& operator=(const UnorderedMap& other) {
        if (this != &other) {
            delete buckets;
            size_ = other.size_;
            bucket_count_ = other.bucket_count_;
            max_load_factor_ = other.max_load_factor_;
            /*
                - std::vector copy constructor will be called when we create a new vector from the other vector.
                - This will, in turn, call the copy constructor of each std::list it contains.
                - Each std::list copy constructor will then copy all the KeyValuePair objects it contains.
                - The Key and Value objects within each KeyValuePair will have their copy constructors called as well.

            */
            buckets = new std::vector<std::list<KeyValuePair>>(*other.buckets); // This dereferences the buckets pointer of the other

        }
        return *this;
    }

    // Move constructor
    UnorderedMap(UnorderedMap&& other) noexcept : size_(other.size_), bucket_count_(other.bucket_count_), max_load_factor_(other.max_load_factor_) {
        buckets = other.buckets;
        other.buckets = nullptr;
        other.size_ = 0;
        other.bucket_count_ = 0;
    }

    // Move assignment operator
    UnorderedMap& operator=(UnorderedMap&& other) noexcept {
        if (this != &other) {
            delete buckets;
            size_ = other.size_;
            bucket_count_ = other.bucket_count_;
            max_load_factor_ = other.max_load_factor_;
            buckets = other.buckets;
            other.buckets = nullptr;
            other.size_ = 0;
            other.bucket_count_ = 0;
        }
        return *this;
    }

    // Destructor
    ~UnorderedMap() {
        /*
            - std::vector destructor will be automatically called when we delete buckets. 
            - This vector destructor will, in turn, call the destructor of each std::list it contains.
            - Each std::list destructor will then delete all the KeyValuePair objects it contains.
            - The Key and Value objects within each KeyValuePair will have their destructors called as well.
        */
        delete buckets;
    }
};


int main() {
    /* Logic for Unordered Map implementation:
    UnorderedMap
   |
   +-- Vector of Buckets
       |
       +-- Bucket 0: [K1,V1] -> [K2,V2] -> nullptr
       |
       +-- Bucket 1: [K3,V3] -> nullptr
       |
   - Each Bucket is a linked list of KeyValuePairs
   - The hash function determines which bucket a key-value pair goes into
   - Collisions are handled by adding to the same bucket (linked list)
   - Keep track of number of elements and buckets (size_ and bucket_count_)
   - Use load factor to determine when to resize the hash table
   - Maintain max_load_factor to trigger resizing when exceeded
   - Allow updating of values for existing keys (insert_or_assign method)
    */

    UnorderedMap<string, int> map1; // default ctor
    UnorderedMap<string, int> map2(20); // ctor with initial bucket count
    map2.insert_or_assign("one", 1); // insert_or_assign
    map2.erase("two"); // erase
    cout << map2.contains("one"); // contains
    cout << map2["one"]; // operator[]
    map2["two"] = 22; // operator[] for insertion
    cout << map2.size(); // size
    cout << map2.empty(); // empty
    map2.clear(); // clear
    cout << map2.bucket_count(); // bucket_count
    cout << map2.load_factor(); // load_factor
    map2.max_load_factor(0.5); // max_load_factor
    UnorderedMap<string, int> map3 = map2; // copy ctor
    UnorderedMap<string, int> map4;
    map4 = map2; // copy assignment
    /*
        -- move :  It just casts ptr2 to an rvalue reference
        -- so it doesn't create a copy or anything. 
        --or else if we passed pointer to ptr3 then it would have created a copy
        -- if we made that pointer const then it wont create a copy but we could not modify the value of the pointer

    */
    UnorderedMap<string, int> map5 = std::move(map3); // move ctor
    UnorderedMap<string, int> map6;
    map6 = std::move(map5); // move assignment

    return 0;
}