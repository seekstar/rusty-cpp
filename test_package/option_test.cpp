#include "rusty/option.h"
#include "test.h"

#include <gtest/gtest.h>

TEST_F(Test, Option) {
	{
		rusty::Option<int> x = rusty::None;
		ASSERT_TRUE(x.as_ref().is_none());
		ASSERT_TRUE(x.is_none());
		ASSERT_FALSE(x.is_some());

		x = 233;
		ASSERT_TRUE(x.as_ref().is_some());
		ASSERT_EQ(x.as_ref().unwrap_unchecked().deref(), 233);
		ASSERT_EQ(x.as_ref().unwrap().deref(), 233);
		ASSERT_FALSE(x.is_none());
		ASSERT_TRUE(x.is_some());
		ASSERT_EQ(std::move(x).unwrap(), 233);
	}
	{
		rusty::Option<rusty::Ref<int>> x = rusty::None;
		ASSERT_TRUE(x.as_ref().is_none());
		ASSERT_TRUE(x.is_none());
		ASSERT_FALSE(x.is_some());

		int v = 233;
		x = rusty::ref(v);
		ASSERT_TRUE(x.as_ref().is_some());
		ASSERT_EQ(x.as_ref().unwrap_unchecked().deref().deref(), 233);
		ASSERT_EQ(x.as_ref().unwrap().deref().deref(), 233);
		ASSERT_FALSE(x.is_none());
		ASSERT_TRUE(x.is_some());
		ASSERT_EQ(std::move(x).unwrap().deref(), 233);
	}
}
