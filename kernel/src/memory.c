#include "memory.h"
#include "terminal.h"

#define MEMORY_MAP 0xffff80001fe06000

enum bios_memmap_type
{
    MEMORY_TYPE_USABLE = 1,
    MEMORY_TYPE_RESERVED = 2,
    MEMORY_TYPE_ACPI_RECLAMIABLE = 3,
    MEMORY_TYPE_ACPI_NON_VOLATILE = 4,
    MEMORY_TYPE_BAD_AREA = 5,
};

struct bios_memmap_entry
{
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t attrib;
};

void mem_init(void)
{
    static const char* strtype[] = {
        "(unknown)",
        "Usable",
        "Reserved",
        "Reclaimable",
        "Non-Volatile",
        "Bad Area"
    };

    uint32_t len = *(uint32_t*)MEMORY_MAP;
    const struct bios_memmap_entry* entry = (const struct bios_memmap_entry*)(MEMORY_MAP + 8);

    term_write_format(TERM_COLOR_LOG, "Memory map report: %d entries\n", len);
    while (len-- != 0)
    {
        const char* t = entry->type <= 5 ? strtype[entry->type] : strtype[0];
        term_write_format(TERM_CYAN, "    [0x%016lx, 0x%016lx) %s\n",
            entry->base, entry->base + entry->size, t);
        ++entry;
    }
}
