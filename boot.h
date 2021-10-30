#include <linux/types.h>



extern struct setup_header hdr;
extern struct boot_params boot_params;

#define cpu_relax asm volatile("rep; nop")

static inline void outb(u8 v, u16 port)
{
	asm volatile("outb %0, %1" : : "a" (v), "dN" (port));
}

static inline u8 inb(u16 port)
{
	u8 v;
	asm volatile("inb %1, %0" : "=a" (v) : "dN" (port));
	return v;
}

static inline void outw(u16 v, u16 port)
{
        asm volatile("outw %0,%1" : : "a" (v), "dN" (port));
}
static inline u16 inw(u16 port)
{
        u16 v;
        asm volatile("inw %1,%0" : "=a" (v) : "dN" (port));
        return v;
}

static inline void outl(u32 v, u16 port)
{
        asm volatile("outl %0,%1" : : "a" (v), "dN" (port));
}
static inline u32 inl(u32 port)
{
        u32 v;
        asm volatile("inl %1,%0" : "=a" (v) : "dN" (port));
        return v;
}

static inline void io_delay(void)
{
        const u16 DELAY_PORT = 0x80;
        asm volatile("outb %%al,%0" : : "dN" (DELAY_PORT));
}

static inline u16 ds(void)
{
        u16 seg;
        asm("movw %%ds,%0" : "=rm" (seg));
        return seg;
}

static inline void set_fs(u16 seg)
{
        asm volatile("movw %0,%%fs" : : "rm" (seg));
}
static inline u16 fs(void)
{
        u16 seg;
        asm volatile("movw %%fs,%0" : "=rm" (seg));
        return seg;
}

static inline void set_gs(u16 seg)
{
        asm volatile("movw %0,%%gs" : : "rm" (seg));
}
static inline u16 gs(void)
{
        u16 seg;
        asm volatile("movw %%gs,%0" : "=rm" (seg));
        return seg;
}

typedef unsigned int addr_t;

static inline u8 rdfs8(addr_t addr)
{
        u8 v;
        asm volatile("movb %%fs:%1,%0" : "=q" (v) : "m" (*(u8 *)addr));
        return v;
}
static inline u16 rdfs16(addr_t addr)
{
        u16 v;
        asm volatile("movw %%fs:%1,%0" : "=r" (v) : "m" (*(u16 *)addr));
        return v;
}

static inline u16 rdfs16(addr_t addr)
{
        u16 v;
        asm volatile("movw %%fs:%1,%0" : "=r" (v) : "m" (*(u16 *)addr));
        return v;
}
static inline u32 rdfs32(addr_t addr)
{
        u32 v;
        asm volatile("movl %%fs:%1,%0" : "=r" (v) : "m" (*(u32 *)addr));
        return v;
}

static inline void wrfs8(u8 v, addr_t addr)
{
        asm volatile("movb %1,%%fs:%0" : "+m" (*(u8 *)addr) : "qi" (v));
}
static inline void wrfs16(u16 v, addr_t addr)
{
        asm volatile("movw %1,%%fs:%0" : "+m" (*(u16 *)addr) : "ri" (v));
}

static inline void wrfs32(u32 v, addr_t addr)
{
        asm volatile("movl %1,%%fs:%0" : "+m" (*(u32 *)addr) : "ri" (v));
}

static inline u8 rdgs8(addr_t addr)
{
        u8 v;
        asm volatile("movb %%gs:%1,%0" : "=q" (v) : "m" (*(u8 *)addr));
        return v;
}

static inline u16 rdgs16(addr_t addr)
{
        u16 v;
        asm volatile("movw %%gs:%1,%0" : "=r" (v) : "m" (*(u16 *)addr));
        return v;
}
static inline u32 rdgs32(addr_t addr)
{
        u32 v;
        asm volatile("movl %%gs:%1,%0" : "=r" (v) : "m" (*(u32 *)addr));
        return v;
}

static inline void wrgs8(u8 v, addr_t addr)
{
        asm volatile("movb %1,%%gs:%0" : "+m" (*(u8 *)addr) : "qi" (v));
}

static inline void wrgs16(u16 v, addr_t addr)
{
        asm volatile("movw %1,%%gs:%0" : "+m" (*(u16 *)addr) : "ri" (v));
}
static inline void wrgs32(u32 v, addr_t addr)
{
        asm volatile("movl %1,%%gs:%0" : "+m" (*(u32 *)addr) : "ri" (v));
}

static inline int memcmp(const void *s1, const void *s2, size_t len)
{
        u8 diff;
        asm("repe; cmpsb; setnz %0"
            : "=qm" (diff), "+D" (s1), "+S" (s2), "+c" (len));
        return diff;
}

static inline int memcmp_fs(const void *s1, addr_t s2, size_t len)
{
        u8 diff;
        asm volatile("fs; repe; cmpsb; setnz %0"
                     : "=qm" (diff), "+D" (s1), "+S" (s2), "+c" (len));
        return diff;
}

static inline int memcmp_gs(const void *s1, addr_t s2, size_t len)
{
        u8 diff;
        asm volatile("gs; repe; cmpsb; setnz %0"
                     : "=qm" (diff), "+D" (s1), "+S" (s2), "+c" (len));
        return diff;
}

# heap stuff

extern char _end[];
extern char *HEAP;
extern char *heap_end;
#define RESET_HEAP() ((void *)( HEAP = _end ))
static inline char *__get_heap(size_t s, size_t a, size_t n)
{
        char *tmp;

        HEAP = (char *)(((size_t)HEAP+(a-1)) & ~(a-1));
        tmp = HEAP;
        HEAP += s*n;
        return tmp;
}
#define GET_HEAP(type, n) \
        ((type *)__get_heap(sizeof(type),__alignof__(type),(n)))

static inline bool heap_free(size_t n)
{
        return (int)(heap_end-HEAP) >= (int)n;
}


