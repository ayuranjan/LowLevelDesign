#include <iostream>
#include <stdexcept>
#include <algorithm>



// Stack class definition following Rule of 5 and STL naming conventions
template <typename T>
class Stack {
private:
    T* data;          // Pointer to dynamically allocated array for stack elements
    size_t size_;     // Number of elements in the stack
    size_t capacity_; // Capacity of the stack
    int top_;         // Index of the top element

    // Helper function to reallocate memory for the stack
    void realloc(size_t newCapacity) {
        T* newData = nullptr;
        try {
            newData = (newCapacity > 0) ? new T[newCapacity] : nullptr;
            std::copy(data, data + size_, newData);
        } catch (std::exception& e) {
            delete[] newData;
            std::cout << "Exception issue: " << e.what() << std::endl;
            return;
        }

        delete[] data;
        data = newData;
        capacity_ = newCapacity;
    }

public:
    // 1. Default constructor: initializes an empty stack
    Stack() : data(nullptr), size_(0), capacity_(0), top_(-1) {}

    // 2. Constructor with initial capacity: allocates memory for the given number of element
    Stack(size_t size) : data(nullptr), size_(0), capacity_(size), top_(-1) {
        if (size > 0) {
            data = new T[size]();
        }
    }

    // 3. Push an element to the top of the stack
    void push(const T& value) {
        if (size_ == capacity_) {
            realloc(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data[++top_] = value; // we increment top first since it starts at -1
        ++size_;
    }

    // 4. Pop an element from the top of the stack
    void pop() {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        --top_;
        --size_;
    }

    // 5. Get the top element of the stack
    const T& top() const {
        if (empty()) {
            throw std::out_of_range("Stack is empty");
        }
        return data[top_];
    }

    // 6. Check if the stack is empty
    bool empty() const { return size_ == 0; }

    // 7. Get the size of the stack
    size_t size() const { return size_; }

    // 8. Copy constructor: creates a deep copy of another stack
    Stack(const Stack& other) : data(nullptr), size_(other.size_), capacity_(other.capacity_), top_(other.top_) {
        if (capacity_ > 0) {
            data = new T[capacity_];
            std::copy(other.data, other.data + size_, data); // we don't need to do manual copy because there is no circular behavior like queue
        }
    }

    // 9. Copy assignment operator: assigns the contents of another stack to this stack
    Stack& operator=(const Stack& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            top_ = other.top_;
            if (capacity_ > 0) {
                data = new T[capacity_];
                std::copy(other.data, other.data + size_, data);
            } else {
                data = nullptr;
            }
        }
        return *this;
    }

    // 10. Move constructor: transfers ownership of resources from another stack
    Stack(Stack&& other) : data(other.data), size_(other.size_), capacity_(other.capacity_), top_(other.top_) {
        other.data = nullptr;
        other.size_ = other.capacity_ = 0;
        other.top_ = -1;
    }

    // 11. Move assignment operator: transfers ownership of resources from another stack
    Stack& operator=(Stack&& other)  {
        if (this != &other) {
            delete[] data;
            data = other.data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            top_ = other.top_;
            // Reset the other stack -- this could be added to a method to avoid code duplication 
            other.data = nullptr;
            other.size_ = other.capacity_ = 0;
            other.top_ = -1;
        }
        return *this;
    }

    // Destructor: deallocates memory to prevent leaks
    ~Stack() {
        delete[] data;
    }

    // Clear all elements without deallocating memory
    void clear() {
        size_ = 0;
        top_ = -1;
    }
};

// Main method to showcase examples of the Stack class usage
int main() {
    /* Logic for Stack implementation:
    -> Use an array of size capacity_
    -> Keep track of the top element using top_ index
    -> When size_ == capacity_, reallocate more memory (double the capacity)
    -> Initialize with top_ = -1 to indicate an empty stack
    -> When pushing, increment top_ and add the element at that index
    -> When popping, decrement top_. No need to delete the element.
    -> Maintain size_ to keep track of the number of elements in the stack
    -> All operations (push, pop, top) are performed at the top_ index
*/
    // 1. Example of default constructor
    Stack<int> s1;
    std::cout << "1. Default constructor - size: " << s1.size() << std::endl;

    // 2. Example of constructor with initial capacity
    Stack<int> s2(5);
    std::cout << "2. Constructor with initial capacity - size: " << s2.size() << std::endl;

    // 3. Example of push
    s2.push(10);
    s2.push(20);
    std::cout << "3. After push(10) and push(20) - size: " << s2.size() << std::endl;

    // 4. Example of pop
    s2.pop();
    std::cout << "4. After pop - size: " << s2.size() << std::endl;

    // 5. Example of top
    std::cout << "5. Top element: " << s2.top() << std::endl;

    // 6. Example of empty
    std::cout << "6. Is stack empty: " << (s2.empty() ? "Yes" : "No") << std::endl;

    // 7. Example of size
    std::cout << "7. Current size: " << s2.size() << std::endl;

    // 8. Example of copy constructor
    Stack<int> s3 = s2;
    std::cout << "8. Copy constructor - size of copied stack: " << s3.size() << std::endl;

    // 9. Example of copy assignment operator
    Stack<int> s4;
    s4 = s2;
    std::cout << "9. Copy assignment operator - size of assigned stack: " << s4.size() << std::endl;

    // 10. Example of move constructor
    Stack<int> s5 = std::move(s4);
    std::cout << "10. Move constructor - size of moved stack: " << s5.size() << std::endl;

    // 11. Example of move assignment operator
    Stack<int> s6;
    s6 = std::move(s5);
    std::cout << "11. Move assignment operator - size of assigned stack: " << s6.size() << std::endl;


    return 0;
}