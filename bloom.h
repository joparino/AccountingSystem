
#ifndef BLOOM_H
#define BLOOM_H

#include <cstdint>
#include <vector>
#include <cstddef>

namespace jp
{
    class BloomFilter
    {
    public:
        typedef uint32_t Hash;

        BloomFilter(size_t k) noexcept;
        void insert(const char* x) noexcept;
        bool check(const char* x) const noexcept;

    private:
        static Hash Fnv1a(const char* s, uint32_t i);

    private:
        static const size_t m_{ 1000 };
        const size_t k_;
        std::vector<bool> bits_;
    };
}


#endif
