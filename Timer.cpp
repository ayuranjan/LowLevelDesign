#include <iostream>
#include <ctime>

class Timer {
private:
    clock_t start_time;
    clock_t end_time;
    bool running;

public:
    // Default constructor
    Timer() : start_time(0), end_time(0), running(false) {}

    // Start the timer
    void start() {
        start_time = clock(); // clock() function returns the number of clock ticks elapsed since the program was launched
        running = true;
    }

    // Stop the timer
    void stop() {
        end_time = clock();
        running = false;
    }

    // Get elapsed time in seconds
    double elapsed() const {
        clock_t end = running ? clock() : end_time;
        return static_cast<double>(end - start_time) / CLOCKS_PER_SEC; // CLOCKS_PER_SEC is a constant representing the number of clock ticks per second from ctime header
    }

    // Reset the timer
    void reset() {
        start_time = 0;
        end_time = 0;
        running = false;
    }

    // Check if timer is running
    bool is_running() const {
        return running;
    }
};

// Main method to showcase examples of the Timer class usage
int main() {
    /* Logic for Timer implementation:
       -> Uses clock() function from ctime header for time measurement
       -> we have to  maintain start and end time points ,so variable for each
       -> Provides methods to start, stop, and reset the timer
       -> Calculates elapsed time in seconds
    */

    Timer t; // default ctor
    t.start(); // start timer
    
    // Simulate some work
    for(int i = 0; i < 1000000; ++i) {
        int a = i * i;
    }
    
    t.stop(); // stop timer
    std::cout << "Elapsed time: " << t.elapsed() << " seconds" << std::endl; // get elapsed time in seconds
    std::cout << "Timer running: " << (t.is_running() ? "Yes" : "No") << std::endl; // check if timer is running
    
    t.reset(); // reset timer
    t.start(); // start again
    t.stop();
    std::cout << "After reset and quick start/stop: " << t.elapsed() << " seconds" << std::endl;

    return 0;
}