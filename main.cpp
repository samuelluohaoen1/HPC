#include <iostream>
#include <chrono>

struct Foo {
    u_int64_t a;
    u_int64_t b;
    u_int64_t c;
    u_int64_t d;
    u_int64_t e;
    u_int64_t f;
    u_int64_t g;
    u_int64_t h;
};

struct Bar {
    u_int64_t* a;
    u_int64_t* b;
    u_int64_t* c;
    u_int64_t* d;
    u_int64_t* e;
    u_int64_t* f;
    u_int64_t* g;
    u_int64_t* h;
};

void clear_cache()
{
    // Assuming a cache size of 400MB
    int cache_size = 400 * 1024 * 1024;  // 400MB
    int eviction_array_size = cache_size / sizeof(int);

    // Allocate an array bigger than cache size
    int* eviction_array = new int[eviction_array_size];

    // Access every element of the array. This should cause cache eviction.
    for (int i = 0; i < eviction_array_size; i++) {
        eviction_array[i] = i;
    }

    delete[] eviction_array;
}

double row_oriented_heap(int count) {
    // Create an array of Foo
    Foo* foo = new Foo[count];

    clear_cache();

    // Time the for loop below
    auto start = std::chrono::high_resolution_clock::now();

    // Iterate over foo and increment its members
    for (int i = 0; i < count; ++i)
    {
        // Increment everything
        foo[i].a++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    return elapsed.count();
}

double column_oriented_heap(int count) {
     // Create bar and initialize its members to arrays of int of size count
    Bar bar = {
        new u_int64_t[count],
        new u_int64_t[count],
        new u_int64_t[count],
        new u_int64_t[count],
        new u_int64_t[count],
        new u_int64_t[count],
        new u_int64_t[count],
        new u_int64_t[count]};

    u_int64_t* bara = bar.a;

    clear_cache();

    // Time the for loop below
    auto start = std::chrono::high_resolution_clock::now();

    // Iterate over bar and increment its members
    for (int i = 0; i < count; ++i)
    {
        // Increment everything
        bara[i]++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    // Delete bar and its members
    delete[] bar.a;
    delete[] bar.b;
    delete[] bar.c;

    return elapsed.count();
}

double row_oriented_stack(int count) {
    // Create an array of Foo
    Foo baz[count];

    clear_cache();

    // Time the for loop below
    auto start = std::chrono::high_resolution_clock::now();

    // Iterate over baz and increment its members
    for (int i = 0; i < count; ++i)
    {
        // Increment everything
        baz[i].a++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    return elapsed.count();
}

int main()
{
    int count = 10000;

    auto row_heap = row_oriented_heap(count);
    auto column_heap = column_oriented_heap(count);
    auto row_stack = row_oriented_stack(count);

    std::cout << "Row oriented heap: " << row_heap << " s\n";
    std::cout << "Column oriented heap: " << column_heap << " s\n";
    std::cout << "Row oriented stack: " << row_stack << " s\n";

    return 0;
}
