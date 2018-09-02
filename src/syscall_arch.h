// https://git.musl-libc.org/cgit/musl/tree/arch/arm/syscall_arch.h
// Check LICENSE-musl

#define R7_OPERAND "r"(r7)

#define __ASM____R7__
#define __asm_syscall(...) do { \
    __asm__ __volatile__ ( "mov %1,r7 ; mov r7,%2 ; svc 0 ; mov r7,%1" \
    : "=r"(r0), "=&r"((int){0}) : __VA_ARGS__ : "memory"); \
    return r0; \
    } while (0)

static inline int __syscall0(int n) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0");
    __asm_syscall(R7_OPERAND);
}

static inline int __syscall1(int n, int a) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0") = a;
    __asm_syscall(R7_OPERAND, "0"(r0));
}

static inline int __syscall2(int n, int a, int b) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0") = a;
    register int r1 __asm__("r1") = b;
    __asm_syscall(R7_OPERAND, "0"(r0), "r"(r1));
}

static inline int __syscall3(int n, int a, int b, int c) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0") = a;
    register int r1 __asm__("r1") = b;
    register int r2 __asm__("r2") = c;
    __asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2));
}

static inline int __syscall4(int n, int a, int b, int c, int d) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0") = a;
    register int r1 __asm__("r1") = b;
    register int r2 __asm__("r2") = c;
    register int r3 __asm__("r3") = d;
    __asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3));
}

static inline int __syscall5(int n, int a, int b, int c, int d, int e) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0") = a;
    register int r1 __asm__("r1") = b;
    register int r2 __asm__("r2") = c;
    register int r3 __asm__("r3") = d;
    register int r4 __asm__("r4") = e;
    __asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4));
}

static inline int __syscall6(int n, int a, int b, int c, int d, int e, int f) {
    register int r7 __ASM____R7__ = n;
    register int r0 __asm__("r0") = a;
    register int r1 __asm__("r1") = b;
    register int r2 __asm__("r2") = c;
    register int r3 __asm__("r3") = d;
    register int r4 __asm__("r4") = e;
    register int r5 __asm__("r5") = f;
    __asm_syscall(R7_OPERAND, "0"(r0), "r"(r1), "r"(r2), "r"(r3), "r"(r4), "r"(r5));
}
