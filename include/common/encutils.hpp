#pragma once
#include <cassert>
#include "common/defs.hpp"
#include <random>
template <class result_type_param = uint64_t>
class RandomDevice : public std::random_device
{
public:
    using result_type = result_type_param;

    static constexpr result_type max() { return std::numeric_limits<result_type>::max(); }
    static constexpr result_type min() { return std::numeric_limits<result_type>::min(); }
};

class RandGen
{
    RandomDevice<> rd;
    std::minstd_rand engine;

public:
    RandGen() : engine(rd()) {};
    uint64_t rand64()
    {
        std::uniform_int_distribution<uint64_t> d;
        return d(engine);
    }
    uint32_t rand32()
    {
        std::uniform_int_distribution<uint32_t> d;
        return d(engine);
    }
    uint8_t rand1()
    {
        std::uniform_int_distribution<short> d(0, 1);
        return d(engine);
    }
};