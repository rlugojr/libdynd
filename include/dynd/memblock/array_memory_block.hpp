//
// Copyright (C) 2011-13 Mark Wiebe, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__NDOBJECT_MEMORY_BLOCK_HPP_
#define _DYND__NDOBJECT_MEMORY_BLOCK_HPP_

#include <iostream>
#include <string>

#include <dynd/type.hpp>
#include <dynd/memblock/memory_block.hpp>

namespace dynd {

/**
 * This structure is the start of any ndobject metadata. The
 * metadata after this structure is determined by the m_type
 * object.
 */
struct array_preamble {
    memory_block_data m_memblockdata;
    /**
     * m_type is overloaded - for builtin scalar types, it
     * simply contains the type id. If (m_type&~builtin_type_id_mask)
     * is 0, its a builtin type.
     */
    const base_type *m_type;
    char *m_data_pointer;
    uint64_t m_flags;
    memory_block_data *m_data_reference;

    /** Returns true if the type is builtin */
    inline bool is_builtin_type() const {
        return (reinterpret_cast<uintptr_t>(m_type)&(~builtin_type_id_mask)) == 0;
    }

    /** Should only be called if is_builtin_type() returns true */
    inline type_id_t get_builtin_type_id() const {
        return static_cast<type_id_t>(reinterpret_cast<uintptr_t>(m_type));
    }

    inline type_id_t get_type_id() const {
        if (is_builtin_type()) {
            return get_builtin_type_id();
        } else {
            return m_type->get_type_id();
        }
    }
};

/**
 * Creates a memory block for holding an ndobject (i.e. a container for ndobject metadata)
 *
 * The created object is uninitialized.
 */
memory_block_ptr make_array_memory_block(size_t metadata_size);

/**
 * Creates a memory block for holding an ndobject (i.e. a container for ndobject metadata),
 * as well as storage for embedding additional POD storage such as the array data.
 *
 * The created object is uninitialized.
 */
memory_block_ptr make_array_memory_block(size_t metadata_size, size_t extra_size,
                    size_t extra_alignment, char **out_extra_ptr);

/**
 * Creates an ndobject memory block, and default-constructs it for the type
 * and specified shape.
 */
memory_block_ptr make_array_memory_block(const ndt::type& dt, size_t ndim, const intptr_t *shape);

/**
 * Makes a shallow copy of the ndobject memory block. In the copy, only the
 * ndobject metadata is duplicated, all the references are the same. Any NULL
 * references are swapped to point at the original ndobject memory block, as they
 * are a signal that the data was embedded in the same memory allocation.
 */
memory_block_ptr shallow_copy_array_memory_block(const memory_block_ptr& ndo);

void array_memory_block_debug_print(const memory_block_data *memblock, std::ostream& o, const std::string& indent);

} // namespace dynd

#endif // _DYND__NDOBJECT_MEMORY_BLOCK_HPP_
