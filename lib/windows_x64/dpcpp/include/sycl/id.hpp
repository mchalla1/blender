//==-------------------- id.hpp --- SYCL iteration id ----------------------==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#pragma once

#include <sycl/detail/array.hpp>              // for array
#include <sycl/detail/common.hpp>             // for InitializedVal
#include <sycl/detail/defines.hpp>            // for __SYCL_ASSUME_INT
#include <sycl/detail/defines_elementary.hpp> // for __SYCL_DEPRECATED, __SYCL_A...
#include <sycl/exception.hpp> // for make_error_code, errc, exce...
#include <sycl/range.hpp>     // for range

#include <stddef.h>    // for size_t
#include <type_traits> // for enable_if_t, conditional_t

namespace sycl {
inline namespace _V1 {
// Forward declarations
namespace detail {
template <typename TransformedArgType, int Dims, typename KernelType>
class RoundedRangeKernel;
template <typename TransformedArgType, int Dims, typename KernelType>
class RoundedRangeKernelWithKH;
} // namespace detail
template <int Dimensions> class range;
template <int Dimensions, bool with_offset> class item;

/// A unique identifier of an item in an index space.
///
/// \ingroup sycl_api
template <int Dimensions = 1> class id : public detail::array<Dimensions> {
public:
  static constexpr int dimensions = Dimensions;

private:
  using base = detail::array<Dimensions>;
  static_assert(Dimensions >= 1 && Dimensions <= 3,
                "id can only be 1, 2, or 3 Dimensional.");
  template <int N, int val, typename T>
  using ParamTy = std::enable_if_t<(N == val), T>;

#ifndef __SYCL_DISABLE_ID_TO_INT_CONV__
  /* Helper class for conversion operator. Void type is not suitable. User
   * cannot even try to get address of the operator __private_class(). User
   * may try to get an address of operator void() and will get the
   * compile-time error */
  class __private_class;

  template <typename N, typename T>
  using EnableIfIntegral = std::enable_if_t<std::is_integral_v<N>, T>;
  template <bool B, typename T>
  using EnableIfT = std::conditional_t<B, T, __private_class>;
#endif // __SYCL_DISABLE_ID_TO_INT_CONV__

public:
  id() = default;

  /* The following constructor is only available in the id struct
   * specialization where: Dimensions==1 */
  template <int N = Dimensions> id(ParamTy<N, 1, size_t> dim0) : base(dim0) {}

  template <int N = Dimensions>
  id(ParamTy<N, 1, const range<Dimensions>> &range_size)
      : base(range_size.get(0)) {}

  template <int N = Dimensions, bool with_offset = true>
  id(ParamTy<N, 1, const item<Dimensions, with_offset>> &item)
      : base(item.get_id(0)) {}

  /* The following constructor is only available in the id struct
   * specialization where: Dimensions==2 */
  template <int N = Dimensions>
  id(ParamTy<N, 2, size_t> dim0, size_t dim1) : base(dim0, dim1) {}

  template <int N = Dimensions>
  id(ParamTy<N, 2, const range<Dimensions>> &range_size)
      : base(range_size.get(0), range_size.get(1)) {}

  template <int N = Dimensions, bool with_offset = true>
  id(ParamTy<N, 2, const item<Dimensions, with_offset>> &item)
      : base(item.get_id(0), item.get_id(1)) {}

  /* The following constructor is only available in the id struct
   * specialization where: Dimensions==3 */
  template <int N = Dimensions>
  id(ParamTy<N, 3, size_t> dim0, size_t dim1, size_t dim2)
      : base(dim0, dim1, dim2) {}

  template <int N = Dimensions>
  id(ParamTy<N, 3, const range<Dimensions>> &range_size)
      : base(range_size.get(0), range_size.get(1), range_size.get(2)) {}

  template <int N = Dimensions, bool with_offset = true>
  id(ParamTy<N, 3, const item<Dimensions, with_offset>> &item)
      : base(item.get_id(0), item.get_id(1), item.get_id(2)) {}

  __SYCL_DEPRECATED("range() conversion is deprecated")
  explicit operator range<Dimensions>() const {
    range<Dimensions> result(
        detail::InitializedVal<Dimensions, range>::template get<0>());
    for (int i = 0; i < Dimensions; ++i) {
      result[i] = this->get(i);
    }
    return result;
  }

#ifndef __SYCL_DISABLE_ID_TO_INT_CONV__
  /* Template operator is not allowed because it disables further type
   * conversion. For example, the next code will not work in case of template
   * conversion:
   * int a = id<1>(value); */

  __SYCL_ALWAYS_INLINE operator EnableIfT<(Dimensions == 1), size_t>() const {
    size_t Result = this->common_array[0];
    __SYCL_ASSUME_INT(Result);
    return Result;
  }
#endif // __SYCL_DISABLE_ID_TO_INT_CONV__

// OP is: ==, !=
#ifndef __SYCL_DISABLE_ID_TO_INT_CONV__
  using detail::array<Dimensions>::operator==;
  // Needed for clang in C++20 mode as the above operator== would be ambigious
  // between regular/reversed call for "Id == Id" case.
  bool operator==(const id<Dimensions> &rhs) const {
    return this->detail::array<Dimensions>::operator==(rhs);
  }
#if __cpp_impl_three_way_comparison < 201907
  using detail::array<Dimensions>::operator!=;
#endif

  /* Enable operators with integral types.
   * Template operators take precedence than type conversion. In the case of
   * non-template operators, ambiguity appears: "id op size_t" may refer
   * "size_t op size_t" and "id op size_t". In case of template operators it
   * will be "id op size_t"*/
#define __SYCL_GEN_OPT(op)                                                     \
  template <typename T>                                                        \
  EnableIfIntegral<T, bool> operator op(const T &rhs) const {                  \
    if (this->common_array[0] != rhs)                                          \
      return false op true;                                                    \
    return true op true;                                                       \
  }                                                                            \
  template <typename T>                                                        \
  friend EnableIfIntegral<T, bool> operator op(const T &lhs,                   \
                                               const id<Dimensions> &rhs) {    \
    if (lhs != rhs.common_array[0])                                            \
      return false op true;                                                    \
    return true op true;                                                       \
  }

  __SYCL_GEN_OPT(==)
  __SYCL_GEN_OPT(!=)

#undef __SYCL_GEN_OPT

#endif // __SYCL_DISABLE_ID_TO_INT_CONV__

// OP is: +, -, *, /, %, <<, >>, &, |, ^, &&, ||, <, >, <=, >=
#define __SYCL_GEN_OPT_BASE(op)                                                \
  friend id<Dimensions> operator op(const id<Dimensions> &lhs,                 \
                                    const id<Dimensions> &rhs) {               \
    id<Dimensions> result;                                                     \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      result.common_array[i] = lhs.common_array[i] op rhs.common_array[i];     \
    }                                                                          \
    return result;                                                             \
  }

#ifndef __SYCL_DISABLE_ID_TO_INT_CONV__
// Enable operators with integral types only
#define __SYCL_GEN_OPT(op)                                                     \
  __SYCL_GEN_OPT_BASE(op)                                                      \
  template <typename T>                                                        \
  friend EnableIfIntegral<T, id<Dimensions>> operator op(                      \
      const id<Dimensions> &lhs, const T &rhs) {                               \
    id<Dimensions> result;                                                     \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      result.common_array[i] = lhs.common_array[i] op rhs;                     \
    }                                                                          \
    return result;                                                             \
  }                                                                            \
  template <typename T>                                                        \
  friend EnableIfIntegral<T, id<Dimensions>> operator op(                      \
      const T &lhs, const id<Dimensions> &rhs) {                               \
    id<Dimensions> result;                                                     \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      result.common_array[i] = lhs op rhs.common_array[i];                     \
    }                                                                          \
    return result;                                                             \
  }
