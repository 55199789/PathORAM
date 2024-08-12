#include <iostream>
#include "oram/common/block.hpp"
#include "oram/common/bucket.hpp"
#include "oram/common/indexers.hpp"
#include "oram/common/oram_client_interface.hpp"

int main()
{
    _ORAM::Bucket::DefaultBucketMetadata<> metadata;
    _ORAM::Block::Block<_ORAM::Block::DefaultBlockData> block;
    _ORAM::Indexers::GetArrIndex(1, 2, 3);
    return 0;
}