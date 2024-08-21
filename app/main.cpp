#include "otree/otree.hpp"
#include "oram/pathoram/oram.hpp"
#include <sys/resource.h>
#include <cassert>
#include <iostream>
#include <random>
#include <chrono>
class Timer
{
public:
    Timer()
        : start_clock(std::chrono::steady_clock::now()),
          last_stop(std::chrono::steady_clock::now())
    {
    }

    double get_total_time() const
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::nanoseconds>(now - this->start_clock).count() / 1000000.0;
    }

    double get_interval_time()
    {
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        double interval_time = std::chrono::duration_cast<std::chrono::nanoseconds>(now - this->last_stop).count();
        this->last_stop = now;
        return interval_time / 1000000000.0;
    }

private:
    std::chrono::steady_clock::time_point start_clock;
    std::chrono::steady_clock::time_point last_stop;
};

static long getMemoryUsage()
{
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

using ORAMClient = _ORAM::PathORAM::ORAMClient::ORAMClient<_OBST::Node, ORAM__Z, false, 4>;
using OramClient = _OBST::OramClient::OramClient<ORAMClient>;

int main(int argc, char **argv)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    size_t n = argc >= 2 ? std::atoi(argv[1]) : 1 << 21; // 1 << 16;
    // uint32_t T = argc >= 3 ? std::atoi(argv[2]) : n;       // 1 << 16;
    size_t T = std::min(size_t(1024), n);
    std::vector<uint64_t> data(n);
    std::uniform_int_distribution<uint64_t> dis(0, std::numeric_limits<int64_t>::max());
    for (uint32_t i = 0; i < n; i++)
        data[i] = i;
    std::shuffle(data.begin(), data.end(), gen);
    std::cout << "Data size: " << sizeof(data[0]) * 2 * n / 1024.0 / 1024.0 << " MB" << std::endl;
    std::cout << "Memory usage before setup: " << getMemoryUsage() / 1024.0 << " MB" << std::endl;
    Timer t;
    _OBST::OBST::OBST<OramClient> x(n);
    double create_time = t.get_interval_time();
    std::cout << "Setup time: " << create_time << " s" << std::endl;
    for (uint32_t i = 0; i < T; i++)
        x.Insert(i, data[i]);
    double insert_time = t.get_interval_time();
    std::cout << "Insert time: " << insert_time << " s, avg " << insert_time * 1000000.0 / T << " us/op" << std::endl;
    _OBST::V a;
    for (uint32_t i = 0; i < T; i++)
    {
        x.Get(i, a);
        assert(a == data[i]);
    }
    double search_time = t.get_interval_time();
    std::cout << "Search time: " << search_time << " s, avg " << search_time * 1000000.0 / T << " us/op" << std::endl;
    std::cout << "Memory usage after setup: " << getMemoryUsage() / 1024.0 << " MB" << std::endl;
    std::cout << "Finished!" << std::endl;
    return 0;
}

// {
//     for (size_t _ = 8; _ <= 26; _++) // n := 2**i
//     {
//         size_t n = 1ll << _;
//         std::cout << "n = " << n << std::endl;
//         tmp(n);
//     }
// }