#include "rusty/error.h"
#include "rusty/result.h"
#include "test.h"

#include <gtest/gtest.h>

TEST_F(Test, Result) {
	auto kind = rusty::io::ErrorKind::NotFound;
	rusty::io::Result<int> result = rusty::io::Error::from(kind);
	ASSERT_TRUE(result.is_err());
	ASSERT_EQ(std::move(result).unwrap_err_unchecked().kind(), kind);

	kind = rusty::io::ErrorKind::PermissionDenied;
	result = rusty::io::Error::from(kind);
	ASSERT_TRUE(result.is_err());
	ASSERT_EQ(std::move(result).unwrap_err().kind(), kind);

	int x = 233;
	result = x;
	ASSERT_TRUE(result.is_ok());
	ASSERT_EQ(std::move(result).unwrap_unchecked(), x);

	result = 114514;
	ASSERT_TRUE(result.is_ok());
	ASSERT_EQ(std::move(result).unwrap(), 114514);

	{
		auto f = [](rusty::io::Result<int> &&result) -> rusty::io::Result<int> {
			return rusty_check_result(std::move(result));
		};
		ASSERT_EQ(f(233).unwrap(), 233);
		auto kind = rusty::io::ErrorKind::AlreadyExists;
		ASSERT_EQ(f(rusty::io::Error::from(kind)).unwrap_err().kind(), kind);
	}

	{
		auto f = [](
			rusty::io::Result<std::unique_ptr<int>> result
		) -> rusty::io::Result<int> {
			// rusty_check_result should be able to move lvalue
			auto x = rusty_check_result(result);
			return *x;
		};
		ASSERT_EQ(f(std::make_unique<int>(233)).unwrap(), 233);
		auto kind = rusty::io::ErrorKind::NotADirectory;
		ASSERT_EQ(f(rusty::io::Error::from(kind)).unwrap_err().kind(), kind);
	}

	{
		auto f = [](
			rusty::io::Result<std::unique_ptr<int>>& result
		) -> rusty::io::Result<int> {
			// rusty_check_result should be able to move lvalue
			auto x = rusty_check_result(result);
			return *x;
		};
		rusty::io::Result<std::unique_ptr<int>> result =
			std::make_unique<int>(233);
		ASSERT_EQ(f(result).unwrap(), 233);
		auto kind = rusty::io::ErrorKind::Other;
		result = rusty::io::Error::from(kind);
		ASSERT_EQ(f(result).unwrap_err().kind(), kind);
	}
}
