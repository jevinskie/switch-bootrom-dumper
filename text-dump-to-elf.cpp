
#include <cassert>
#include <fstream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std::string_literals;

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif

#include <elfio/elfio.hpp>
using namespace ELFIO;

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
	assert(argc == 4);

	std::ifstream in_text{argv[1]};

	unsigned char elf_class;
	Elf_Half elf_machine;

	if ("arm32"s == argv[3]) {
		elf_class = ELFCLASS32;
		elf_machine = EM_ARM;
	} else if ("arm64"s == argv[3]) {
		elf_class = ELFCLASS64;
		elf_machine = EM_AARCH64;
	} else if ("x86-64"s == argv[3]) {
		elf_class = ELFCLASS64;
		elf_machine = EM_X86_64;
	} else {
		assert(!"unknown arch");
	}

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

	elfio writer;
	writer.create(elf_class, ELFDATA2LSB);

	writer.set_os_abi(ELFOSABI_NONE);
	writer.set_type(ET_EXEC);
	writer.set_machine(elf_machine);

	for (const auto &dp : dump_parts) {
		auto sec = writer.sections.add("." + dp.second.name);
		sec->set_type(SHT_PROGBITS);
		sec->set_flags(SHF_ALLOC);
		sec->set_data(reinterpret_cast<const char *>(dp.second.data.data()), dp.second.data.size());
		auto seg = writer.segments.add();
		seg->set_type(PT_LOAD);
		seg->set_virtual_address(dp.first);
		seg->set_physical_address(dp.first);
		seg->set_flags(PF_X | PF_R | PF_W);
		// seg->set_align( 0x1000 );
		seg->add_section_index(sec->get_index(), sec->get_addr_align());
	}

	writer.save(argv[2]);

	return 0;
}
