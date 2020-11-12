#pragma once
#include <xdr/AiBlocks-types.h>

namespace std
{
template <> struct hash<aiblocks::uint256>
{
    size_t operator()(aiblocks::uint256 const& x) const noexcept;
};
}
