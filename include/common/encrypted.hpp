#pragma once

#include <inttypes.h>
#include <utility>

#include "common/utils.hpp"
#include "common/encutils.hpp"

#define IV_SIZE 12

namespace Concepts
{
    template <typename T>
    concept Encryptable = requires(typename T::Encrypted_t et) {
        { et };
    };
}

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

template <typename PublicData, typename PrivateData>
    requires(IS_POD<PublicData>()) && (IS_POD<PrivateData>()) && ::Concepts::Encryptable<PrivateData>
struct MixedEncryptable
{
    using PublicData_t = PublicData;
    using PrivateData_t = PrivateData;
    PublicData pub;
    PrivateData priv;

    bool operator==(const MixedEncryptable &o) const
    {
        return (pub == o.pub) * (priv == o.priv);
    }

    // Classes that extend this need to declare Encrypted_t.
    //
};
