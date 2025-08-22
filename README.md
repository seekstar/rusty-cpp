# rusty-cpp

Write C++ code like Rust!

This project does not simply port code from the Rust standard library to C++. Instead, this project provides C++ utilities with a similar API to the Rust standard library.

This project is highly incomplete, but I will maintain this project in the long term.

## LICENSE

This project is dual licensed under the Apache License v2.0 and the MIT License.

## Guidelines

### Never return rvalue reference

Here we discuss two scenarios where you may want to return a rvalue reference.

#### Chained function call

If you return rvalue reference:

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

However, if `Builder` or any of its members has a non-default move constructor or move assignment operator, the compiler won't optimize out the move construction or move assignment of `Builder`. For example:

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

Therefore, the best practice is to forget chained function calls in Rust and return nothing:

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

If you want to support chained function calls, you should return an lvalue reference:

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

If you insist to support the rusty pattern `builder = std::move(builder).update_id(514);`, you should define the move assignment like this to prevent self-move:

```cpp
struct Builder {
	Builder &operator=(Builder &&other) {
		if (this != &other) { // sometimes won't be optimized out
            a = std::move(other.a);
        }
        return *this;
	}
...
}
```

In some cases, the compiler can optimize out the branch. However, it's observed that sometimes the compiler refuses to inline the move assignment and thus fails to optimize out the branch.

#### Unwrap something

If you return rvalue reference:

```cpp
#include <iostream>
#include <optional>
struct A {
	int id;
	A(int _id) : id(_id) {}
	A(A &&rhs) : id(rhs.id) {
		rhs.id = 0;
	}
	A &operator=(A &&rhs) {
		id = rhs.id;
		rhs.id = 0;
		return *this;
	}
	~A() {
		if (id == 0) {
			return;
		}
		std::cout << id << " destructed" << std::endl;
		id = 0;
	}
};

A &&unwrap(std::optional<A> &&x) {
	return std::move(x.value());
}

int main() {
	A &&a = unwrap(std::make_optional<A>(233));
	std::cout << a.id << std::endl;
	return 0;
}
```

`A &&a` will become a dangling reference.

The correct approach is to return an object with ownership:

```cpp
A unwrap(std::optional<A> &&x) {
```

Then the output will be correct:

```text
233
233 destructed
```
