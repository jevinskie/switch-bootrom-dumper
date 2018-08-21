
#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include <elfio/elfio.hpp>

#include "dump.h"
#include "z85.h"

int main(int argc, const char **argv) {
	assert(argc == 3);

	std::ifstream in_text{argv[1]};

	std::string line;
	while (std::getline(in_text, line)) {
		if (line.rfind(DUMP_TAG, 0) == 0) {
			line.erase(0, strlen(DUMP_TAG));
			dump_record_t rec;
			assert(Z85_decode(line.data(), reinterpret_cast<uint8_t *>(&rec), sizeof(rec)));
			rec.addr = ntohll(rec.addr);
			rec.len = ntohl(rec.len);
			printf("dumping '%s' at %p size %u\n", rec.name, reinterpret_cast<void *>(rec.addr), rec.len);
		}
	}

	return 0;
}
