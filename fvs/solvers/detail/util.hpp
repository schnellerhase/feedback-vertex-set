#pragma once

#include <limits>

namespace fvs::detail {

static constexpr double ROUNDING_EPS = 2e-6;
static constexpr double INF = std::numeric_limits<double>::max();

template<typename T = double>
bool
round_to_bool(T value)
{
    return (value + ROUNDING_EPS) >= 1;
}

}