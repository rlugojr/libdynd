//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

/**
 * Analysis of compiled kernel disassembly.
 *
 * Windows Visual Studio Instructions:
 *   1. Open the 'Properties' page for 'dynamicndarray'.
 *   2. Navigate to 'Configuration Properties' -> 'C/C++' -> 'Output Files'.
 *   3. Change the 'Assembler Output' item to say 'Assembly With Machine Code (/FAc)'.
 *   4. Change the 'ASM List Location' to the 'inherit from parent or project defaults'.
 */

// Set this to 1 when needed to analyze compiled kernel output, 0 otherwise
#define ENABLE_DISASSEMBLY_ANALYSIS 1

#if ENABLE_DISASSEMBLY_ANALYSIS
#include <stdint.h>

#include <dnd/auxiliary_data.hpp>

using namespace std;
using namespace dnd;

extern "C" void unary__int_int__general_kernel__disassembly_analysis(char *dst, intptr_t dst_stride, const char *src, intptr_t src_stride,
                    intptr_t count, const AuxDataBase *auxdata)
{
    typedef int (*cdecl_func_ptr_t)(int);
    cdecl_func_ptr_t kfunc = reinterpret_cast<cdecl_func_ptr_t>(get_raw_auxiliary_data(auxdata)&(~1));
    for (intptr_t i = 0; i < count; ++i) {
        *(int *)dst = kfunc(*(const int *)src);

        dst += dst_stride;
        src += src_stride;
    }
}

#endif // ENABLE_DISASSEMBLY_ANALYSIS