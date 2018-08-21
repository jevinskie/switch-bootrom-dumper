
#include <cassert>
#include <fstream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include <elfio/elfio.hpp>

#include "cxx-prettyprint/prettyprint.hpp"
#include "fmt/format.h"
#include "fmt/ostream.h"

#include "dump.h"
#include "z85.h"

struct merged_dump_rec {
	std::string name;
	std::vector<uint8_t> data;

	friend std::ostream& operator<<(std::ostream &o, const merged_dump_rec &rec) {
		return o << fmt::format("{{ name: '{}' len: 0x{:x} }}",
			rec.name, rec.data.size());
	}

};

int main(int argc, const char **argv) {
	assert(argc == 3);

	std::ifstream in_text{argv[1]};

	std::map<uint64_t, dump_record_t> dump_recs;

	std::string line;
	while (std::getline(in_text, line)) {
		if (line.rfind(DUMP_TAG, 0) == 0) {
			line.erase(0, strlen(DUMP_TAG));
			dump_record_t rec;
			assert(Z85_decode(line.data(), reinterpret_cast<uint8_t *>(&rec), sizeof(rec)));
			rec.addr = ntohll(rec.addr);
			rec.len = ntohl(rec.len);
			fmt::print("rec {}\n", rec);
			dump_recs[rec.addr] = rec;
		}
	}

	fmt::print("dump_recs: {}\n", dump_recs);

	std::map<uint64_t, merged_dump_rec> dump_parts;

	for (const auto &i : dump_recs) {
		bool concatenated = false;
		for (auto &j : dump_parts) {
			if (i.first == j.first + j.second.data.size()) {
				// i is adjacent after j
				j.second.data.insert(j.second.data.end(), i.second.data, i.second.data + i.second.len);
				concatenated = true;
				break;
			}
			if (i.first + i.second.len == j.first) {
				// i is adjacent before j
				j.second.data.insert(j.second.data.begin(), i.second.data, i.second.data + i.second.len);
				concatenated = true;
				break;
			}
		}
		if (concatenated)
			continue;
		dump_parts[i.first] = {.name = i.second.name, .data = std::vector<uint8_t>{i.second.data, i.second.data + i.second.len}};
	}

	fmt::print("dump_parts: {}\n", dump_parts);

	return 0;
}
