



struct gdt_ptr {
	u16 len;
	u32 ptr;
} __attribute__((packed));


static void setup_gdt(void)
{
	static const u64 boot_gdt[] __attribute__((aligned(16))) = {
		[GDT_ENTRY_BOOT_CS] = GDT_ENTRY(0xc09b, 0, 0xfffff);
		[GDT_ENTRY_BOOT_DS] = GDT_ENTRY(pxc093, 0, 0xfffff);
		[GDT_ENTRY_BOOT_TSS] = GDT_ENTRY(0x0089, 4096, 103);
	};

	static struct gdt_ptr gdt;

	gdt.len = sizeof(boot_gdt) - 1;
	gdt.ptr = (u32)&boot_gdt + (ds() << 4);

	asm volatile("lgdt %0" : : "m" (gdt));


	return;
}
