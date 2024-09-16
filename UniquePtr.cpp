#include <iostream>
#include <utility>
using namespace std;
template <typename T>
class custom_unique_ptr {
private:
    T* ptr;  // Raw pointer to manage

public:
    // 0. Pointer getter
    T* get() const {
        return ptr;
    }

    // 0. Dereference operator
    T& operator*() const {
        return *ptr;
    }
    // 1. Default constructor
    custom_unique_ptr() : ptr(nullptr) {}

    // 2. Constructor with raw pointer
    custom_unique_ptr(T* p) : ptr(p) {}

    

    // 3. Move constructor
    custom_unique_ptr(custom_unique_ptr&& other)  : ptr(other.ptr) {
        other.ptr = nullptr;  // Transfer ownership, set other's ptr to null
    }

    // 4. Move assignment operator
    custom_unique_ptr& operator=(custom_unique_ptr&& other)  {
        if (this != &other) {
            delete ptr;  // Delete current object i.e value of ptr
            ptr = other.ptr;  // Transfer ownership
            other.ptr = nullptr;  // Set other's ptr to null
        }
        return *this;
    }

    // 5. Reset with a new pointer
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }


    // 7. Member access operator
    T* operator->() const {
        return ptr;
    }

 
    // Disable copy constructor and copy assignment operator
    /*
        -- If we don't disable the copy constructor and copy assignment operator, 
        -- then the compiler will generate them for us, 
        -- and we will end up with two custom_unique_ptrs owning the same object, 
        -- which violates the single ownership semantics of unique_ptr.
        -- delete is a C++11 feature that explicitly deletes this function.
    */
    custom_unique_ptr(const custom_unique_ptr&) = delete;
    custom_unique_ptr& operator=(const custom_unique_ptr&) = delete;

    // 3. Destructor
    ~custom_unique_ptr() {
        delete ptr;  // Automatically delete the managed object
    }
};

// Main method to showcase examples of the custom_unique_ptr class usage

int main() {
    /* Logic for custom_unique_ptr implementation:
       -> It should Manages a single object through a pointer
       -> It should ensures that only one custom_unique_ptr owns the object at a time
       -> It should Automatically deletes the managed object when the custom_unique_ptr is destroyed
       -> It should Allow moving of ownership between custom_unique_ptrs
       -> It should Prevent copying to maintain single ownership semantics - so no copy constructor or copy assignment operator
    */

    custom_unique_ptr<int> ptr1; // default ctor
    custom_unique_ptr<int> ptr2(new int(42)); // param ctor
    cout << *ptr2; // dereference operator
    cout << (ptr2.get() == nullptr ? "null" : "not null"); // get raw pointer
    custom_unique_ptr<int> ptr3 = std::move(ptr2); // move copy ctor
    custom_unique_ptr<int> ptr4;
    /*
        -- move :  It just casts ptr2 to an rvalue reference
        -- so it doesn't create a copy or anything. 
        --or else if we passed pointer to ptr3 then it would have created a copy
        -- if we made that pointer const then it wont create a copy but we could not modify the value of the pointer

    */
    ptr4 = std::move(ptr3); // move copy assignment operator
    ptr4.reset(new int(100)); // reset
    cout << ptr4->value; // member access operator

     // Don't say untill asked : 7. Example of custom object
    // struct MyClass { 
    //     int value;
    //     MyClass(int v) : value(v) { std::cout << "MyClass constructed" << std::endl; }
    //     ~MyClass() { std::cout << "MyClass destructed" << std::endl; }
    // };

    // {
    //     custom_unique_ptr<MyClass> ptr5(new MyClass(200));
    //     std::cout << "7. Custom object value: " << ptr5->value << std::endl;
    // }  // ptr5 goes out of scope here, MyClass should be destructed

    // std::cout << "End of main" << std::endl;

    return 0;
}