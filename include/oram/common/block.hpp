#pragma once
#include "common/cpp_extended.hpp"
#include "common/defs.hpp"
#include "common/dummy.hpp"
#include "common/encrypted.hpp"
#include "common/mov_intrinsics.hpp"
#include "oram/common/types.hpp"

#include <type_traits>

namespace _ORAM::Block
{
  struct DefaultBlockData
  {
    uint64_t v;
    static consteval INLINE DefaultBlockData DUMMY() { return DefaultBlockData{static_cast<uint64_t>(-1)}; }
    bool operator==(const DefaultBlockData &other) const { return v == other.v; }
    friend std::ostream &operator<<(std::ostream &o, const DefaultBlockData &x)
    {
      o << x.v;
      return o;
    }
  };

  template <typename T = DefaultBlockData, bool ENCRYPT_BLOCKS = ORAM__ENCRYPT_BLOCKS>
    requires(IS_POD<T>())
  struct Block
  {
    using _T = T;

    T data;

    auto operator==(const Block &other) const
    {
      return data == other.data;
    }

    friend std::ostream &operator<<(std::ostream &o, const Block &x)
    {
      o << "(" << x.data << ")";
      return o;
    }
    static consteval INLINE Block DUMMY() { return Block{::DUMMY<T>()}; }

    using Encrypted_t = NonEncrypted<Block>;
  }; // struct Block

  static_assert(IS_POD<Block<DefaultBlockData>>());
  static_assert(IS_POD<Block<DefaultBlockData>::Encrypted_t>());
} // namespace _ORAM::Block

namespace _ORAM::StashedBlock
{
  template <typename Block = _ORAM::Block::Block<_ORAM::Block::DefaultBlockData>>
  struct StashedBlock
  {
    using Block_t = Block;

    bool cached;
    ORAMAddress oaddress;
    Block block;

    static consteval INLINE StashedBlock DUMMY()
    {
      return StashedBlock{
          /*cached=*/false,
          ::DUMMY<ORAMAddress>(),
          ::DUMMY<Block>()};
    }

    bool operator==(const StashedBlock &other) const
    {
      // X_LOG("[StashedBlock::operator==]", "this: ", *this, " other: ", other, " are equal:", (cached == other.cached && oaddress == other.oaddress && block == other.block), cached == other.cached, oaddress == other.oaddress, block == other.block);
      return cached == other.cached && oaddress == other.oaddress && block == other.block;
    }
  }; // struct StashedBlock

  static_assert(IS_POD<StashedBlock<Block::Block<Block::DefaultBlockData>>>());

} // namespace _ORAM::StashedBlock

template <>
INLINE void CMOV<_ORAM::Block::DefaultBlockData>(const uint64_t &condition, _ORAM::Block::DefaultBlockData &A, const _ORAM::Block::DefaultBlockData &B)
{
  CMOV(condition, A.v, B.v);
}

template <typename T, bool W>
INLINE void CMOV(const uint64_t &condition, _ORAM::Block::Block<T, W> &A, const _ORAM::Block::Block<T, W> &B)
{
  CMOV(condition, A.data, B.data);
}

template <typename T>
INLINE void CMOV(const uint64_t &condition, _ORAM::StashedBlock::StashedBlock<T> &A, const _ORAM::StashedBlock::StashedBlock<T> &B)
{
  CMOV(condition, A.cached, B.cached);
  CMOV(condition, A.oaddress, B.oaddress);
  CMOV(condition, A.block, B.block);
}

template <>
INLINE void CXCHG<_ORAM::Block::DefaultBlockData>(const uint64_t &condition, _ORAM::Block::DefaultBlockData &A, _ORAM::Block::DefaultBlockData &B)
{
  const _ORAM::Block::DefaultBlockData C = A;
  CMOV(condition, A, B);
  CMOV(condition, B, C);
}

// UNDONE(): figure out how to not have to overload CXCHG and TSET here:
//
OVERLOAD_TSET_CXCHG(_ORAM::StashedBlock::StashedBlock<T>, typename T)

OVERLOAD_TSET_CXCHG(_ORAM::Block::Block<T COMMA W>, typename T, bool W)

// UNDONE: move this to tthe right file, we just moved here beause we know
// the include order won't break.
//
template <typename T>
struct TaggedT
{
  bool isDummy;
  uint64_t tag;
  T v;

  // UNDONE(): this function needs to stop being a member function and be a template function on it's
  // own. The main reason is that T::DUMMY() for int is not callable.
  //
  static consteval TaggedT DUMMY()
  {
    TaggedT ret;
    ret.isDummy = true;
    ret.tag = static_cast<uint64_t>(-1);
    ret.v = T::DUMMY();
    return ret;
  }
};

template <typename T>
INLINE void CMOV(const uint64_t &condition, TaggedT<T> &A, const TaggedT<T> &B)
{
  CMOV(condition, A.isDummy, B.isDummy);
  CMOV(condition, A.tag, B.tag);
  CMOV(condition, A.v, B.v);
}

OVERLOAD_TSET_CXCHG(TaggedT<T>, typename T);