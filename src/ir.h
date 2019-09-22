
#ifndef KCC_IR_H
#define KCC_IR_H

namespace kcc {
    namespace IR {
        static constexpr unsigned int ARITH_BIT = 1u << 6;
        static constexpr unsigned int ARITH_INT_BIT = 1u << 7;
        enum Opcode {
            move = 0,
            phi,
            load,
            store,
            jmp,
            jz,
            loadi,
            loadf,
            add = ARITH_BIT | 1,
            sub,
            mul,
            div,
            shl,
            shr,
            bit_and,
            bit_or,
            bit_xor,
        };

        inline bool isArith(unsigned int opcode) {
            return opcode & ARITH_BIT;
        }

        inline bool isArithInt(unsigned int opcode) {
            return isArith(opcode) && (opcode & ARITH_BIT);
        }

        struct Var {
            std::string name;
            int version = -1;

            Var(const std::string &name, int version) : name(name), version(version) {}
        };

        struct Instruction {
            unsigned int opcode;
            Var src;
            Var dst;
            union {
                int64_t immInt;
                double immFloat;
            };
            std::vector<Var> _phiElements;
        };
    }
}


#endif //KCC_IR_H
