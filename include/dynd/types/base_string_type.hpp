//
// Copyright (C) 2011-13 Mark Wiebe, DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__BASE_STRING_TYPE_HPP_
#define _DYND__BASE_STRING_TYPE_HPP_

#include <dynd/types/base_type.hpp>
#include <dynd/string_encodings.hpp>

namespace dynd {


/**
 * Base class for all string types. If a type
 * has kind string_kind, it must be a subclass of
 * base_string_type.
 */
class base_string_type : public base_type {
public:
    inline base_string_type(type_id_t type_id, size_t data_size,
                    size_t alignment, flags_type flags, size_t metadata_size)
        : base_type(type_id, string_kind, data_size, alignment, flags, metadata_size, 0)
    {}

    virtual ~base_string_type();
    /** The encoding used by the string */
    virtual string_encoding_t get_encoding() const = 0;

    /** Retrieves the data range in which a string is stored */
    virtual void get_string_range(const char **out_begin, const char**out_end,
                    const char *metadata, const char *data) const = 0;
    /** Converts a string element into a C++ std::string with a UTF8 encoding */
    std::string get_utf8_string(const char *metadata, const char *data, assign_error_mode errmode) const;
    /** Copies a string with a UTF8 encoding to a string element */
    virtual void set_utf8_string(const char *metadata, char *data, assign_error_mode errmode,
                    const char* utf8_begin, const char *utf8_end) const = 0;
    /** Copies a C++ std::string with a UTF8 encoding to a string element */
    inline void set_utf8_string(const char *metadata, char *data, assign_error_mode errmode,
                    const std::string& utf8_str) const {
        set_utf8_string(metadata, data, errmode, utf8_str.data(), utf8_str.data() + utf8_str.size());
    }

    // String types stop the iterdata chain
    // TODO: Maybe it should be more flexible?
    size_t get_iterdata_size(intptr_t ndim) const;

    void get_dynamic_type_properties(
                    const std::pair<std::string, gfunc::callable> **out_properties,
                    size_t *out_count) const;

    void get_dynamic_array_functions(
                    const std::pair<std::string, gfunc::callable> **out_functions,
                    size_t *out_count) const;
};


} // namespace dynd

#endif // _DYND__BASE_STRING_TYPE_HPP_