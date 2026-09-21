#include <borealis/presentation.hpp>

#include <gtest/gtest.h>

#include <limits>

TEST(Presentation, RejectsInvalidAndUnsupportedRates) {
    using borealis::presentation::set_preferred_frame_rate;

    EXPECT_FALSE(set_preferred_frame_rate(-1.0f));
    EXPECT_FALSE(set_preferred_frame_rate(std::numeric_limits<float>::infinity()));
    EXPECT_FALSE(set_preferred_frame_rate(std::numeric_limits<float>::quiet_NaN()));

#if !defined(__ANDROID__)
    EXPECT_FALSE(set_preferred_frame_rate(0.0f));
    EXPECT_FALSE(set_preferred_frame_rate(120.0f));
#endif
}
