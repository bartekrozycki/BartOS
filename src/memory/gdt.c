#include "ints.h"
#include "print.h"
#include "gdt.h"
#include "lib.h"

void gdt_load(u32 gdt_ptr);
void gdt_set_gate(u32 n, u32 base, u32 limit, u8 access, u8 flag);

struct _GDT gdt[5];
struct _GDT_Descriptor gdt_ptr;

void init_gdt(void) {

	ASSERT(struct _GDT, 8)
	ASSERT(struct _GDT_Descriptor, 6)

    // Descriptor
	gdt_ptr.limit = sizeof(gdt) - 1;
	gdt_ptr.base  = (u32) &gdt;

	gdt_set_gate(0, 0, 0, 0, 0);                // null entry
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0b10011010, 0b11001111); // kernel code Execute/Read 
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0b10010010, 0b11001111); // kerel data Read/Write 
									//11
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0b11111010, 0b11001111); // user code
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0b11110010, 0b11001111); // user data 
                                                // Covering 4GiB adddress space


	gdt_load((u32) &gdt_ptr);

    print(SERIAL, "[Kernel] GDT Initalized.\n");
}

void gdt_set_gate(u32 n, u32 base, u32 limit, u8 access, u8 flag) {
	gdt[n].limit_low             = (limit & 0x0000FFFF);
	gdt[n].base_low              = (base  & 0x0000FFFF);
	gdt[n].base_middle           = (base  & 0x00FF0000) >> 16;
	gdt[n].access                = access;
	gdt[n].limit_high_and_flags  = (limit & 0x000F0000) >> 16;
	gdt[n].limit_high_and_flags |= (flag & 0xF0);
	gdt[n].base_high             = (base  & 0xFF000000) >> 24;
}