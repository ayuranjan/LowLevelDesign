#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
class String
{
private:
    char *data;
    size_t length;

public:
    // Default constructor
    String() : data(nullptr), length(0) {}

    // Type Constructor with C-style string
    String(const char *str)
    {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }

    // Type 3 : Copy constructor
    /*
    -> The & makes other a reference. 
        -> avoid making an unnecessary copy of the entire string 
    -> The const before String& means that the function cannot change the object that it is called on.
        -> It allows us to pass both const and non-const objects to the constructor.
    */
    String(const String &other) : length(other.length)
    {
        /*
            -> memory allocation might throw an exception if it fails
                ->  std::nothrow : return a null pointer if the allocation fails
                ->  noexcept : the function will not throw an exception
                -> try and catch ( auto& e ) : e.what() to get the error message
        */ 
        data = new char[length + 1];
        strcpy(data, other.data);
    }

    // Type 4 : Copy assignment operator
    /*
        -> Returning a reference to the current object allows assignment operations to be chained together
     */
    String& operator=(const String &other)
    {
        if (this != &other) // Avoid self-assignment 
        {
            delete[] data; // Deallocate old memory
            length = other.length;
            data = new char[length + 1];
            strcpy(data, other.data);
        }
        return *this; // derefencing : this is a pointer to the current object 
    }

    //Type 5 :  Move constructor
    /*
        -> && : rvalue reference
    */

    String(String &&other): data(other.data), length(other.length)
    {
        other.data = nullptr;
        other.length = 0;
    }

    // Type 6 : Move assignment operator

    String& operator=(String &&other) 
    {
        if (this != &other) // Avoid self-assignment
        {
            delete[] data; // Deallocate old memory
            data = other.data;
            length = other.length;
            other.data = nullptr; // so that no one else can access the data
            other.length = 0;
        }
        return *this;
    }

    // Length getter
    size_t size() const // const : the function cannot change the object that it is called on
    {
        return length;
    }

    // Type 7 : Overloaded addition operator
    String operator+(const String &other) const
    {
        String result; 
        result.length = length + other.length;
        result.data = new char[result.length + 1];
        strcpy(result.data, data);
        strcat(result.data, other.data);
        return result; 
        // return by value since this is a temporary object and will be destroyed after the expression
    }

    // Stream insertion operator
    /*
        -> non-member function of the class
        -> friend : allows the function to access the private members of the class
        -> Returning the stream reference allows for chaining of output operations
            -> std::cout << s1 << s2 << s3;

    */
    friend std::ostream &operator<<(std::ostream &os, const String &str)
    {
        if (!str.data) // if data is null i.e String obj ; 
        {
            os << "No Value Exist";
            return os;
        }
        os << str.data;
        return os;
    }

     // Destructor
    ~String()
    {
        delete[] data;
    }
};

int main() {
    /* Logic for String implementation:
       -> Uses a dynamically allocated char array to store the string
       -> Maintains size of the string
       -> Implements deep copy for copy operations
       -> Supports move semantics for efficient transfers
       -> Overloads operators like +, =, <<
       -> Provides basic string operations and comparisons
    */
   // Type 0 : size getter 
    // Type 0 : cout operator overloading 

    String s1; // default ctor
      /*
        The above line is equivalent to:
        const char* s = "Hello";
        String s2(s);
    */
    String s2("Hello"); // ctor with C-style string - 
    String s3 = s2; // copy ctor
    String s4;
    s4 = s2; // copy assignment
    String s5 = std::move(String("World")); // move ctor
    String s6;
     /*
        -- move :  It just casts ptr2 to an rvalue reference
        -- so it doesn't create a copy or anything. 
        --or else if we passed pointer to ptr3 then it would have created a copy
        -- if we made that pointer const then it wont create a copy but we could not modify the value of the pointer

    */
    s6 = std::move(String("C++")); // move assignment
    String s7 = s2 + " " + s5; // addition operator
    cout << s1; // cout operator overloading
    cout << s1.size(); // size getter

    return 0;
}