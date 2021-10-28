
static inline unsigned char inb(unsigned short port)
{
	unsigned char data;
	asm volatile("in %1, %0" : "=a" (data) : "d" (port));
	return data;
}

