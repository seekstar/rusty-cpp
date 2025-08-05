#include <cstdio>
#include <gtest/gtest.h>
#include <rusty/macro.h>
#include <rusty/primitive.h>

class Test : public ::testing::Test {
public:
	void SetUp() override {}
	void TearDown() override {}
};

TEST_F(Test, Asserts) {
	rusty_assert(1);
	rusty_assert_eq(
		rusty::next_power_of_two((size_t)42), 64, "Math doesn't exist anymore"
	);
	rusty_assert_eq(rusty::next_power_of_two((size_t)233), 256);
	rusty_assert_ne(rusty::next_power_of_two((size_t)233), 255);
}

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
