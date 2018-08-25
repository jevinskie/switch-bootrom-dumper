
#include <cassert>
#include <fstream>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wgnu-string-literal-operator-template"
#endif

#include <elfio/elfio.hpp>
using namespace ELFIO;

#include "cxx-prettyprint/prettyprint.hpp"
#include "fmt/format.h"
#include "fmt/ostream.h"

#include "miniloader/tegra114-miniloader.h"
#include "miniloader/tegra124-miniloader.h"
#include "miniloader/tegra20-miniloader.h"
#include "miniloader/tegra30-miniloader.h"

struct miniloader {
	std::string name;
	std::vector<uint8_t> payload;
	uint32_t entry;
	uint32_t iram_start;
	uint32_t iram_size;
	uint32_t irom_start;
	uint32_t irom_size;
};

struct miniloader miniloaders[] {
	{
		.name = "tegra114",
		.payload = {miniloader_tegra114, miniloader_tegra114 + sizeof(miniloader_tegra114)},
		.entry = TEGRA114_MINILOADER_ENTRY,
		// tegra 2 dummy values
		.iram_start = 0x40000000,
		.iram_size = 256 * 1024,
		.irom_start = 0xFFF00000,
		.irom_size = 48 * 1024,
	},
	{
		.name = "tegra124",
		.payload = {miniloader_tegra124, miniloader_tegra124 + sizeof(miniloader_tegra124)},
		.entry = TEGRA124_MINILOADER_ENTRY,
		// tegra 2 dummy values
		.iram_start = 0x40000000,
		.iram_size = 256 * 1024,
		.irom_start = 0xFFF00000,
		.irom_size = 48 * 1024,
	},
	{
		.name = "tegra20",
		.payload = {miniloader_tegra20, miniloader_tegra20 + sizeof(miniloader_tegra20)},
		.entry = TEGRA114_MINILOADER_ENTRY,
		.iram_start = 0x40000000,
		.iram_size = 256 * 1024,
		.irom_start = 0xFFF00000,
		.irom_size = 48 * 1024,
	},
	{
		.name = "tegra30",
		.payload = {miniloader_tegra30, miniloader_tegra30 + sizeof(miniloader_tegra30)},
		.entry = TEGRA30_MINILOADER_ENTRY,
		.iram_start = 0x40000000,
		.iram_size = 256 * 1024,
		.irom_start = 0xFFF00000,
		.irom_size = 48 * 1024,
	},
};

int main(void) {
	for (const auto &loader : miniloaders) {
		elfio writer;
		writer.create(ELFCLASS32, ELFDATA2LSB);

		writer.set_os_abi(ELFOSABI_NONE);
		writer.set_type(ET_EXEC);
		writer.set_machine(EM_ARM);
		writer.set_entry(loader.entry);

		auto iram = std::vector<uint8_t>(loader.iram_size, 0);
		memcpy(iram.data() + loader.entry - loader.iram_start, loader.payload.data(), loader.payload.size());

		auto iram_sec = writer.sections.add(".iram");
		iram_sec->set_type(SHT_PROGBITS);
		iram_sec->set_flags(SHF_ALLOC);
		iram_sec->set_data(reinterpret_cast<const char *>(iram.data()), iram.size());
		auto iram_seg = writer.segments.add();
		iram_seg->set_type(PT_LOAD);
		iram_seg->set_virtual_address(loader.iram_start);
		iram_seg->set_physical_address(loader.iram_start);
		iram_seg->set_flags(PF_X | PF_R | PF_W);
		iram_seg->add_section_index(iram_sec->get_index(), iram_sec->get_addr_align());


		auto irom = std::vector<uint8_t>(loader.irom_size, 0);

		auto irom_sec = writer.sections.add(".irom");
		irom_sec->set_type(SHT_PROGBITS);
		irom_sec->set_flags(SHF_ALLOC);
		irom_sec->set_data(reinterpret_cast<const char *>(irom.data()), irom.size());
		auto irom_seg = writer.segments.add();
		irom_seg->set_type(PT_LOAD);
		irom_seg->set_virtual_address(loader.irom_start);
		irom_seg->set_physical_address(loader.irom_start);
		irom_seg->set_flags(PF_X | PF_R);
		irom_seg->add_section_index(irom_sec->get_index(), irom_sec->get_addr_align());


		writer.save(loader.name + "-miniloader.elf");
	}

	return 0;
}
