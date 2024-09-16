#include <iostream>
using namespace std;

template <typename T>
class sharedptr {
private:
    T* res;
    int* counter; // why int* and not int? because we need to keep track of the number of custom_shared_ptrs that own the object 

     // Increment counter
    void incrementcounter() {
        if (counter) {
            (*counter)++;
        }
    }

    // Decrement counter
    void decrementcounter() {
        if (counter) {
            (*counter)--;
            if ((*counter) == 0) {
                if (res) {
                    delete res;
                    delete counter;
                    res = nullptr;
                    counter = nullptr;
                }
            }
        }
    }

public:
    // Constructor - default ctor
    sharedptr() : res(nullptr), counter(new int(0)) {
        cout << "default ctor was called\n";
    }
    // param ctor 
    sharedptr(T* ptr) : res(ptr), counter(new int(1)) {
        cout << "param ctor was called\n";
    }

    // Copy constructor
    sharedptr(const sharedptr<T>& ptr) {
        res = ptr.res;
        counter = ptr.counter;
        incrementcounter(); // this is done because we are creating a new object that points to the same resource
    }

    // Move constructor
    sharedptr(sharedptr<T>&& ptr) {
        res = ptr.res;
        counter = ptr.counter;
        ptr.res = nullptr;
        ptr.counter = nullptr;
        // we don't need to increment the counter because we are just moving the ownership so one object leaves and the other one enters the scope
    }

    // Copy assignment operator
    sharedptr<T>& operator=(const sharedptr<T>& ptr) {
        if (this != &ptr) {
            decrementcounter();// decrement the counter of the current object because we are going to assign a new object to it 
            res = ptr.res;
            counter = ptr.counter;
            incrementcounter(); // increment the counter of the new object because we are assigning a new object to it 
        }
        return *this;
    }

    // Move assignment operator
    sharedptr<T>& operator=(sharedptr<T>&& ptr) {
        if (this != &ptr) {
            decrementcounter();
            res = ptr.res;
            counter = ptr.counter;
            ptr.res = nullptr;
            ptr.counter = nullptr; // we don't need to increment the counter because we are just moving the ownership so one object leaves and the other one enters the scope
        }
        return *this;
    }


    // both paramertiized and default reset 
    void reset(T* ptr = nullptr) {
        decrementcounter();
        res = ptr;
        counter = new int(ptr ? 1 : 0);
       
    }

    // Get count
    int get_count() {
        if (counter) {
            return (*counter);
        }
        return 0;
    }

    // Overload operators
    T* operator->() {
        return res;
    }

    T& operator*() {
        return (*res);
    }

    // Get raw pointer
    T* get() {
        return res;
    }

    // Destructor
    ~sharedptr() {
        decrementcounter();
    }
};

int main() {
     /* Logic for custom_shared_ptr implementation:
       -> Manages a single object through a pointer
       -> we need to Keep track of the number of custom_shared_ptrs that own the object so we need a pointer to counter variable 
       -> we also need to Automatically deletes the managed object when the last owning custom_shared_ptr is destroyed
       -> it should  Allow copying of ownership between custom_shared_ptrs
    */
    sharedptr<int> ptr1; // default ctor
    sharedptr<int> ptr2(new int(10)); // param ctor
    sharedptr<int> ptr3(ptr2); // copy ctor
    ptr3 = ptr2; // copy assignment operator
    sharedptr<int> ptr4(std::move(ptr1)); // move copy ctor
    ptr2 = std::move(ptr3); // move copy assignment operator

    ptr1.reset();
    ptr1.reset(new int(5));

    cout << (*ptr1); // implementing * operator
    ptr1->func(); // implementing -> operator

    ptr1.get(); // raw pointer
    ptr1.get_count(); // number of objects pointing to the same resource

    return 0;
}
