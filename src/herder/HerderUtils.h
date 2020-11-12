#pragma once

// Copyright 2017 AiBlocks Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "xdr/AiBlocks-types.h"
#include <vector>

namespace aiblocks
{

struct SCPEnvelope;
struct SCPStatement;
struct AiBlocksValue;

std::vector<Hash> getTxSetHashes(SCPEnvelope const& envelope);
std::vector<AiBlocksValue> getAiBlocksValues(SCPStatement const& envelope);
}
