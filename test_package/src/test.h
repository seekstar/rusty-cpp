#ifndef RUSTY_TEST_H_
#define RUSTY_TEST_H_

#include <gtest/gtest.h>

class Test : public ::testing::Test {
public:
	void SetUp() override {}
	void TearDown() override {}
};

#endif // RUSTY_TEST_H_
