//
// Copyright (C) 2011-16 DyND Developers
// BSD 2-Clause License, see LICENSE.txt
//

#pragma once

#include <dynd/callables/default_instantiable_callable.hpp>
#include <dynd/kernels/less_kernel.hpp>

namespace dynd {
namespace nd {

  template <typename Arg0Type, typename Arg1Type>
  class less_callable : public default_instantiable_callable<less_kernel<Arg0Type, Arg1Type>> {
  public:
    less_callable()
        : default_instantiable_callable<less_kernel<Arg0Type, Arg1Type>>(ndt::make_type<ndt::callable_type>(
              ndt::make_type<bool1>(), {ndt::make_type<Arg0Type>(), ndt::make_type<Arg1Type>()})) {}
  };

} // namespace dynd::nd
} // namespace dynd
