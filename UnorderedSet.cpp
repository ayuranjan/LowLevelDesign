#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <functional>

template <typename T>
class UnorderedSet {
private:
    std::vector<std::list<T>>* buckets;
    size_t size_;
    size_t bucket_count_;
    float max_load_factor_;

    // Hash function
    size_t hash(const T& key) const {
        return std::hash<T>{}(key) % bucket_count_;
    }
    // this returns an iterator to the key value pair in the bucket
    auto find_in_bucket(const T& key, size_t bucket_index) {
        auto& bucket = (*buckets)[bucket_index];
        return std::find(bucket.begin(), bucket.end(), key);
    }

    // Resize the hash table
    void rehash(size_t new_bucket_count) {
        auto new_buckets = new std::vector<std::list<T>>(new_bucket_count);
        for (const auto& bucket : *buckets) {
            for (const auto& element : bucket) {
                size_t new_index = std::hash<T>{}(element) % new_bucket_count;
                (*new_buckets)[new_index].push_back(element);
            }
        }
        delete buckets;
        buckets = new_buckets;
        bucket_count_ = new_bucket_count;
    }

public:
    // 1. Default constructor
    UnorderedSet() : size_(0), bucket_count_(10), max_load_factor_(1.0) {
        buckets = new std::vector<std::list<T>>(bucket_count_);
    }

    // 2. Constructor with initial bucket count
    explicit UnorderedSet(size_t bucket_count) : size_(0), bucket_count_(bucket_count), max_load_factor_(1.0) {
        buckets = new std::vector<std::list<T>>(bucket_count_);
    }

    // 3. Insert an element
    bool insert(const T& key) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        if (it == (*buckets)[index].end()) {
            (*buckets)[index].push_back(key);
            ++size_;
            if (size_ > bucket_count_ * max_load_factor_) {
                rehash(bucket_count_ * 2);
            }
            return true;
        }
        return false;
    }

    // 4. Remove an element
    bool erase(const T& key) {
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        if (it != (*buckets)[index].end()) {
            (*buckets)[index].erase(it);
            --size_;
            return true;
        }
        return false;
    }

    // 5. Check if an element exists
    bool contains(const T& key)  { // here we call the const version of find_in_bucket since we are not modifying the set
        size_t index = hash(key);
        auto it = find_in_bucket(key, index);
        return it != (*buckets)[index].end();
    }

    // 6. Get the number of elements
    size_t size() const {
        return size_;
    }

    // 7. Check if the set is empty
    bool empty() const {
        return size_ == 0;
    }

    // 8. Clear the set
    void clear() {
        for (auto& bucket : *buckets) {
            bucket.clear();
        }
        size_ = 0;
    }

    // 9. Get the number of buckets
    size_t bucket_count() const {
        return bucket_count_;
    }

    // 10. Get the current load factor
    float load_factor() const {
        return static_cast<float>(size_) / bucket_count_;
    }

    // 11. Set the maximum load factor
    void max_load_factor(float mlf) {
        max_load_factor_ = mlf;
        if (load_factor() > max_load_factor_) {
            rehash(size_ / max_load_factor_ + 1);
        }
    }

    // 12. Copy constructor
    UnorderedSet(const UnorderedSet& other) : size_(other.size_), bucket_count_(other.bucket_count_), max_load_factor_(other.max_load_factor_) {
        buckets = new std::vector<std::list<T>>(*other.buckets);
    }

    // 13. Copy assignment operator
    UnorderedSet& operator=(const UnorderedSet& other) {
        if (this != &other) {
            delete buckets;
            size_ = other.size_;
            bucket_count_ = other.bucket_count_;
            max_load_factor_ = other.max_load_factor_;
            buckets = new std::vector<std::list<T>>(*other.buckets);
        }
        return *this;
    }

    // Move constructor
    UnorderedSet(UnorderedSet&& other) noexcept : size_(other.size_), bucket_count_(other.bucket_count_), max_load_factor_(other.max_load_factor_) {
        buckets = other.buckets;
        other.buckets = nullptr;
        other.size_ = 0;
        other.bucket_count_ = 0;
    }

    // Move assignment operator
    UnorderedSet& operator=(UnorderedSet&& other) noexcept {
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
    ~UnorderedSet() {
        /*
            - will free the memory allocated for the std::vector itself.
            - The std::vector destructor will then be called, which will free the memory allocated for the std::list objects.
            - Each std::list destructor will then destroy all the T objects it contains.
            - The destructor for each T object will be called.
        */
        delete buckets;
    }

  
};

// Main method to showcase examples of the UnorderedSet class usage
int main() {
    // 1. Example of default constructor
    UnorderedSet<int> set1;
    std::cout << "1. Default constructor - size: " << set1.size() << ", buckets: " << set1.bucket_count() << std::endl;

    // 2. Example of constructor with initial bucket count
    UnorderedSet<int> set2(20);
    std::cout << "2. Constructor with initial bucket count - size: " << set2.size() << ", buckets: " << set2.bucket_count() << std::endl;

    // 3. Example of insert
    set2.insert(10);
    set2.insert(20);
    set2.insert(30);
    std::cout << "3. After insertions - size: " << set2.size() << std::endl;

    // 4. Example of erase
    set2.erase(20);
    std::cout << "4. After erase(20) - size: " << set2.size() << std::endl;

    // 5. Example of contains
    std::cout << "5. Contains 10: " << (set2.contains(10) ? "Yes" : "No") << std::endl;
    std::cout << "   Contains 20: " << (set2.contains(20) ? "Yes" : "No") << std::endl;

    // 6. Example of size
    std::cout << "6. Current size: " << set2.size() << std::endl;

    // 7. Example of empty
    std::cout << "7. Is set empty: " << (set2.empty() ? "Yes" : "No") << std::endl;

    // 8. Example of clear
    set2.clear();
    std::cout << "8. After clear - size: " << set2.size() << std::endl;

    // 9. Example of bucket_count
    std::cout << "9. Bucket count: " << set2.bucket_count() << std::endl;

    // 10. Example of load_factor
    set2.insert(40);
    set2.insert(50);
    std::cout << "10. Current load factor: " << set2.load_factor() << std::endl;

    // 11. Example of max_load_factor
    set2.max_load_factor(0.5);
    std::cout << "11. After setting max_load_factor to 0.5 - bucket count: " << set2.bucket_count() << std::endl;

    // 12. Example of copy constructor
    UnorderedSet<int> set3 = set2;
    std::cout << "12. Copy constructor - size of copied set: " << set3.size() << std::endl;

    // 13. Example of copy assignment operator
    UnorderedSet<int> set4;
    set4 = set2;
    std::cout << "13. Copy assignment operator - size of assigned set: " << set4.size() << std::endl;

    
    return 0;
}