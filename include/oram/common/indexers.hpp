#pragma once
#include "oram/common/types.hpp"

namespace _ORAM
{
  namespace Indexers
  {
    using namespace _ORAM;
    template <bool Disableassertions = false>
    INLINE Index GetArrIndex(const Index L, const Position pos, const Index depth)
    {
      if constexpr (!Disableassertions)
      {
        assert(pos != DUMMY_POSITION);
        assert((pos < (1 << L)));
      }
      assert(depth <= L);

      Index ret = 0;
      ret += (1 << depth) - 1;
      ret += (pos >> (L - depth));

      // Index ret = (pos + (V_ >> 1)) / (1 << (L_ - depth));
      CMOV(pos == DUMMY_POSITION, ret, Index{0});
      assert(ret < (1 << (L + 1)));

      return ret;
    }

    // Returns smallest the distance from the leaf that pos can be on target_path
    //
    INLINE Index GetDeepestShallowness(const Index L, const Position pos, const Position target_path)
    {
      if (pos == DUMMY_POSITION)
      {
        return L + 1;
      }

      for (Index shallowness = 0; shallowness <= L; shallowness++)
      {
        const Position b1 = pos >> shallowness;
        const Position b2 = target_path >> shallowness;
        if (b1 == b2)
        {
          return shallowness;
        }
      }

      // assert(false, "deadcode", L, pos, target_path);
      assert(false);
      return -1;
    }

    INLINE void GetPosDepthFromIndex(const Index L, const Index index, Position &pos, Index &depth)
    {
      depth = GetLogBaseTwo(index + 1);
      pos = (index - ((1 << (depth)) - 1)) << (L - depth);
    }

    // This is the eb-position of a large block where a block is in (asssuming LB_LEVELS=2):
    // 0
    // 0 0
    // 1 2 3 4
    // 1 1 2 2 3 3 4 4
    // 5 6 7 8 9 ...
    // 5 5 6 6 7 7...
    // ..
    template <unsigned int LEVELS_PER_PACK>
    INLINE Index GetHBIndex(const Index L, const Position pos, const Index depth)
    {
      constexpr Index BUCKETS_PER_PACK = (1 << LEVELS_PER_PACK) - 1;
      assert(depth % LEVELS_PER_PACK == 0); // , "Please use the root instead of ", pos);
      // X_LOG_SIMPLE(NAMED_VALUES(pos, depth, ((-1 + (1<<depth)) / LargeBucket::BUCKETS_PER_PACK), (pos >> (L_ - depth))));
      Index lBDepth = (depth / LEVELS_PER_PACK);
      Index ret = ((-1 + (1 << depth)) / BUCKETS_PER_PACK) + (pos >> (L - depth));
      return ret;
    }

    template <unsigned int LEVELS_PER_PACK>
    INLINE Index GetLBIndex(const Index L, const Position pos, const Index depth)
    {
      Index lBDepth = (depth % LEVELS_PER_PACK);
      Index ret = (-1 + (1 << lBDepth)) + (GetArrIndex(L, pos, depth) % (1 << lBDepth));
      return ret;
    }

    template <unsigned int LEVELS_PER_PACK>
    INLINE void GetBIndexFromArrIndex(const Index L, const Index evictedIndex, Index &rootIdx, Index &innerIdx)
    {
      Index pos, depth;
      GetPosDepthFromIndex(L, evictedIndex, pos, depth);
      Index rootDepth = depth - (depth % LEVELS_PER_PACK);
      rootIdx = Indexers::GetHBIndex<LEVELS_PER_PACK>(L, pos, rootDepth);
      innerIdx = Indexers::GetLBIndex<LEVELS_PER_PACK>(L, pos, depth);
    }

    template <unsigned int LEVELS_PER_PACK>
    INLINE bool PathsIntercept(const Index L, const Position path1, const Position path2, const Index depth)
    {
      return true * (path1 != DUMMY_POSITION) * (path2 != DUMMY_POSITION) * (GetArrIndex<true>(L, path1, depth) == GetArrIndex<true>(L, path2, depth));
    }
  }
}