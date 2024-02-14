#include <utils/uuidgenerator.h>

#include <random>
#include <iomanip>
#include <sstream>

namespace engine {
	static std::random_device              rd;
	static std::mt19937_64                 gen(rd());
	static std::uniform_int_distribution<uint64_t> dist;

	UUID::UUID() {
		id.high = dist(gen);
		id.low = dist(gen);
	}
	UUID::UUID(const std::string &str) {
		std::stringstream ss(str.substr(0, 16));

		ss >> std::hex >> id.high;
		ss = std::stringstream(str.substr(16));
		ss >> std::hex >> id.low;
	}

	UUID &UUID::operator=(const UUID &other) {
		id = other.id;
		return *this;
	}
	UUID::operator std::string() const {
		std::stringstream ss;

		ss << 
			std::setw(16) << std::setfill('0') << std::hex << id.high << 
			std::setw(16) << std::setfill('0') << std::hex << id.low;

		return ss.str();
	}
}