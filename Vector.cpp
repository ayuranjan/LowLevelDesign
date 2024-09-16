#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
using namespace std;
template <typename T>
class Vector {
private:
    T* data;          // Pointer to dynamically allocated array of elements
    size_t size_;     // Number of elements currently in the vector
    size_t capacity_; // Current capacity of the vector

    // Helper function to reallocate memory and copy elements
    void realloc(size_t newCapacity) {
        T* newData = nullptr;
        size_t newSize = 0;
        try {
            newData = (newCapacity > 0) ? new T[newCapacity] : nullptr;
            newSize = std::min(size_, newCapacity); // to shrink the vector if the new size is smaller
            for (size_t i = 0; i < newSize; ++i) {
                newData[i] = std::move(data[i]); // to reduce the copy overhead  
            }
        } catch (std::exception& e) {
            delete[] newData; // Clean up if allocation fails
            std::cout << "Exception issue: " << e.what() << std::endl;
            return; // Return without modifying the vector
        }

        delete[] data; // Free old memory
        data = newData;
        capacity_ = newCapacity;
        size_ = newSize;
    }

public:
    // Type 0: Capacity and size getters
    size_t size() const { return size_; }     // Returns the number of elements
    size_t capacity() const { return capacity_; } // Returns the current capacity
    bool empty() const { return size_ == 0; } // Checks if the vector is empty

    // Type 1: Default constructor: initializes an empty vector
    Vector() : data(nullptr), size_(0), capacity_(0) {}

    // Type 2: Constructor with initial size: allocates memory for the given number of elements
    /*
        when size is 0 : data is null
    */
    explicit Vector(size_t size) : data(nullptr), size_(size), capacity_(size) {
        if (size > 0) {
            data = new T[size](); // Initialize elements to default value
        }
    }

    // Type Extra: Constructor with initializer list: initializes vector with list of elements
    Vector(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()) {
        data = new T[capacity_];
        std::copy(init.begin(), init.end(), data);
    }

    // Type 3: Add an element to the end, resizing if necessary
    void push_back(const T& value) {
        if (size_ == capacity_) {
            realloc(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[size_++] = value;
    }

    // Type 4: Const element access: provides read-only access to elements
    const T& operator[](const size_t& index) const {
        return data[index];
    }

    // Type 5: Remove the last element, potentially shrinking the vector
    void pop_back() {
        if (size_ > 0) {
            --size_; // for indexing since last element is at size_ - 1
            data[size_].~T(); // Explicitly call the destructor of the last element
        }
        // Optionally shrink the capacity
        if (size_ < capacity_ / 4 && capacity_ > 1) {
            realloc(capacity_ / 2); // Reduce capacity to half
        }
    }

    // Type 6: Iterator support: allows range-based for loops and other STL algorithms 
    T* begin() { return data; } // for -> for (auto& x : v)
    const T* begin() const { return data; } // for ->  for (const auto& x : v)
    T* end() { return data + size_; }
    const T* end() const { return data + size_; }

    // Type 7: Copy constructor: creates a new vector as a copy of another vector
    Vector(const Vector& other) : data(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            std::copy(other.begin(), other.end(), data);
        }
    }

    // Type 7 Extra: Copy assignment operator: assigns the contents of another vector to this vector
    Vector& operator=(const Vector& other) {
        if (this != &other) { // Avoid self-assignment
            delete[] data; // Free existing resources
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];
            std::copy(other.begin(), other.end(), data);
        }
        return *this;
    }

    // Type 8: Move constructor: transfers ownership of resources from another vector
    /*
        -> && : rvalue reference
    */
    Vector(Vector&& other) 
        : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    // Type 8 Extra: Move assignment operator: transfers ownership of resources from another vector
    Vector& operator=(Vector&& other) {
        if (this != &other) { // Avoid self-assignment
            delete[] data; // Release current resources
            data = other.data; // Transfer ownership
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data = nullptr; // Nullify the other vector's resources
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    // Type 9: Resize the vector to the new size
    void resize(size_t newSize) {
        if (newSize > capacity_) {
            realloc(newSize);
        }
        size_ = newSize;
    }

    // Destructor: deallocates memory to prevent leaks
    ~Vector() {
        delete[] data;
    }

    // Type 15: Clear all elements without deallocating memory
    void clear() {
        size_ = 0;
    }
};



int main() {
    /* Logic for Vector implementation:
       -> Uses a dynamically allocated array to store elements
       -> Maintains size (number of elements) and capacity (total allocated space)
       -> Automatically resizes when capacity is reached (typically doubles capacity)
       -> Provides random access through operator[]
       -> Supports operations like push_back, pop_back, and resize
       -> Implements copy and move semantics
       -> Allows iteration through range-based for loops
    */
    // Type 0: Size getter
    // Type 0: Capacity getter
    Vector<int> v0; // default ctor
    Vector<int> vsz(5); // ctor with initial size
    Vector<int> v = {1, 6, 4, 7}; // ctor with initializer list
    v.push_back(10); // push_back
    cout << v[0]; // operator[]
    v.pop_back(); // pop_back
    for (const auto& elem : v) { cout << elem << " "; } // iteration
    Vector<int> v2 = v; // copy ctor
    Vector<int> v4;
    v4 = v; // copy assignment
    /*
        -- move :  It just casts ptr2 to an rvalue reference
        -- so it doesn't create a copy or anything. 
        --or else if we passed pointer to ptr3 then it would have created a copy
        -- if we made that pointer const then it wont create a copy but we could not modify the value of the pointer

    */
    Vector<int> v3 = std::move(v); // move ctor
    Vector<int> v5;
    v5 = std::move(v2); // move assignment
    v2.resize(5); // resize
    cout << v0.size(); // size getter
    cout << v0.capacity(); // capacity getter

    return 0;
}