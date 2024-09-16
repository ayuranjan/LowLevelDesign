#include <iostream>
#include <stdexcept>
#include <initializer_list>
#include <algorithm>

template <typename T>
class List {
private:
    T* data;          // Pointer to dynamically allocated array of elements
    size_t size_;     // Number of elements currently in the list
    size_t capacity_; // Current capacity of the list

    // Helper function to reallocate memory and copy elements
    void realloc(size_t newCapacity) {
        T* newData = nullptr;
        size_t newSize = 0;
        try {
            newData = (newCapacity > 0) ? new T[newCapacity] : nullptr;
            newSize = std::min(size_, newCapacity); // to shrink the list if the new size is smaller
            for (size_t i = 0; i < newSize; ++i) {
                newData[i] = std::move(data[i]); // to reduce the copy overhead  
            }
        } catch (std::exception& e) {
            delete[] newData; // Clean up if allocation fails
            std::cout << "Exception issue: " << e.what() << std::endl;
            return;
        }

        delete[] data; // Free old memory
        data = newData;
        capacity_ = newCapacity;
        size_ = newSize;
    }

public:

    // 0. Capacity and size getters
    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }
    bool empty() const { return size_ == 0; }



    // 1. Default constructor: initializes an empty list
    List() : data(nullptr), size_(0), capacity_(0) {}

    // 2. Constructor with initial size: allocates memory for the given number of elements
     /*
            -> memory allocation might throw an exception if it fails
                ->  std::nothrow : return a null pointer if the allocation fails
                ->  noexcept : the function will not throw an exception
                -> try and catch ( auto& e ) : e.what() to get the error message
        */ 
    explicit List(size_t size) : data(nullptr), size_(size), capacity_(size) {
        if (size > 0) {
            data = new T[size](); // Initialize elements to default value
        }
    }

    // 3. Constructor with initializer list: initializes list with list of elements
    List(std::initializer_list<T> init) : size_(init.size()), capacity_(init.size()) {
        data = new T[capacity_];
        std::copy(init.begin(), init.end(), data);
    }

    // 4. Add an element to the end, resizing if necessary
    void push_back(const T& value) {
        if (size_ == capacity_) {
            realloc(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[size_++] = value;
    }

     // 5. Access elements (const): provides read-only access to elements
    const T& operator[](const size_t& index) const {
        return data[index];
    }

    // 5 Extra : Iterator support: allows range-based for loops and other STL algorithms 
    T* begin() { return data; } // for -> for (auto& x : v)
    const T* begin() const { return data; } // for ->  for (const auto& x : v)
    T* end() { return data + size_; }
    const T* end() const { return data + size_; }

     // 6. Remove the last element, potentially shrinking the list
    void pop_back() {
        if (size_ > 0) {
            --size_; // for indexing since last element is at size_ - 1
            data[size_].~T(); // Explicitly call the destructor of the last element
        }
        // Extra credit: Reduce capacity if size is less than 1/4 of capacity
        if (size_ < capacity_ / 4 && capacity_ > 1) {
            realloc(capacity_ / 2); // Reduce capacity to half
        }
    }


    // 7. Resize the list to the new size
    void resize(size_t newSize) {
        if (newSize > capacity_) {
            realloc(newSize);
        }
        size_ = newSize;
    }

      // 8 : Copy constructor
    /*
    -> The & makes other a reference. 
        -> avoid making an unnecessary copy of the entire string 
    -> The const before String& means that the function cannot change the object that it is called on.
        -> It allows us to pass both const and non-const objects to the constructor.
    */
    List(const List& other) : data(nullptr), size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            std::copy(other.begin(), other.end(), data);
        }
    }

     // 8 Extra. Copy assignment operator: assigns the contents of another list to this list (Rule of 5)
    List& operator=(const List& other) {
        if (this != &other) {
            delete[] data; // Free existing resources
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];
            std::copy(other.data, other.data + size_, data);
        }
        return *this;
    }


    // 9. Move constructor: transfers ownership of resources from another list (Rule of 5)
    // -> && : rvalue reference
    List(List&& other)  : data(other.data), size_(other.size_), capacity_(other.capacity_) {
        other.data = nullptr;
        other.size_ = other.capacity_ = 0;
    }

  

    // 9 Extra. Move assignment operator: transfers ownership of resources from another list (Rule of 5)
    List& operator=(List&& other)  {
        if (this != &other) {
            delete[] data; // Release current resources
            data = other.data; // Transfer ownership
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data = nullptr; // Nullify the other list's resources
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    // Destructor: deallocates memory to prevent leaks (Rule of 5)
    ~List() {
        delete[] data;
    }

   

    // Extra Clear all elements without deallocating memory
    void clear() {
        size_ = 0;
    }


};

// Main method to showcase examples of the List class usage
int main() {
    // 1. Example of default constructor
    List<int> l1; 
    std::cout << "1. Default constructor - size: " << l1.size() << ", capacity: " << l1.capacity() << std::endl;

    // 2. Example of constructor with initial size
    List<int> l2(5);
    std::cout << "2. Constructor with initial size - size: " << l2.size() << ", capacity: " << l2.capacity() << std::endl;

    // 3. Example of constructor with initializer list
    List<int> l3 = {2, 4, 6, 8} ; // or List<int> l3{2, 4, 6, 8} or List<int> l3({2, 4, 6, 8});
    std::cout << "3. Constructor with initializer list - size: " << l3.size() << ", capacity: " << l3.capacity() << std::endl;

    // 4. Example of push_back method
    l3.push_back(10);
    std::cout << "4. After push_back(10) - size: " << l3.size() << ", last element: " << l3[l3.size() - 1] << std::endl;

    // 5. Example of element access using operator[]
    std::cout << "5. Accessing element at index 2: " << l3[2] << std::endl;

    // 5 Extra. Example of iterator support
    std::cout << "5 Extra. Using iterator support: ";
    for (const auto& x : l3) {
        std::cout << x << " ";
    }

    // 6. Example of pop_back method
    l3.pop_back();
    std::cout << "6. After pop_back - size: " << l3.size() << std::endl;

    // 7. Example of resize method
    l3.resize(10);
    std::cout << "7. After resize(10) - size: " << l3.size() << ", capacity: " << l3.capacity() << std::endl;

    // 8. Example of copy constructor
    List<int> l4 = l3;
    std::cout << "8. Copy constructor - size of copied list: " << l4.size() << std::endl;

    // 8 Extra. Example of copy assignment operator
    List<int> l5;
    l5 = l4;
    std::cout << "8 Extra. Copy assignment operator - size of assigned list: " << l5.size() << std::endl;

    // 9. Example of move constructor
    List<int> l6 = std::move(l4);
    std::cout << "9. Move constructor - size of moved list: " << l6.size() << ", size of original list: " << l4.size() << std::endl;

    // 9 Extra. Example of move assignment operator
    List<int> l7;
    l7 = std::move(l5);

    return 0;
}
