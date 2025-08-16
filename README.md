# rusty-cpp

Write C++ code like Rust!

This project does not simply port code from Rust standard library to C++. Instead, this project provides C++ utilities with similar API in Rust standard library.

This project is highly incomplete, but I will maintain this project in the long term.

## LICENSE

This project is dual licensed under the Apache License v2.0 and the MIT License.

## Guidelines

### Never return rvalue reference

This is a negative example of returning rvalue reference:

```cpp
#include <iostream>
struct Builder {
	std::string value;
	Builder &&with_value(std::string v) && {
		value = std::move(v);
		return std::move(*this);
	}
};
int main() {
	Builder builder;
	builder = std::move(builder).with_value("2333");
	std::cout << builder.value.size() << std::endl;
	return 0;
}
```

You will find that it prints `0`. This is because when assigning the rvalue reference of `builder` to itself, it will first destruct itself and then assign the destructed itself to itself. As a result, it's basically destructed.

A better practice is returning an object with ownership:

```cpp
	Builder with_value(std::string v) && {
```

Then it will print `4` correctly. However, `builder = std::move(builder)` will still destruct `builder` (there will be a self-move warning if you compile with `-Wself-move`). The root cause is that `std::move` returns rvalue reference. rusty-cpp provides `rusty::move` that returns an object with ownership to solve this problem:

```cpp
#include <iostream>
#include <rusty/primitive.h>
int main() {
	std::string v = "2333";
	v = rusty::move(v);
	// Prints "4"
	std::cout << v.size() << std::endl;
	return 0;
}
```
