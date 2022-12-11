#pragma once

#include <ostream>
#include <vector>

namespace fvs {
// using index_t = uint_fast32_t;
using index_t = std::size_t;

// template<typename T=index_t>
// using IndexList = std::vector<T>;
using IndexList = std::vector<index_t>;

// template<typename T=index_t>
// using IndexListList = std::vector<IndexList<T>>;
using IndexListList = std::vector<IndexList>;

// template<typename T>
// std::ostream&
// operator<<(std::ostream& os, const std::vector<T>& vec)
// {
//     bool first = true;
//     for (const auto& e : vec) {
//         if (!first)
//             os << ", ";
//         os << e;
//     }

//     return os;
// }
}