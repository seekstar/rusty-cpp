#include "rusty/collections/min_heap.h"
#include "test.h"

#include <gtest/gtest.h>
#include <queue>

namespace {
void pop_all(rusty::MinHeap<int> &heap, std::priority_queue<int, std::vector<int>, std::greater<int>> &q) {
	for (;;) {
		auto ret = heap.pop();
		if (!ret.has_value()) {
			break;
		}
		ASSERT_TRUE(!q.empty());
		ASSERT_EQ(ret.value(), q.top());
		q.pop();
	}
	ASSERT_TRUE(q.empty());
}
}

TEST_F(Test, MinHeapSimple) {
	rusty::MinHeap<int> heap;
	ASSERT_TRUE(heap.is_empty());
	ASSERT_TRUE(heap.peek() == nullptr);
	ASSERT_FALSE(heap.peek_mut().has_value());

	std::vector<int> data{3, 6, 4, 1, 7};
	std::priority_queue<int, std::vector<int>, std::greater<int>> q(data.begin(), data.end());
	heap = rusty::MinHeap<int>(data);
	ASSERT_NO_FATAL_FAILURE(pop_all(heap, q));

	q = {data.begin(), data.end()};
	heap = rusty::MinHeap<int>(data);
	for (int x : data) {
		auto res = heap.peek_mut();
		ASSERT_TRUE(res.has_value());
		auto &peeked = res.value();
		ASSERT_EQ(*peeked, q.top());
		*peeked = x;
		q.pop();
		q.push(x);
	}
	ASSERT_NO_FATAL_FAILURE(pop_all(heap, q));
}
