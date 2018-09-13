//
// Created by xiaoc on 2018/9/11.
//

#ifndef KCC_TYPE_H
#define KCC_TYPE_H

#include "ast.h"
namespace kcc{
    PrimitiveType * makePrimitiveType(const std::string& );
    PointerType * makePointerType(Type * );
}
#endif //KCC_TYPE_H
