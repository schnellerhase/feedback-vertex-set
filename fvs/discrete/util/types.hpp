#pragma once

#include <vector>

namespace fvs {

using index_t = std::size_t; // uint_fast32_t
using IndexList = std::vector<index_t>;
using IndexListList = std::vector<IndexList>;

}