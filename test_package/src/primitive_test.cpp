#include "rusty/primitive.h"
#include "test.h"

#include <gtest/gtest.h>
#include <string>

TEST_F(Test, Primitive) {
	std::string v = "233";
	v = rusty::move(v);
	ASSERT_EQ(v, "233");
	// In comparison, v = std::move(v) will clear v

	ASSERT_EQ(rusty::move(std::string("233")), "233");
}
