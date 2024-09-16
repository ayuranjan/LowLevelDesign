#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>

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

// Example usage
int main() {
    // Type 0: Size getter
    // Type 0: Capacity getter
    // Type 1: Default constructor
    Vector<int> v0; 
    std::cout << "v0 (default constructor) - size: " << v0.size() << ", capacity: " << v0.capacity() << std::endl;

    // Type 2: Constructor with initial size
    Vector<int> vsz(5);
    std::cout << "v1 (initialized with size) - size: " << vsz.size() << ", capacity: " << vsz.capacity() << std::endl;

    // Type Extra: Constructor with initializer list
    Vector<int> v = {1, 6, 4, 7}; 
    std::cout << "v (initialized with list) - size: " << v.size() << ", capacity: " << v.capacity() << std::endl;

    // Type 3: Push back elements
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
    }
    std::cout << "v after push_back - size: " << v.size() << ", capacity: " << v.capacity() << std::endl;

    // Type 4: Access elements
    std::cout << "First element: " << v[0] << ", Last element: " << v[v.size() - 1] << std::endl;

    // Type 5: Pop back an element
    std::cout << "Vector size before pop_back: " << v.size() << std::endl;
    v.pop_back();
    std::cout << "Vector size after pop_back: " << v.size() << std::endl;

    // Type 6: Iterate over elements
    std::cout << "All elements: ";
    for (const auto& elem : v) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    // Type 7: Copy constructor
    Vector<int> v2 = v;
    std::cout << "v2 (copy constructed) - size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;

    // Type 7 Extra: Copy assignment operator
    Vector<int> v4;
    v4 = v;
    // Type 8: Move constructor
    Vector<int> v3 = std::move(v);
    std::cout << "v3 (move constructed) - size: " << v3.size() << ", Original vector size: " << v.size() << std::endl;

    // Type 8 Extra: Move assignment operator
    Vector<int> v5;
    v5 = std::move(v2);
    std::cout << "v5 (move assigned) - size: " << v5.size() << ", Original vector size: " << v2.size() << std::endl;
    
    // Type 9: Resize
    v2.resize(5);
    std::cout << "v2 after resize - size: " << v2.size() << ", capacity: " << v2.capacity() << std::endl;

    return 0;
}
