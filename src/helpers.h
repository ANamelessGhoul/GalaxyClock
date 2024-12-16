
#ifndef HELPERS_H
#define HELPERS_H

#include <stdlib.h>
#include <time.h>


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