#else
#define __SYCL_GEN_OPT(op)                                                     \
  __SYCL_GEN_OPT_BASE(op)                                                      \
  friend id<Dimensions> operator op(const id<Dimensions> &lhs,                 \
                                    const size_t &rhs) {                       \
    id<Dimensions> result;                                                     \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      result.common_array[i] = lhs.common_array[i] op rhs;                     \
    }                                                                          \
    return result;                                                             \
  }                                                                            \
  friend id<Dimensions> operator op(const size_t &lhs,                         \
                                    const id<Dimensions> &rhs) {               \
    id<Dimensions> result;                                                     \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      result.common_array[i] = lhs op rhs.common_array[i];                     \
    }                                                                          \
    return result;                                                             \
  }
#endif // __SYCL_DISABLE_ID_TO_INT_CONV__

  __SYCL_GEN_OPT(+)
  __SYCL_GEN_OPT(-)
  __SYCL_GEN_OPT(*)
  __SYCL_GEN_OPT(/)
  __SYCL_GEN_OPT(%)
  __SYCL_GEN_OPT(<<)
  __SYCL_GEN_OPT(>>)
  __SYCL_GEN_OPT(&)
  __SYCL_GEN_OPT(|)
  __SYCL_GEN_OPT(^)
  __SYCL_GEN_OPT(&&)
  __SYCL_GEN_OPT(||)
  __SYCL_GEN_OPT(<)
  __SYCL_GEN_OPT(>)
  __SYCL_GEN_OPT(<=)
  __SYCL_GEN_OPT(>=)

#undef __SYCL_GEN_OPT
#undef __SYCL_GEN_OPT_BASE

