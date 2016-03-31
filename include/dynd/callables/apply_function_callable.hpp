//
// Copyright (C) 2011-15 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/callables/base_callable.hpp>
#include <dynd/kernels/apply_function_kernel.hpp>

namespace dynd {
namespace nd {
  namespace functional {

    template <typename func_type, func_type func, int N = arity_of<func_type>::value>
    class apply_function_callable : public base_callable {
    public:
      template <typename... T>
      apply_function_callable(T &&... names)
          : base_callable(ndt::make_type<typename funcproto_of<func_type>::type>(std::forward<T>(names)...)) {}

      const ndt::type &resolve(call_graph &cg, const ndt::type &dst_tp, size_t DYND_UNUSED(nsrc),
                               const ndt::type *DYND_UNUSED(src_tp), size_t DYND_UNUSED(nkwd),
                               const array *DYND_UNUSED(kwds),
                               const std::map<std::string, ndt::type> &DYND_UNUSED(tp_vars)) {
        cg.emplace_back(this);
        return dst_tp;
      }

      void instantiate(char *DYND_UNUSED(data), kernel_builder *ckb, const ndt::type &DYND_UNUSED(dst_tp),
                       const char *DYND_UNUSED(dst_arrmeta), intptr_t DYND_UNUSED(nsrc),
                       const ndt::type *DYND_UNUSED(src_tp), const char *const *src_arrmeta, kernel_request_t kernreq,
                       intptr_t nkwd, const array *kwds, const std::map<std::string, ndt::type> &DYND_UNUSED(tp_vars)) {
        typedef apply_function_kernel<func_type, func, N> kernel_type;
        ckb->emplace_back<kernel_type>(kernreq, typename kernel_type::args_type(src_arrmeta, kwds),
                                       typename kernel_type::kwds_type(nkwd, kwds));
      }

      void new_instantiate(call_frame *DYND_UNUSED(frame), kernel_builder &ckb, kernel_request_t kernreq,
                           const char *DYND_UNUSED(dst_arrmeta), const char *const *src_arrmeta, size_t nkwd,
                           const array *kwds) {
        typedef apply_function_kernel<func_type, func, N> kernel_type;
        ckb.emplace_back<kernel_type>(kernreq, typename kernel_type::args_type(src_arrmeta, kwds),
                                      typename kernel_type::kwds_type(nkwd, kwds));
      }
    };

  } // namespace dynd::nd::functional
} // namespace dynd::nd
} // namespace dynd
