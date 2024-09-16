#include <iostream>
#include <stdexcept>
#include <algorithm>

// Deque class definition following Rule of 5 and STL naming conventions
template <typename T>
class Deque {
private:
    T* data;          // Pointer to dynamically allocated array for deque elements
    size_t size_;     // Number of elements in the deque
    size_t capacity_; // Capacity of the deque
    size_t front_;    // Index of the front element
    size_t rear_;     // Index of the rear element

    // Helper function to reallocate memory for the deque
    void realloc(size_t newCapacity) {
        T* newData = nullptr;
        size_t newSize = 0;
        try {
            newData = (newCapacity > 0) ? new T[newCapacity] : nullptr;
            newSize = std::min(size_, newCapacity);
            for (size_t i = 0; i < newSize; ++i) {
                newData[i] = std::move(data[(front_ + i) % capacity_]);
            }
        } catch (std::exception& e) {
            delete[] newData;
            std::cout << "Exception issue: " << e.what() << std::endl;
            return;
        }

        delete[] data;
        data = newData;
        capacity_ = newCapacity;
        front_ = 0;
        rear_ = newSize - 1;
        size_ = newSize;
    }

public:
    // 1. Default constructor: initializes an empty deque
    Deque() : data(nullptr), size_(0), capacity_(0), front_(0), rear_(0) {}

    // 2. Constructor with initial capacity: allocates memory for the given number of elements
    explicit Deque(size_t size) : data(nullptr), size_(0), capacity_(size), front_(0), rear_(size - 1) {
        if (size > 0) {
            data = new T[size]();
        }
    }

    // 3. Push an element to the front of the deque
    void push_front(const T& value) {
        if (size_ == capacity_) {
            realloc(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        front_ = (front_ - 1 + capacity_) % capacity_;
        data[front_] = value;
        ++size_;
    }

    // 4. Push an element to the back of the deque
    void push_back(const T& value) {
        if (size_ == capacity_) {
            realloc(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        rear_ = (rear_ + 1) % capacity_;
        data[rear_] = value;
        ++size_;
    }

    // 5. Pop an element from the front of the deque
    void pop_front() {
        if (size_ == 0) {
            throw std::out_of_range("Deque is empty");
        }
        front_ = (front_ + 1) % capacity_;
        --size_;
    }

    // 6. Pop an element from the back of the deque
    void pop_back() {
        if (size_ == 0) {
            throw std::out_of_range("Deque is empty");
        }
        rear_ = (rear_ - 1 + capacity_) % capacity_;
        --size_;
    }

    // 7. Get the front element of the deque
    const T& front() const {
        if (size_ == 0) {
            throw std::out_of_range("Deque is empty");
        }
        return data[front_];
    }

    // 8. Get the back element of the deque
    const T& back() const {
        if (size_ == 0) {
            throw std::out_of_range("Deque is empty");
        }
        return data[rear_];
    }

    // 9. Get the size of the deque
    size_t size() const { return size_; }

    // 10. Check if the deque is empty
    bool empty() const { return size_ == 0; }

    // 11. Copy constructor: creates a deep copy of another deque
    Deque(const Deque& other) : data(nullptr), size_(other.size_), capacity_(other.capacity_), front_(other.front_), rear_(other.rear_) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i]; // since both will be of same size , or else we can do (front_ + i) % capacity_
            }
        }
    }

    // 12. Copy assignment operator: assigns the contents of another deque to this deque
    Deque& operator=(const Deque& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            front_ = other.front_;
            rear_ = other.rear_;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i]; // since both will be of same size , or else we can do (front_ + i) % capacity_
            }
        }
        return *this;
    }

    // Move constructor: transfers ownership of resources from another deque
    Deque(Deque&& other) : data(other.data), size_(other.size_), capacity_(other.capacity_), front_(other.front_), rear_(other.rear_) {
        other.data = nullptr;
        other.size_ = other.capacity_ = 0;
        other.front_ = other.rear_ = 0;
    }

    // Move assignment operator: transfers ownership of resources from another deque
    Deque& operator=(Deque&& other) noexcept {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            front_ = other.front_;
            rear_ = other.rear_;
            other.data = nullptr;
            other.size_ = other.capacity_ = 0;
            other.front_ = other.rear_ = 0;
        }
        return *this;
    }

    // Destructor: deallocates memory to prevent leaks
    ~Deque() {
        delete[] data;
    }

    // Clear all elements without deallocating memory
    void clear() {
        size_ = 0;
        front_ = 0;
        rear_ = capacity_ - 1;
    }
};

// Main method to showcase examples of the Deque class usage
int main() {

    /* Logic for Deque implementation:
    -> Use an array of size capacity_
    -> Circular array scenario:
        - For push_front: decrement front_ using ((front_ - 1 + capacity_) % capacity_)
        - For push_back: increment rear_ using ((rear_ + 1) % capacity_)
        - For pop_front: increment front_ using ((front_ + 1) % capacity_)
        - For pop_back: decrement rear_ using ((rear_ - 1 + capacity_) % capacity_)
    -> When size_ == capacity_, reallocate more memory (double the capacity)
    -> Initialize with front_ = 0 and rear_ = capacity_ - 1
        - This avoids the case of front_ = rear_ = 0
        - Helps differentiate between empty and full deque
    -> When popping, only update front_ or rear_ index. No need to delete the element.
    -> When pushing, update the index (front_ or rear_) and add the element at that index.
    -> Maintain size_ to keep track of the number of elements in the deque
*/
    // 1. Example of default constructor
    Deque<int> d1;
    std::cout << "1. Default constructor - size: " << d1.size() << std::endl;

    // 2. Example of constructor with initial size
    Deque<int> d2(5);
    std::cout << "2. Constructor with initial size - size: " << d2.size() << std::endl;

    // 3. Example of push_front
    d2.push_front(10);
    std::cout << "3. After push_front(10) - size: " << d2.size() << std::endl;

    // 4. Example of push_back
    d2.push_back(20);
    std::cout << "4. After push_back(20) - size: " << d2.size() << std::endl;

    // 5. Example of pop_front
    d2.pop_front();
    std::cout << "5. After pop_front - size: " << d2.size() << std::endl;

    // 6. Example of pop_back
    d2.pop_back();
    std::cout << "6. After pop_back - size: " << d2.size() << std::endl;

    // 7. Example of front
    d2.push_front(30);
    std::cout << "7. Front element: " << d2.front() << std::endl;

    // 8. Example of back
    d2.push_back(40);
    std::cout << "8. Back element: " << d2.back() << std::endl;

    // 9. Example of size
    std::cout << "9. Current size: " << d2.size() << std::endl;

    // 10. Example of empty
    std::cout << "10. Is deque empty: " << (d2.empty() ? "Yes" : "No") << std::endl;

    // 11. Example of copy constructor
    Deque<int> d3 = d2;
    std::cout << "11. Copy constructor - size of copied deque: " << d3.size() << std::endl;

    // 12. Example of copy assignment operator
    Deque<int> d4;
    d4 = d2;
    std::cout << "12. Copy assignment operator - size of assigned deque: " << d4.size() << std::endl;

    return 0;
}