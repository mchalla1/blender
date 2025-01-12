  // -*- C++ -*-
  //===----------------------------------------------------------------------===//
  // Modifications Copyright Intel Corporation 2022
  // SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
  //===----------------------------------------------------------------------===//
  // Auto-generated from boost/mp11 sources https://github.com/boostorg/mp11

#ifndef SYCL_DETAIL_BOOST_MP11_DETAIL_MP_VOID_HPP_INCLUDED
#define SYCL_DETAIL_BOOST_MP11_DETAIL_MP_VOID_HPP_INCLUDED

//  Copyright 2015-2017 Peter Dimov.
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
namespace sycl
{
inline namespace _V1
{
namespace detail
{
namespace boost
{
namespace mp11
{

// mp_void<T...>
namespace detail
{

template<class... T> struct mp_void_impl
{
    using type = void;
};

} // namespace detail

template<class... T> using mp_void = typename detail::mp_void_impl<T...>::type;

} // namespace mp11
} // namespace boost
} // namespace detail
} // namespace _V1
} // namespace sycl

#endif // #ifndef SYCL_DETAIL_BOOST_MP11_DETAIL_MP_VOID_HPP_INCLUDED
