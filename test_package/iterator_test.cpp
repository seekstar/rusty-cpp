#include "rusty/iter/iterator.h"
#include "test.h"

#include <gtest/gtest.h>

TEST_F(Test, IteratorSimple) {
	std::vector<int> a{1, 3, 8, 2};
	ASSERT_TRUE(rusty::MakeIter(a.data(), a.data()).next().is_none());

	std::vector<rusty::Ref<const int>> b;
	rusty::collect_into(rusty::MakeIter(a.data(), a.data() + a.size()), b);
	ASSERT_EQ(a.size(), b.size());
	for (size_t i = 0; i < a.size(); ++i) {
		ASSERT_EQ(a[i], b[i].deref());
	}
}
