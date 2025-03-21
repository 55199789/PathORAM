
#include <benchmark/benchmark.h>
#include <vector>
#include <limits>
#include <random>
#include "otree/otree.hpp"
#include "oram/pathoram/oram.hpp"

class OMapDataFixtureInsert : public benchmark::Fixture
{
public:
    size_t n;
    using IndexType = size_t;
    std::random_device rd;
    using ORAMClient = _ORAM::PathORAM::ORAMClient::ORAMClient<_OBST::Node, ORAM__Z, false, 4>;
    using OramClient = _OBST::OramClient::OramClient<ORAMClient>;
    _OBST::OBST::OBST<OramClient> *omap;
    void SetUp(const ::benchmark::State &state) override
    {
        n = state.range(0);
        std::cout << "insertion with n: " << n << std::endl;
        omap = new _OBST::OBST::OBST<OramClient>(n);
    }

    void TearDown(const ::benchmark::State &) override
    {
        delete omap;
    }
};

BENCHMARK_DEFINE_F(OMapDataFixtureInsert, OMap)
(benchmark::State &state)
{
    for (auto _ : state)
    {
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(0, std::numeric_limits<int>::max());
        for (uint32_t i = 0; i < std::min((size_t)1024, n); i++)
            omap->Insert(dist(gen), dist(gen));
    }
}

static void CustomizedArgsN(benchmark::internal::Benchmark *b)
{
    for (size_t i = 8; i <= 26; i++) // n := 2**i
    {
        size_t n = 1ll << i;
        b->Args({(int64_t)n});
    }
}

BENCHMARK_REGISTER_F(OMapDataFixtureInsert, OMap)->Apply(CustomizedArgsN)->MeasureProcessCPUTime()->UseRealTime();