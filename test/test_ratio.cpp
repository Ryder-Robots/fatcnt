#include <gtest/gtest.h>
#include <stdint.h>
#include <fatcnt/math/ratio.hpp>

using namespace rrobot;

class TestRatio : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
    }

    void TearDown() override {
        // Teardown code
    }
};

// Tests for moving controller axis 
TEST(TestScaleNumber, testChar) {
    Ratio<int8_t, uint8_t> scale = Ratio<int8_t, uint8_t>(-1, 1, 0, 255);

    uint8_t result = scale.scale(1);
    EXPECT_EQ(255, result);
}

TEST(TestScaleNumber, testChar2) {
    Ratio<int8_t, uint8_t> scale = Ratio<int8_t, uint8_t>(-1, 1, 0, 255);

    uint8_t result = scale.scale(-1);
    EXPECT_EQ(0, result);
}

TEST(TestScaleNumber, testChar3) {
    Ratio<int8_t, uint8_t> scale = Ratio<int8_t, uint8_t>(-1, 1, 0, 255);

    uint8_t result = scale.scale(0);
    EXPECT_EQ(127, result);
}

// Tests for ultrasonics
TEST(TestScaleNumber, testChar4) {
    Ratio<int32_t, uint8_t> scale = Ratio<int32_t, uint8_t>(2, 500, 1, 255);

    uint8_t result = scale.scale(2);
    EXPECT_EQ(1, result);

    result = scale.scale(250);
    EXPECT_EQ(127, result);

    result = scale.scale(500);
    EXPECT_EQ(255, result);
}