// OP is: +=, -=, *=, /=, %=, <<=, >>=, &=, |=, ^=
#define __SYCL_GEN_OPT(op)                                                     \
  friend id<Dimensions> &operator op(id<Dimensions> &lhs,                      \
                                     const id<Dimensions> &rhs) {              \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      lhs.common_array[i] op rhs.common_array[i];                              \
    }                                                                          \
    return lhs;                                                                \
  }                                                                            \
  friend id<Dimensions> &operator op(id<Dimensions> &lhs, const size_t &rhs) { \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      lhs.common_array[i] op rhs;                                              \
    }                                                                          \
    return lhs;                                                                \
  }

  __SYCL_GEN_OPT(+=)
  __SYCL_GEN_OPT(-=)
  __SYCL_GEN_OPT(*=)
  __SYCL_GEN_OPT(/=)
  __SYCL_GEN_OPT(%=)
  __SYCL_GEN_OPT(<<=)
  __SYCL_GEN_OPT(>>=)
  __SYCL_GEN_OPT(&=)
  __SYCL_GEN_OPT(|=)
  __SYCL_GEN_OPT(^=)

#undef __SYCL_GEN_OPT

// OP is unary +, -
#define __SYCL_GEN_OPT(op)                                                     \
  friend id<Dimensions> operator op(const id<Dimensions> &rhs) {               \
    id<Dimensions> result;                                                     \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      result.common_array[i] = (op rhs.common_array[i]);                       \
    }                                                                          \
    return result;                                                             \
  }

  __SYCL_GEN_OPT(+)
  __SYCL_GEN_OPT(-)

#undef __SYCL_GEN_OPT

// OP is prefix ++, --
#define __SYCL_GEN_OPT(op)                                                     \
  friend id<Dimensions> &operator op(id<Dimensions> &rhs) {                    \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      op rhs.common_array[i];                                                  \
    }                                                                          \
    return rhs;                                                                \
  }

  __SYCL_GEN_OPT(++)
  __SYCL_GEN_OPT(--)

#undef __SYCL_GEN_OPT

// OP is postfix ++, --
#define __SYCL_GEN_OPT(op)                                                     \
  friend id<Dimensions> operator op(id<Dimensions> &lhs, int) {                \
    id<Dimensions> old_lhs;                                                    \
    for (int i = 0; i < Dimensions; ++i) {                                     \
      old_lhs.common_array[i] = lhs.common_array[i];                           \
      op lhs.common_array[i];                                                  \
    }                                                                          \
    return old_lhs;                                                            \
  }

  __SYCL_GEN_OPT(++)
  __SYCL_GEN_OPT(--)

#undef __SYCL_GEN_OPT
};

namespace detail {
template <int Dimensions>
size_t getOffsetForId(range<Dimensions> Range, id<Dimensions> Id,
                      id<Dimensions> Offset) {
  size_t offset = 0;
  for (int i = 0; i < Dimensions; ++i)
    offset = offset * Range[i] + Offset[i] + Id[i];
  return offset;
}

inline id<1> getDelinearizedId(const range<1> &, size_t Index) {
  return {Index};
}

inline id<2> getDelinearizedId(const range<2> &Range, size_t Index) {
  size_t X = Index % Range[1];
  size_t Y = Index / Range[1];
  return {Y, X};
}

inline id<3> getDelinearizedId(const range<3> &Range, size_t Index) {
  size_t D1D2 = Range[1] * Range[2];
  size_t Z = Index / D1D2;
  size_t ZRest = Index % D1D2;
  size_t Y = ZRest / Range[2];
  size_t X = ZRest % Range[2];
  return {Z, Y, X};
}
} // namespace detail

// C++ feature test macros are supported by all supported compilers
// with the exception of MSVC 1914. It doesn't support deduction guides.
#ifdef __cpp_deduction_guides
id(size_t)->id<1>;
id(size_t, size_t)->id<2>;
id(size_t, size_t, size_t)->id<3>;
#endif

template <int Dims>
__SYCL_DEPRECATED("use sycl::ext::oneapi::experimental::this_id() instead")
id<Dims> this_id() {
#ifdef __SYCL_DEVICE_ONLY__
  return detail::Builder::getElement(detail::declptr<id<Dims>>());
#else
  throw sycl::exception(
      sycl::make_error_code(sycl::errc::feature_not_supported),
      "Free function calls are not supported on host");
#endif
}

namespace ext::oneapi::experimental {
template <int Dims> id<Dims> this_id() {
#ifdef __SYCL_DEVICE_ONLY__
  return sycl::detail::Builder::getElement(sycl::detail::declptr<id<Dims>>());
#else
  throw sycl::exception(
      sycl::make_error_code(sycl::errc::feature_not_supported),
      "Free function calls are not supported on host");
#endif
}
} // namespace ext::oneapi::experimental
} // namespace _V1
} // namespace sycl