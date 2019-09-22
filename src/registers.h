#ifndef KCC_REGISTERS_H
#define KCC_REGISTERS_H


namespace kcc {
    enum Register {
        rax = -128, rdi, rsi, rdx, rcx, rsp, rbp, rbx, r8, r9, r10, r11, r12, r13, r14, r15,
    };
    enum Register32 {
        eax = -128, edi, esi, edx, ecx, esp, ebp, ebx, r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d,
    };
    enum XmmRegister {
        xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7
    };
#define REG_TO_STR(x) case x: return #x;

    inline const char *toString(Register x) {
        switch (x) {
            REG_TO_STR(rax)
            REG_TO_STR(rbx)
            REG_TO_STR(rcx)
            REG_TO_STR(rdx)
            REG_TO_STR(rdi)
            REG_TO_STR(rsi)
            REG_TO_STR(r8)
            REG_TO_STR(r9)
            REG_TO_STR(r10)
            REG_TO_STR(r11)
            REG_TO_STR(r12)
            REG_TO_STR(r13)
            REG_TO_STR(r14)
            REG_TO_STR(r15)
        }
        return "unkown";
    }

    inline const char *toString(Register32 x) {
        switch (x) {
            REG_TO_STR(eax)
            REG_TO_STR(ebx)
            REG_TO_STR(ecx)
            REG_TO_STR(edx)
            REG_TO_STR(edi)
            REG_TO_STR(esi)
            REG_TO_STR(r8d)
            REG_TO_STR(r9d)
            REG_TO_STR(r10d)
            REG_TO_STR(r11d)
            REG_TO_STR(r12d)
            REG_TO_STR(r13d)
            REG_TO_STR(r14d)
            REG_TO_STR(r15d)
        }
        return "unkown";
    }
}
#endif //KCC_REGISTERS_H
