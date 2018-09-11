//
// Created by xiaoc on 2018/9/11.
//

#include "type.h"

kcc::PrimitiveType *kcc::makePrimitiveType(const std::string& s) {
    return new PrimitiveType(Token(Token::Type::Identifier,s,-1,-1));
}
