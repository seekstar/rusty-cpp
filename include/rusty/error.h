#ifndef RUSTY_ERROR_H_
#define RUSTY_ERROR_H_

#include <optional>
#include <ostream>

namespace rusty {

namespace error {

class Error {
public:
	virtual void print(std::ostream &) const = 0;
};
inline std::ostream &operator<<(std::ostream &out, const Error &e) {
	e.print(out);
	return out;
}

}  // namespace error

namespace io {

using RawOsError = int;

enum class ErrorKind {
	NotFound,
	PermissionDenied,
	AlreadyExists,
	Other,
};

class Error : public error::Error {
public:
	Error(const Error&) = delete;
	Error& operator=(const Error&) = delete;
	Error(Error&& e) : kind_(e.kind_), code_(e.code_) {}
	Error& operator=(Error&& e) {
		kind_ = e.kind_;
		code_ = e.code_;
		return *this;
	}
	static Error from(ErrorKind kind) {
		return Error(kind);
	}
	static Error from_raw_os_error(RawOsError code) {
		return Error(code);
	}
	ErrorKind kind() const {
		return kind_;
	}
	std::optional<RawOsError> raw_os_error() const {
		return code_;
	}

	void print(std::ostream &out) const override {
		if (code_.has_value()) {
			RawOsError code = code_.value();
			switch (code) {
			case ENOENT:
				out << "No such file or directory";
				break;
			case EPERM:
				out << "Operation not permitted";
				break;
			case EACCES:
				out << "Permission denied";
				break;
			case EEXIST:
				out << "File exists";
				break;
			default:
				out << "Unrecoginized OS error";
				break;
			}
			out << " (os error " << code << ")";
		}
		switch (kind()) {
		case ErrorKind::NotFound:
			out << "entity not found";
			break;
		case ErrorKind::PermissionDenied:
			out << "permission denied";
			break;
		case ErrorKind::AlreadyExists:
			out << "entity already exists";
			break;
		case ErrorKind::Other:
			out << "other error";
			break;
		}
	}

private:
	Error(ErrorKind kind) : kind_(kind) {}
	Error(RawOsError code) : code_(code) {
		switch (code) {
		case ENOENT:
			kind_ = ErrorKind::NotFound;
			break;
		case EPERM:
		case EACCES:
			kind_ = ErrorKind::PermissionDenied;
			break;
		case EEXIST:
			kind_ = ErrorKind::AlreadyExists;
			break;
		default:
			kind_ = ErrorKind::Other;
			break;
		}
	}

	ErrorKind kind_;
	std::optional<RawOsError> code_;
};

}  // namespace io

}  // namespace rusty

#endif // RUSTY_ERROR_H_
