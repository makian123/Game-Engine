#pragma once

#include <cstdint>
#include <string>

namespace engine {
	class UUID {
		struct UUIDImpl {
			uint64_t high;
			uint64_t low;

			bool operator==(const UUIDImpl &other) const {
				return (
					high == other.high &&
					low == other.low
				);
			}
			bool operator!=(const UUIDImpl &other) const {
				return !(*this == other);
			}
		};
		public:
		UUIDImpl id;

		UUID();
		UUID(const std::string &str);

		operator std::string() const;
		bool operator==(const UUID &other) const { return id == other.id; }
		bool operator!=(const UUID &other) const { return id != other.id; }

		UUID &operator=(const UUID &other);
	};
}