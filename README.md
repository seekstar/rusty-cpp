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

You will find that it prints `0`. It's because when assigning the rvalue reference of `builder` to itself, it will first destruct itself and then assign the destructed itself to itself. As a result, it's basically destructed.

A better practice is returning an object with ownership:

```cpp
	Builder with_value(std::string v) && {
```

Then it will print `4` correctly.

However, if `Builder` or any of its members has non-default move constructor or move assignment operator, the compiler won't optimize out the move construction or move assignment of `Builder`. For example:

```cpp
#include <iostream>
struct A {
	int id;
	A(int _id) : id(_id) {}
	A(A &&rhs) {
		std::cout << "Moving " << rhs.id << std::endl;
		id = rhs.id;
		rhs.id = 0;
	}
	A &operator=(A &&rhs) {
		std::cout << "Move assign " << rhs.id << std::endl;
		id = rhs.id;
		rhs.id = 0;
		return *this;
	}
};
struct Builder {
	A a;
	Builder(int id) : a(id) {}
	Builder update_id(int new_id) && {
		a.id = new_id;
		return std::move(*this);
	}
};
int main() {
	Builder builder(114);
	builder = std::move(builder).update_id(514);
	std::cout << builder.a.id << std::endl;
	return 0;
}
```

Output:

```text
Moving 514
Move assign 514
514
```

Therefore, the best practice is forgetting chained function call in Rust and returning nothing:

```cpp
struct Builder {
	A a;
	Builder(int id) : a(id) {}
	void update_id(int new_id) {
		a.id = new_id;
	}
};
int main() {
	Builder builder(114);
	builder.update_id(514);
	builder.update_id(233);
	std::cout << builder.a.id << std::endl;
	return 0;
}
```

If you really want to support chained function call, you should return lvalue reference:

```cpp
struct Builder {
	A a;
	Builder(int id) : a(id) {}
	Builder &update_id(int new_id) {
		a.id = new_id;
		return *this;
	}
};
int main() {
	Builder builder(114);
	builder
		.update_id(514)
		.update_id(233);
	std::cout << builder.a.id << std::endl;
	return 0;
}
```
