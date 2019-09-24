#ifndef KCC_REGISTERS_H
#define KCC_REGISTERS_H


namespace kcc {
    enum Register {
        rax = -128, rdi, rsi, rdx, rcx, rsp, rbp, rbx, r8, r9, r10, r11, r12, r13, r14, r15,
    };
    enum Register32 {
        eax = -128, edi, esi, edx, ecx, esp, ebp, ebx, r8d, r9d, r10d, r11d, r12d, r13d, r14d, r15d,
    };
    enum Register16 {
        ax = -128, di, si, dx, cx, sp, bp, bx, r8w, r9w, r10w, r11w, r12w, r13w, r14w, r15w,
    };
    enum Register8 {
        al = -128, dil, sil, dl, cl, spl, bpl, bl, r8b, r9b, r10b, r11b, r12b, r13b, r14b, r15b,
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
        AssertThrow(false);
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
        AssertThrow(false);
        return "unkown";
    }

    inline const char *toString(Register16 x) {
        switch (x) {
            REG_TO_STR(ax)
            REG_TO_STR(bx)
            REG_TO_STR(cx)
            REG_TO_STR(dx)
            REG_TO_STR(di)
            REG_TO_STR(si)
            REG_TO_STR(r8w)
            REG_TO_STR(r9w)
            REG_TO_STR(r10w)
            REG_TO_STR(r11w)
            REG_TO_STR(r12w)
            REG_TO_STR(r13w)
            REG_TO_STR(r14w)
            REG_TO_STR(r15w)
        }
        AssertThrow(false);
        return "unkown";
    }

    inline const char *toString(Register8 x) {
        switch (x) {
            REG_TO_STR(al)
            REG_TO_STR(bl)
            REG_TO_STR(cl)
            REG_TO_STR(dl)
            REG_TO_STR(dil)
            REG_TO_STR(sil)
            REG_TO_STR(r8b)
            REG_TO_STR(r9b)
            REG_TO_STR(r10b)
            REG_TO_STR(r11b)
            REG_TO_STR(r12b)
            REG_TO_STR(r13b)
            REG_TO_STR(r14b)
            REG_TO_STR(r15b)
        }
        AssertThrow(false);
        return "unkown";
    }

    inline const char *toString(int x, size_t size) {
        if (size == 8) {
            return toString((Register) x);
        } else if (size == 4) {
            return toString((Register32) x);
        }  else if (size == 2) {
            return toString((Register16) x);
        }  else if (size == 1) {
            return toString((Register8) x);
        }
        KCC_NOT_IMPLEMENTED();
    }
}
#endif //KCC_REGISTERS_H
