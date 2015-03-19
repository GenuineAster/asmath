#include <chrono>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <algorithm>
#include <asmath/aabb.h>

bool aabb_intersects_c(AABB_i32 *a, AABB_i32 *b);
void benchmark();
int main()
{
    benchmark();
}


void benchmark()
{
    const int aabbAmount  = 10000;
    const int aabbMinPos  = 0;
    const int aabbMaxPos  = 1000000;
    const int aabbMinSize = 1;
    const int aabbMaxSize = 1000000;

    AABB_i32* aabbs = new AABB_i32[aabbAmount];

    std::default_random_engine generator;
    std::uniform_int_distribution<int32_t> positionDistribution(aabbMinPos, aabbMaxPos);
    std::uniform_int_distribution<int32_t> sizeDistribution(aabbMinSize, aabbMaxSize);

    for(int32_t i = 0; i < aabbAmount; ++i)
    {   
        aabbs[i].left = positionDistribution(generator);
        aabbs[i].top = positionDistribution(generator);
        aabbs[i].right = aabbs[i].left + sizeDistribution(generator);
        aabbs[i].bottom = aabbs[i].top + sizeDistribution(generator);
    }; 

    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

    for(int32_t i = 0; i < aabbAmount; ++i)
    {   
        for(int32_t j = i + 1; j < aabbAmount; ++j)
        {   
            bool collision = aabb_intersects_c(&aabbs[i], &aabbs[j]);
            collision = aabb_intersects_c(&aabbs[j], &aabbs[i]);
        }   
    }

    auto t = std::chrono::high_resolution_clock::now()-start;

    std::cout<<std::setw(25)<<"Conventional (C): "<<std::chrono::duration_cast<std::chrono::milliseconds>(t).count()<<std::endl;

    start = std::chrono::high_resolution_clock::now();

    for(int32_t i = 0; i < aabbAmount; ++i)
    {   
        for(int32_t j = i + 1; j < aabbAmount; ++j)
        {   
            bool collision = aabb_intersects_conventional_i32(&aabbs[i], &aabbs[j]);
            collision = aabb_intersects_conventional_i32(&aabbs[j], &aabbs[i]);
        }   
    }

    t = std::chrono::high_resolution_clock::now()-start;

    std::cout<<std::setw(25)<<"Conventional (ASM): "<<std::chrono::duration_cast<std::chrono::milliseconds>(t).count()<<std::endl;

    start = std::chrono::high_resolution_clock::now();

    for(int32_t i = 0; i < aabbAmount; ++i)
    {   
        for(int32_t j = i + 1; j < aabbAmount; ++j)
        {   
            bool collision = aabb_intersects_i32(&aabbs[i], &aabbs[j]);
            collision = aabb_intersects_i32(&aabbs[j], &aabbs[i]);
        }
    }

    t = std::chrono::high_resolution_clock::now()-start;

    std::cout<<std::setw(25)<<"SIMD: "<<std::chrono::duration_cast<std::chrono::milliseconds>(t).count()<<std::endl;
    std::cout<<"These results are not trustworthy due to loop and other overhead. Just use them to see \"which is faster\" instead of as a real performance metric. Use callgrind for that."<<std::endl;
}

bool aabb_intersects_c(AABB_i32 *a, AABB_i32 *b)
{
    return !((b->left)>(a->right) || (b->right)<(a->left) || (b->top)>(a->bottom) || (b->bottom)<(a->top));
}
