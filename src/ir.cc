//
// Created by xiaoc on 2018/9/17.
//

#include "ir.h"
#include "format.h"

std::string kcc::IRNode::dump() const {
    switch(op){
        case Opcode::iconst:
            return format("t{} = ${}",a,b);
        case Opcode::fconst:
            return format("t{} = ${}",a,fval);
        case Opcode ::cvtf2i:
            return format("t{} = (int)t{}",a,b);
        case Opcode::cvti2f:
            return format("t{} = (double)t{}",a,b);
        case Opcode ::iadd:
            return format("t{} = t{} + t{}",a,b,c);
        case Opcode::isub:
            return format("t{} = t{} - t{}",a,b,c);
        case Opcode::imul:
            return format("t{} = t{} * t{}",a,b,c);
        case Opcode::idiv:
            return format("t{} = t{} / t{}",a,b,c);
        case Opcode::fadd:
            return format("t{} = t{} +. t{}",a,b,c);
        case Opcode::fsub:
            return format("t{} = t{} -. t{}",a,b,c);
        case Opcode::fmul:
            return format("t{} = t{} *. t{}",a,b,c);
        case Opcode::fdiv:
            return format("t{} = t{} /. t{}",a,b,c);
        case Opcode::jmp:
            return format("jmp ",a);
        case Opcode::branch:
            return format("branch t{}, %true. {}, %false. {}",a,b,c);
        case Opcode ::load:
            return format("t{} = [{}]",a,b);
        case Opcode::store:
            return format("[{}] = t{}",b,a);
        default:
            return format("unknown opcode {}",(int)op);
    }
}
