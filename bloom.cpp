
#include "bloom.h"

jp::BloomFilter::BloomFilter(size_t k) noexcept :
    k_{k}
{
    bits_.resize(m_, false);
}


void jp::BloomFilter::insert(const char* x) noexcept
{
    for (size_t i{}; i < k_; ++i)
    {
        Hash h{ Fnv1a(x, i) };
        bits_[h % m_] = true;
    }
}


bool jp::BloomFilter::check(const char* x) const noexcept
{
    for (size_t i{}; i < k_; ++i)
    {
        Hash h{ Fnv1a(x, i) };
        if (!bits_[h % m_])
        {
            return false;
        }
    }
    return true;
}


jp::BloomFilter::Hash jp::BloomFilter::Fnv1a(const char* s, uint32_t i)
{
    Hash h{ 0x811C9DC5 };
    static constexpr Hash p{ 0x01000193 };
    h ^= i;
    while (*s)
    {
        h ^= *s;
        h *= p;
        ++s;
    }
    return h;
}
