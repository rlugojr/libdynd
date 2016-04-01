//
// Copyright (C) 2011-15 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/callables/base_callable.hpp>
#include <dynd/assignment.hpp>

namespace dynd {
namespace nd {

  class copy_callable : public base_callable {
  public:
    copy_callable() : base_callable(ndt::type("(A... * S) -> B... * T")) {}

    ndt::type resolve(base_callable *DYND_UNUSED(caller), char *DYND_UNUSED(data), call_graph &DYND_UNUSED(cg),
                      const ndt::type &dst_tp, size_t DYND_UNUSED(nsrc), const ndt::type *DYND_UNUSED(src_tp),
                      size_t DYND_UNUSED(nkwd), const array *DYND_UNUSED(kwds),
                      const std::map<std::string, ndt::type> &DYND_UNUSED(tp_vars)) {
      return dst_tp;
    }

    void resolve_dst_type(char *DYND_UNUSED(data), ndt::type &dst_tp, intptr_t nsrc, const ndt::type *src_tp,
                          intptr_t DYND_UNUSED(nkwd), const array *DYND_UNUSED(kwds),
                          const std::map<std::string, ndt::type> &DYND_UNUSED(tp_vars)) {
      if (nsrc != 1) {
        std::stringstream ss;
        ss << "arrfunc 'copy' expected 1 argument, got " << nsrc;
        throw std::invalid_argument(ss.str());
      }

      dst_tp = src_tp[0].get_canonical_type();
    }

    void instantiate(call_node *DYND_UNUSED(node), char *DYND_UNUSED(data), kernel_builder *ckb,
                     const ndt::type &dst_tp, const char *dst_arrmeta, intptr_t DYND_UNUSED(nsrc),
                     const ndt::type *src_tp, const char *const *src_arrmeta, kernel_request_t kernreq,
                     intptr_t DYND_UNUSED(nkwd), const array *DYND_UNUSED(kwds),
                     const std::map<std::string, ndt::type> &DYND_UNUSED(tp_vars)) {
      array error_mode = eval::default_eval_context.errmode;
      assign->instantiate(nullptr, NULL, ckb, dst_tp, dst_arrmeta, 1, src_tp, src_arrmeta, kernreq, 1, &error_mode,
                          std::map<std::string, ndt::type>());
    }
  };

} // namespace dynd::nd
} // namespace dynd
