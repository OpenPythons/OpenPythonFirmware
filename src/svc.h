#ifndef OPENPIE_INCLUDED_SVC_H
#define OPENPIE_INCLUDED_SVC_H

#define SVC_FUNCTION_START volatile void *R[4] = {};
#define R0 R[0]
#define R1 R[1]
#define R2 R[2]
#define R3 R[3]
#define SVC_CALL(code) _SVC_CALL(code, &R, R)


#define _SVC_CALL(code, arr, R) __asm__ ( \
    "push {r0-r4}\n\t" \
    "mov r4, %[input]\n\t" \
    "ldr r0, [r4, #0]\n\t" \
    "ldr r1, [r4, #4]\n\t" \
    "ldr r2, [r4, #8]\n\t" \
    "ldr r3, [r4, #12]\n\t" \
    "svc %[imm]\n\t" \
    "str r0, [r4, #0]\n\t" \
    "str r1, [r4, #4]\n\t" \
    "str r2, [r4, #8]\n\t" \
    "str r3, [r4, #12]\n\t" \
    "pop {r0-r4}\n\t" \
    : \
    :[input] "r" (arr), \
     [Z] "I" (0), \
     [imm] "I" (code) \
    : "memory" \
)

#endif