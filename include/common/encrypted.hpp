#pragma once

#include <inttypes.h>
#include <utility>

#include "common/utils.hpp"
#include "common/encutils.hpp"
template <typename T>
struct NonEncrypted
{
    static_assert(IS_POD<T>());

    static constexpr uint64_t SIZE = sizeof(T);
    T data;

    INLINE void Encrypt(const T &in)
    {
        data = in;
    }

    INLINE void Decrypt(T &out) /*const*/
    {
        out = data;
    }

#ifndef ENCLAVE_MODE
    friend std::ostream &operator<<(std::ostream &o, const NonEncrypted &x)
    {
        T v;
        const_cast<NonEncrypted &>(x).Decrypt(v);
        o << "E{" << v << "}";
        return o;
    }
#endif

    bool operator==(const NonEncrypted &o) const
    {
        return data == o.data;
    }
};
static_assert(IS_POD<NonEncrypted<int>>());