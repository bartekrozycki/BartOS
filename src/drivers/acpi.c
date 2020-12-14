#include "acpi.h"
#include "print.h"

u32 * acpi_address = 0x0;

void init_acpi(void)
{
    if (!acpi_address) return;

    print(TERMINAL, "Found ACPI at %p... maybe later ll be used\n", acpi_address);

}