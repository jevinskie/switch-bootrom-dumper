
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

	return 0;
}
