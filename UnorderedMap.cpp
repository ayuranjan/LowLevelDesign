#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>
#include <stdexcept>

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

// Main method to showcase examples of the UnorderedMap class usage
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
   - The hash function to determine which bucket a key-value pair goes into
   - Lets supppose Collisions are handled by adding to the same bucket (linked list) 

    -> We also need to Keep track of the number of elements and the number of buckets so we need to have a size_ and bucket_count_ variable
    -> load factor is used to determine when to resize the hash table so we need to have a load_factor method
                 ->When load factor (elements / buckets) exceeds a threshold, resize the hash table so we need to have a max_load_factor_ variable
                -> we would also need to maintain a max_load_factor to trigger resizing when exceeded
    -> we also need to Allow updating of values for existing keys so we need to have an insert_or_assign method
   
*/
    
    // 1. Example of default constructor
    UnorderedMap<std::string, int> map1;
    std::cout << "1. Default constructor - size: " << map1.size() << ", buckets: " << map1.bucket_count() << std::endl;

    // 2. Example of constructor with initial bucket count
    UnorderedMap<std::string, int> map2(20);
    std::cout << "2. Constructor with initial bucket count - size: " << map2.size() << ", buckets: " << map2.bucket_count() << std::endl;

    // 3. Example of insert_or_assign
    map2.insert_or_assign("one", 1);
    map2.insert_or_assign("two", 2);
    map2.insert_or_assign("three", 3);
    std::cout << "3. After insertions - size: " << map2.size() << std::endl;

    // 4. Example of erase
    map2.erase("two");
    std::cout << "4. After erase('two') - size: " << map2.size() << std::endl;

    // 5. Example of contains
    std::cout << "5. Contains 'one': " << (map2.contains("one") ? "Yes" : "No") << std::endl;
    std::cout << "   Contains 'two': " << (map2.contains("two") ? "Yes" : "No") << std::endl;

    // 6. Example of operator[]
    std::cout << "6. Value for key 'one': " << map2["one"] << std::endl;
    map2["two"] = 22;  // This will insert a new key-value pair
    std::cout << "   Value for key 'two': " << map2["two"] << std::endl;

    // 7. Example of size
    std::cout << "7. Current size: " << map2.size() << std::endl;

    // 8. Example of empty
    std::cout << "8. Is map empty: " << (map2.empty() ? "Yes" : "No") << std::endl;

    // 9. Example of clear
    map2.clear();
    std::cout << "9. After clear - size: " << map2.size() << std::endl;

    // 10. Example of bucket_count
    std::cout << "10. Bucket count: " << map2.bucket_count() << std::endl;

    // 11. Example of load_factor
    map2.insert_or_assign("four", 4);
    map2.insert_or_assign("five", 5);
    std::cout << "11. Current load factor: " << map2.load_factor() << std::endl;

    // 12. Example of max_load_factor
    map2.max_load_factor(0.5);
    std::cout << "12. After setting max_load_factor to 0.5 - bucket count: " << map2.bucket_count() << std::endl;

    // 13. Example of copy constructor
    UnorderedMap<std::string, int> map3 = map2;
    std::cout << "13. Copy constructor - size of copied map: " << map3.size() << std::endl;

    // 14. Example of copy assignment operator
    UnorderedMap<std::string, int> map4;
    map4 = map2;
    std::cout << "14. Copy assignment operator - size of assigned map: " << map4.size() << std::endl;

    // 15. Example of move constructor
    UnorderedMap<std::string, int> map5 = std::move(map3);
    std::cout << "15. Move constructor - size of moved map: " << map5.size() << std::endl;

    // 16. Example of move assignment operator
    UnorderedMap<std::string, int> map6;
    map6 = std::move(map5);
    std::cout << "16. Move assignment operator - size of moved map: " << map6.size() << std::endl;

    return 0;
}