#ifndef UMPORT_INCLUDED_SVC_H
#define UMPORT_INCLUDED_SVC_H

#define SVC_FUNCTION_START unsigned int R[4] = {};
#define R0 R[0]
#define R1 R[1]
#define R2 R[2]
#define R3 R[3]

#define SVC4(code, a, b, c, d) __asm__ volatile ( \
    "mov r0, %0\n\t" \
    "mov r1, %1\n\t" \
    "mov r2, %2\n\t" \
    "mov r3, %3\n\t" \
    "svc %[imm]\n\t" \
    "str r0, %4\n\t" \
    "str r1, %5\n\t" \
    "str r2, %6\n\t" \
    "str r3, %7\n\t" \
    :"+r" (a), \
     "+r" (b), \
     "+r" (c), \
     "+r" (d) \
    :"m" (R[0]), \
     "m" (R[1]), \
     "m" (R[2]), \
     "m" (R[3]), \
     [Z] "I" (0), \
     [imm] "I" (code) \
    :"r0", "r1", "r2", "r3" \
);

#define SVC3(code, a, b, c) __asm__ volatile ( \
    "mov r0, %0\n\t" \
    "mov r1, %1\n\t" \
    "mov r2, %2\n\t" \
    "mov r3, %[Z]\n\t" \
    "svc %[imm]\n\t" \
    "str r0, %3\n\t" \
    "str r1, %4\n\t" \
    "str r2, %5\n\t" \
    "str r3, %6\n\t" \
    :"+r" (a), \
     "+r" (b), \
     "+r" (c), \
    :"m" (R[0]), \
     "m" (R[1]), \
     "m" (R[2]), \
     "m" (R[3]), \
     [Z] "I" (0), \
     [imm] "I" (code) \
    :"r0", "r1", "r2", "r3" \
);

#define SVC2(code, a, b) __asm__ volatile ( \
    "mov r0, %0\n\t" \
    "mov r1, %1\n\t" \
    "mov r2, %[Z]\n\t" \
    "mov r3, r2\n\t" \
    "svc %[imm]\n\t" \
    "str r0, %2\n\t" \
    "str r1, %3\n\t" \
    "str r2, %4\n\t" \
    "str r3, %5\n\t" \
    :"+r" (a), \
     "+r" (b) \
    :"m" (R[0]), \
     "m" (R[1]), \
     "m" (R[2]), \
     "m" (R[3]), \
     [Z] "I" (0), \
     [imm] "I" (code) \
    :"r0", "r1", "r2", "r3" \
);

#define SVC1(code, a) __asm__ volatile ( \
    "mov r0, %0\n\t" \
    "mov r1, %[Z]\n\t" \
    "mov r2, r1\n\t" \
    "mov r3, r1\n\t" \
    "svc %[imm]\n\t" \
    "str r0, %1\n\t" \
    "str r1, %2\n\t" \
    "str r2, %3\n\t" \
    "str r3, %4\n\t" \
    :"+r" (a) \
    :"m" (R[0]), \
     "m" (R[1]), \
     "m" (R[2]), \
     "m" (R[3]), \
     [Z] "I" (0), \
     [imm] "I" (code) \
    :"r0", "r1", "r2", "r3" \
);

#define SVC0(code) __asm__ volatile ( \
    "mov r0, %[Z]\n\t" \
    "mov r1, r0\n\t" \
    "mov r2, r0\n\t" \
    "mov r3, r0\n\t" \
    "svc %[imm]\n\t" \
    "str r0, %0\n\t" \
    "str r1, %1\n\t" \
    "str r2, %2\n\t" \
    "str r3, %3\n\t" \
    : \
    :"m" (R[0]), \
     "m" (R[1]), \
     "m" (R[2]), \
     "m" (R[3]), \
     [Z] "I" (0), \
     [imm] "I" (code) \
    :"r0", "r1", "r2", "r3" \
);

#endif