#include <iostream>
#include <stdexcept>
#include <algorithm>

// 1. Queue class definition following Rule of 5 and STL naming conventions
template <typename T>
class Queue {
private:
    T* data;          // Pointer to dynamically allocated array for queue elements
    size_t size_;     // Number of elements in the queue
    size_t capacity_; // Capacity of the queue
    size_t front_;    // Index of the front element
    size_t rear_;     // Index of the rear element

    // Helper function to reallocate memory for the queue
    void realloc(size_t newCapacity) {
        T* newData = nullptr;
        size_t newSize = 0;
        try {
            newData = (newCapacity > 0) ? new T[newCapacity] : nullptr;
            newSize = std::min(size_, newCapacity); // Adjust size if capacity shrinks
            for (size_t i = 0; i < newSize; ++i) {
                newData[i] = std::move(data[(front_ + i) % capacity_]); // Move elements in correct order
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
    // 0. Size  getter
    size_t size() const { return size_; }
    // 1. Default constructor: initializes an empty queue
    Queue() : data(nullptr), size_(0), capacity_(0), front_(0), rear_(0) {}

    // 2. Constructor with initial capacity: allocates memory for the given number of elements
    explicit Queue(size_t size) : data(nullptr), size_(0), capacity_(size), front_(0), rear_(size - 1) {
        if (size > 0) {
            data = new T[size]();
        }
    }

    // 3. Enqueue (push) an element to the queue
    void push(const T& value) {
        if (size_ == capacity_) {
            realloc(capacity_ == 0 ? 1 : capacity_ * 2); // Double capacity when full
        }
        rear_ = (rear_ + 1) % capacity_;
        data[rear_] = value;
        ++size_;
    }

    // 4. Dequeue (pop) an element from the front of the queue
    void pop() {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
        front_ = (front_ + 1) % capacity_;
        --size_;
    }

    // 5: Check if the queue is empty 
    bool empty() const { return size_ == 0; }

    // 6. Copy constructor: creates a deep copy of another queue
    Queue(const Queue& other) : data(nullptr), size_(other.size_), capacity_(other.capacity_), front_(other.front_), rear_(other.rear_) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];// since both will be of same size , or else we can do (front_ + i) % capacity_
            }
        }
    }

    // 7. Copy assignment operator: assigns the contents of another queue to this queue
    Queue& operator=(const Queue& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            front_ = other.front_;
            rear_ = other.rear_;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }


    // 8. Move constructor: transfers ownership of resources from another queue
    Queue(Queue&& other)  : data(other.data), size_(other.size_), capacity_(other.capacity_), front_(other.front_), rear_(other.rear_) {
        other.data = nullptr;
        other.size_ = other.capacity_ = 0;
        other.front_ = other.rear_ = 0;
    }

 

    // 9. Move assignment operator: transfers ownership of resources from another queue
    Queue& operator=(Queue&& other)  {
        if (this != &other) {
            delete[] data; // Release current resources before moving
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


  

    // 9. Peek the front element 
    const T& front() const {
        if (size_ == 0) {
            throw std::out_of_range("Queue is empty");
        }
        return data[front_];
    }

    

    // Destructor: deallocates memory to prevent leaks
    ~Queue() {
        delete[] data;
    }

    // Extra: Clear all elements without deallocating memory
    void clear() {
        size_ = 0;
        front_ = 0;
        rear_ = capacity_ - 1;
    }

    
};

// Main method to showcase examples of the Queue class usage
int main() {

    /* Logic :  
        -> array of size capacity_ 
        -> while adding/removing we will do (( front/back + 1 )% capacity)  to create kind of circular array scenario 
            -> when  size_ == capacity_, reallocate more memory.
        -> front_  = 0  and rear_ = ( capacity_ -1 )
                -> to avoid the case of front_ = rear_ = 0 
                ->  else it will be difficult to differentiate between empty and full queue 
        -> when we pop, we will increment front_ by 1 only. No need to delete the element.
        -> when we push, we will increment rear_ by 1 and add the element at rear_ index.
    */

    // 0. size getter
    // 1. Example of default constructor
    Queue<int> q1;
    std::cout << "1. Default constructor - size: " << q1.size() << std::endl;

    // 2. Example of constructor with initial size
    Queue<int> q2(5);
    std::cout << "2. Constructor with initial size - size: " << q2.size() << std::endl;

    // 3. Example of push (enqueue)
    q2.push(10);
    std::cout << "7. After push(30) - size: " << q2.size() << std::endl;

    // 4. Example of pop (dequeue)
    q2.pop();
    std::cout << "8. After pop - size: " << q2.size() << std::endl;

    // 5. empty check 
    std::cout << "9. Is queue empty: " << q2.empty() << std::endl;

    // 6. Example of copy constructor
    q2.push(10);
    q2.push(20);
    Queue<int> q3 = q2;
    std::cout << "3. Copy constructor - size of copied queue: " << q3.size() << std::endl;


    // 7 . Example of copy assignment operator
    Queue<int> q4;
    q4 = q2;
    std::cout << "5. Copy assignment operator - size of assigned queue: " << q4.size() << std::endl;

    // 8. Example of move constructor
    Queue<int> q5 = std::move(q3);
    std::cout << "4. Move constructor - size of moved queue: " << q5.size() << ", size of original queue: " << q5.size() << std::endl;


    // 9 Example of move assignment operator
    Queue<int> q6;
    q6 = std::move(q5);
    std::cout << "6. Move assignment operator - size of moved queue: " << q6.size() << std::endl;

   

    // 10. Example of front
    std::cout << "9. Front element: " << q6.front() << std::endl;

    return 0;
}
