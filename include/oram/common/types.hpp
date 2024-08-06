#pragma once
#include <inttypes.h>
#include "common/defs.hpp"
#include "common/mov_intrinsics.hpp"

namespace _ORAM
{
  typedef uint64_t Index;
  typedef uint64_t Position;
  typedef uint64_t Address;

  constexpr inline Address DUMMY_ADDRESS = ~0;
  constexpr inline Position DUMMY_POSITION = ~0;

  struct ORAMAddress
  {
    Address address;
    Position position;

    bool operator==(const ORAMAddress &o) const
    {
      return (address == o.address) & (position == o.position);
    }

    static consteval INLINE ORAMAddress DUMMY() { return ORAMAddress{DUMMY_ADDRESS, DUMMY_POSITION}; }
  };
}

template <>
INLINE void CMOV<_ORAM::ORAMAddress>(const uint64_t &condition, _ORAM::ORAMAddress &A, const _ORAM::ORAMAddress &B)
{
  CMOV(condition, A.address, B.address);
  CMOV(condition, A.position, B.position);
}