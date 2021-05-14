#pragma once
#include <numeric>
#include <numbers>

static constexpr float fMax = std::numeric_limits<float>::max();
static constexpr float fMin = std::numeric_limits<float>::lowest();

static constexpr float fPI = (float)std::numbers::pi;
static constexpr double PI = std::numbers::pi;