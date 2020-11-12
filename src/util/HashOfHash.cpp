#include "HashOfHash.h"
#include "crypto/ShortHash.h"

namespace std
{

size_t
hash<aiblocks::uint256>::operator()(aiblocks::uint256 const& x) const noexcept
{
    size_t res =
        aiblocks::shortHash::computeHash(aiblocks::ByteSlice(x.data(), 8));

    return res;
}
}
