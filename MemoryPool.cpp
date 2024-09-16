#include <iostream>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <new>



class MemoryPool {
private:
    // Block structure: represents a free memory block
    struct Block {
        Block* next;  // Pointer to the next free block
    };

    size_t block_size_;   // Size of each memory block
    size_t pool_size_;    // Total number of blocks in the pool
    char* memory_;        // Pointer to the allocated memory chunk
    Block* free_list_;    // Pointer to the first free block

public:
    // Constructor
    MemoryPool(size_t block_size, size_t pool_size)
        : block_size_(block_size < sizeof(Block) ? sizeof(Block) : block_size),
          // Ensure block_size is at least as large as a Block structure
          pool_size_(pool_size),
          memory_(nullptr),
          free_list_(nullptr) {
        // Allocate the entire memory pool
        memory_ = new char[block_size_ * pool_size_];
        
        // Initialize the free list
        free_list_ = reinterpret_cast<Block*>(memory_);
        Block* current = free_list_;
        for (size_t i = 1; i < pool_size_; ++i) {
            // Set the 'next' pointer of the current block to the next block
            current->next = reinterpret_cast<Block*>(memory_ + i * block_size_);
            current = current->next;
        }
        current->next = nullptr;  // Last block's next pointer is null
    }

    // Destructor
    ~MemoryPool() {
        delete[] memory_;  // Release the entire memory chunk
    }

    // Allocate a block of memory
    void* allocate() {
        if (free_list_ == nullptr) {
            // No free blocks available
            throw std::bad_alloc();
        }
        
        Block* block = free_list_;
        free_list_ = free_list_->next;  // Move free_list to the next free block
        return block;  // Return the allocated block
    }

    // Deallocate a block of memory
    void deallocate(void* ptr) {
        if (ptr == nullptr) return;
        
        // Check if the pointer is within the pool's range
        if (ptr < memory_ || ptr >= memory_ + block_size_ * pool_size_) {
            throw std::invalid_argument("Invalid pointer: not from this pool");
        }

        // Cast the pointer to a Block and add it to the front of the free list
        Block* block = static_cast<Block*>(ptr);
        block->next = free_list_;
        free_list_ = block;
    }

    // Get the block size
    size_t get_block_size() const {
        return block_size_;
    }

    // Get the pool size
    size_t get_pool_size() const {
        return pool_size_;
    }

    // Get the number of free blocks
    size_t get_free_count() const {
        size_t count = 0;
        Block* current = free_list_;
        while (current != nullptr) {
            ++count;
            current = current->next;
        }
        return count;
    }
};

// Example usage
int main() {
    /* Logic for Custom Memory Pool implementation:
   -> Pre-allocate a large chunk of memory divided into fixed-size blocks
   -> Maintain a free list of available blocks
   -> Allocate by removing a block from the free list
   -> Deallocate by adding the block back to the free list
   -> Use a linked list structure within the memory blocks themselves
   -> Provide O(1) time complexity for allocation and deallocation

   Diagram:
   
   MemoryPool
   |
   +-- memory_ (char array)
       |
       +-- [Block] -> [Block] -> [Block] -> ... -> [Block]
           ^
           |
       free_list_ (points to first free block)

   Where:
   - Each [Block] is a fixed-size memory block
   - Blocks in the free list are linked together
   - Allocated blocks are removed from the free list
   - Deallocated blocks are added back to the free list
*/
    // Create a memory pool with 16-byte blocks and 100 blocks total
    MemoryPool pool(16, 100);

    std::cout << "Block size: " << pool.get_block_size() << " bytes" << std::endl;
    std::cout << "Pool size: " << pool.get_pool_size() << " blocks" << std::endl;
    std::cout << "Initial free blocks: " << pool.get_free_count() << std::endl;

    // Allocate some memory
    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();
    void* ptr3 = pool.allocate();

    std::cout << "After allocating 3 blocks, free blocks: " << pool.get_free_count() << std::endl;

    // Deallocate memory
    pool.deallocate(ptr2);
    pool.deallocate(ptr1);

    std::cout << "After deallocating 2 blocks, free blocks: " << pool.get_free_count() << std::endl;

    // Allocate again
    void* ptr4 = pool.allocate();

    std::cout << "After allocating 1 block, free blocks: " << pool.get_free_count() << std::endl;

    // Clean up
    pool.deallocate(ptr3);
    pool.deallocate(ptr4);

    return 0;
}