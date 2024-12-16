
#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <time.h>
#include <cassert>
#include <cstring>


namespace SimpleStorage
{
    template <typename T>
    class DynamicArray
    {
    public:
        DynamicArray()
        {
        }

        DynamicArray(DynamicArray&) = delete;

        ~DynamicArray()
        {
            delete[] data;
        }

        size_t GetCount() const { return count; }
        size_t GetCapacity() const {return capacity; }

        void Push(T element)
        {
            if (capacity == 0)
            {
                data = new T[16];
                assert(data != nullptr);
                capacity = 16;
            }
            else if (count == capacity)
            {
                // Realloc
                T* new_data = new T[capacity * 2];

                memcpy(new_data, data, sizeof(T) * capacity);

                capacity = capacity * 2;
                delete[] data;
                data = new_data;
            }

            data[count] = element;
            count++;
        }
        
        T& Get(size_t index) const
        {
            assert(index >= 0 && index < capacity && "Index out of bounds");
            return data[index];
        }

        void Clear()
        {
            count = 0;
        }

    private:
        T* data = nullptr;
        size_t capacity = 0;
        size_t count = 0;
    };

} // SimpleStorage

namespace math
{
    template <typename T>
    inline T max(T a, T b) {
        return a > b ? a : b;
    }

    template <typename T>
    inline T min(T a, T b) {
        return a < b ? a : b;
    }

    template <typename T>
    inline T wrap(T value, T lower, T upper) {
        T range = upper - lower;
        return lower + ((((value - lower) % range) + range) % range);
    }
} // namespace math

namespace NGrandom
{
    inline void seed(){
        srand(time(NULL));
    }

    // Random float between (0, 1)
    inline float unsigned_float(){
        return rand() / float(RAND_MAX);
    }

    // Random float between (-1, 1)
    inline float signed_float(){
        return -1.f + unsigned_float() * 2;
    }

} // namespace random



#endif // HELPERS_H