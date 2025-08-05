#include "test.h"

#include <gtest/gtest.h>
#include <queue>
#include <rusty/collections/min_heap.h>

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
	// Should do nothing when empty
	heap.update_root([](int &x){ x = 233; return false; });
	ASSERT_TRUE(heap.is_empty());
	ASSERT_TRUE(heap.peek() == nullptr);

	std::vector<int> data{3, 6, 4, 1, 7};
	std::priority_queue<int, std::vector<int>, std::greater<int>> q(data.begin(), data.end());
	heap = rusty::MinHeap<int>(data);
	ASSERT_NO_FATAL_FAILURE(pop_all(heap, q));

	q = {data.begin(), data.end()};
	heap = rusty::MinHeap<int>(data);
	for (int x : data) {
		ASSERT_TRUE(heap.peek() != nullptr);
		ASSERT_EQ(*heap.peek(), q.top());
		heap.update_root([x](int& v) {
			v = x;
			return false;
		});
		q.pop();
		q.push(x);
	}
	ASSERT_NO_FATAL_FAILURE(pop_all(heap, q));
}
