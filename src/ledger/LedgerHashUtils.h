#pragma once

// Copyright 2018 AiBlocks Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "crypto/ShortHash.h"
#include "ledger/GeneralizedLedgerEntry.h"
#include "xdr/AiBlocks-ledger.h"
#include <functional>

// implements a default hasher for "LedgerKey"
namespace std
{
template <> class hash<aiblocks::Asset>
{
  public:
    size_t
    operator()(aiblocks::Asset const& asset) const
    {
        size_t res = asset.type();
        switch (asset.type())
        {
        case aiblocks::ASSET_TYPE_NATIVE:
            break;
        case aiblocks::ASSET_TYPE_CREDIT_ALPHANUM4:
        {
            auto& a4 = asset.alphaNum4();
            res ^= aiblocks::shortHash::computeHash(
                aiblocks::ByteSlice(a4.issuer.ed25519().data(), 8));
            res ^= a4.assetCode[0];
            break;
        }
        case aiblocks::ASSET_TYPE_CREDIT_ALPHANUM12:
        {
            auto& a12 = asset.alphaNum12();
            res ^= aiblocks::shortHash::computeHash(
                aiblocks::ByteSlice(a12.issuer.ed25519().data(), 8));
            res ^= a12.assetCode[0];
            break;
        }
        }
        return res;
    }
};

template <> class hash<aiblocks::LedgerKey>
{
  public:
    size_t
    operator()(aiblocks::LedgerKey const& lk) const
    {
        size_t res;
        switch (lk.type())
        {
        case aiblocks::ACCOUNT:
            res = aiblocks::shortHash::computeHash(
                aiblocks::ByteSlice(lk.account().accountID.ed25519().data(), 8));
            break;
        case aiblocks::TRUSTLINE:
        {
            auto& tl = lk.trustLine();
            res = aiblocks::shortHash::computeHash(
                aiblocks::ByteSlice(tl.accountID.ed25519().data(), 8));
            res ^= hash<aiblocks::Asset>()(tl.asset);
            break;
        }
        case aiblocks::DATA:
            res = aiblocks::shortHash::computeHash(
                aiblocks::ByteSlice(lk.data().accountID.ed25519().data(), 8));
            res ^= aiblocks::shortHash::computeHash(aiblocks::ByteSlice(
                lk.data().dataName.data(), lk.data().dataName.size()));
            break;
        case aiblocks::OFFER:
            res = aiblocks::shortHash::computeHash(aiblocks::ByteSlice(
                &lk.offer().offerID, sizeof(lk.offer().offerID)));
            break;
        case aiblocks::CLAIMABLE_BALANCE:
            res = aiblocks::shortHash::computeHash(aiblocks::ByteSlice(
                lk.claimableBalance().balanceID.v0().data(), 8));
            break;
        default:
            abort();
        }
        return res;
    }
};

template <> class hash<aiblocks::GeneralizedLedgerKey>
{
  public:
    size_t
    operator()(aiblocks::GeneralizedLedgerKey const& glk) const
    {
        switch (glk.type())
        {
        case aiblocks::GeneralizedLedgerEntryType::LEDGER_ENTRY:
            return hash<aiblocks::LedgerKey>()(glk.ledgerKey());
        case aiblocks::GeneralizedLedgerEntryType::SPONSORSHIP:
            return aiblocks::shortHash::computeHash(aiblocks::ByteSlice(
                glk.sponsorshipKey().sponsoredID.ed25519().data(), 8));
        case aiblocks::GeneralizedLedgerEntryType::SPONSORSHIP_COUNTER:
            return aiblocks::shortHash::computeHash(aiblocks::ByteSlice(
                glk.sponsorshipCounterKey().sponsoringID.ed25519().data(), 8));
        default:
            abort();
        }
    }
};
}
