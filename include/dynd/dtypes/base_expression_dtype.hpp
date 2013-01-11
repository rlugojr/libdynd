//
// Copyright (C) 2011-12, Dynamic NDArray Developers
// BSD 2-Clause License, see LICENSE.txt
//

#ifndef _DYND__BASE_EXPRESSION_DTYPE_HPP_
#define _DYND__BASE_EXPRESSION_DTYPE_HPP_

#include <dynd/dtypes/base_dtype.hpp>

namespace dynd {

/**
 * Base class for all dtypes of expression_kind, and for the pointer_dtype.
 */
class base_expression_dtype : public base_dtype {
public:
    inline base_expression_dtype(type_id_t type_id, dtype_kind_t kind, size_t data_size, size_t alignment, size_t undim=0)
        : base_dtype(type_id, kind, data_size, alignment, undim)
    {}

    virtual ~base_expression_dtype();

    /**
     * Should return a reference to the dtype representing the value which
     * is for calculation. This should never be an expression dtype.
     */
    virtual const dtype& get_value_dtype() const = 0;
    /**
     * Should return a reference to a dtype representing the data this dtype
     * uses to produce the value.
     */
    virtual const dtype& get_operand_dtype() const = 0;

    /** Returns a kernel which converts from (operand_dtype().value_dtype()) to (value_dtype()) */
    virtual void get_operand_to_value_kernel(const eval::eval_context *ectx,
                            kernel_instance<unary_operation_pair_t>& out_borrowed_kernel) const = 0;
    /** Returns a kernel which converts from (value_dtype()) to (operand_dtype().value_dtype()) */
    virtual void get_value_to_operand_kernel(const eval::eval_context *ectx,
                            kernel_instance<unary_operation_pair_t>& out_borrowed_kernel) const = 0;

    /**
     * This method is for expression dtypes, and is a way to substitute
     * the storage dtype (deepest operand dtype) of an existing dtype.
     *
     * The value_dtype of the replacement should match the storage dtype
     * of this instance. Implementations of this should raise an exception
     * when this is not true.
     */
    virtual dtype with_replaced_storage_dtype(const dtype& replacement_dtype) const = 0;

    // Always return true for expression dtypes
    bool is_expression() const;

    // The canonical dtype for expression dtypes is always the value dtype
    dtype get_canonical_dtype() const;

    // Expression dtypes use the values from their operand dtype.
    size_t get_metadata_size() const;
    void metadata_default_construct(char *metadata, int ndim, const intptr_t* shape) const;
    void metadata_copy_construct(char *dst_metadata, const char *src_metadata, memory_block_data *embedded_reference) const;
    void metadata_destruct(char *metadata) const;
    void metadata_debug_print(const char *metadata, std::ostream& o, const std::string& indent) const;

    // Expression dtypes stop the iterdata chain
    // TODO: Maybe it should be more flexible?
    size_t get_iterdata_size(int ndim) const;
};

} // namespace dynd

#endif // _DYND__BASE_EXPRESSION_DTYPE_HPP_
