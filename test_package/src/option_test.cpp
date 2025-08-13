#include "rusty/option.h"
#include "test.h"

#include <gtest/gtest.h>

TEST_F(Test, Option) {
	ASSERT_EQ(rusty::None, std::nullopt);
	{
		std::optional<int> x = rusty::None;
		ASSERT_EQ(x, std::nullopt);

		x = rusty::Option<int>(233);
		ASSERT_EQ(x, std::optional<int>(233));
	}
	{
		std::optional<rusty::Ref<const int>> x = rusty::None;
		ASSERT_EQ(x, std::nullopt);

		x = rusty::Option<rusty::Ref<const int>>(rusty::ref(233));
		ASSERT_EQ(x, std::optional<rusty::Ref<const int>>(rusty::ref(233)));
	}

	{
		rusty::Option<int> x = std::nullopt;
		ASSERT_EQ(x, rusty::None);

		x = std::optional<int>(233);
		ASSERT_EQ(x, rusty::Option<int>(233));
	}
	{
		rusty::Option<rusty::Ref<const int>> x = std::nullopt;
		ASSERT_EQ(x, rusty::None);

		x = std::optional<rusty::Ref<const int>>(rusty::ref(233));
		ASSERT_EQ(x, rusty::Option<rusty::Ref<const int>>(rusty::ref(233)));
	}

	{
		rusty::Option<int> x = rusty::None;
		ASSERT_TRUE(x.as_ptr() == nullptr);
		ASSERT_TRUE(x.is_none());
		ASSERT_FALSE(x.is_some());

		x = 233;
		ASSERT_TRUE(x.as_ptr() != nullptr);
		ASSERT_EQ(*x.as_ptr(), 233);
		ASSERT_FALSE(x.is_none());
		ASSERT_TRUE(x.is_some());
		ASSERT_EQ(std::move(x).unwrap(), 233);
	}
	{
		rusty::Option<rusty::Ref<int>> x = rusty::None;
		ASSERT_TRUE(x.as_ptr() == nullptr);
		ASSERT_TRUE(x.is_none());
		ASSERT_FALSE(x.is_some());

		int v = 233;
		x = rusty::ref(v);
		ASSERT_TRUE(x.as_ptr() != nullptr);
		ASSERT_EQ(x.as_ptr()->deref(), 233);
		ASSERT_FALSE(x.is_none());
		ASSERT_TRUE(x.is_some());
		ASSERT_EQ(std::move(x).unwrap().deref(), 233);
	}
}
