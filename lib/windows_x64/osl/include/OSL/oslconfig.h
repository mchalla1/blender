// Copyright Contributors to the Open Shading Language project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/AcademySoftwareFoundation/OpenShadingLanguage

#pragma once

#define OSL_USE_OPTIX 1
#define OSL_BUILD_BATCHED 0
#define OSL_USE_BATCHED OSL_BUILD_BATCHED


#include <OSL/export.h>
#include <OSL/oslversion.h>
#include <OSL/platform.h>


// All the things we need from Imath.
#define OSL_USING_IMATH 3
#if OSL_USING_IMATH >= 3
#   include <Imath/ImathVec.h>
#   include <Imath/ImathMatrix.h>
#   include <Imath/ImathColor.h>
#else
    // OpenEXR 2.x lacks the Cuda decorators we need, so we replicated some
    // Imath files in OSL/Imathx, adding the decorations needed for them to
    // be safe to compile with Cuda (and sometimes other improvements).
#   include <OSL/Imathx/ImathLimits.h>
#   include <OSL/Imathx/ImathVec.h>
#   include <OSL/Imathx/ImathMatrix.h>
#   include <OSL/Imathx/ImathColor.h>
#endif

// We included the Imath files we needed, so set the OIIO_IMATH_H_INCLUDED
// symbol to prevent OpenImageIO/Imath.h from including them again (or
// before), which is critical if they turn out to be different versions.
#define OIIO_IMATH_H_INCLUDED 1


// The fmt library causes trouble for Cuda. Work around by telling fmt not to
// use the troublesome int128 that Cuda doesn't understand.
#ifdef __CUDA_ARCH__
#    ifndef FMT_USE_INT128
#        define FMT_USE_INT128 0
#    endif
#endif

// For OSL internal compilation only, get deprecation warnings if we call OIIO
// methods that use the old printf style interface.
#ifdef OSL_INTERNAL
#    ifdef OIIO_ERRORHANDLER_HIDE_PRINTF
#        define OIIO_ERRORHANDLER_HIDE_PRINTF 1
#    endif
#    ifdef OIIO_HIDE_FORMAT
#        define OIIO_HIDE_FORMAT 1
#    endif
#endif

// All the things we need from OpenImageIO
#include <OpenImageIO/oiioversion.h>
#include <OpenImageIO/errorhandler.h>
#include <OpenImageIO/texture.h>
#include <OpenImageIO/typedesc.h>
#include <OpenImageIO/ustring.h>
#include <OpenImageIO/platform.h>
#include <OpenImageIO/span.h>


OSL_NAMESPACE_ENTER

/// Various compile-time defaults are defined here that could, in
/// principle, be redefined if you are using OSL in some particular
/// renderer that wanted things a different way.

/// By default, we operate with single precision float.  Change this
/// definition to make a shading system that fundamentally operates
/// on doubles.
/// FIXME: it's very likely that all sorts of other things will break
/// if you do this, but eventually we should make sure it works.
typedef float Float;

/// By default, use the excellent Imath vector, matrix, and color types
/// from the IlmBase package from: http://www.openexr.com
///
/// It's permissible to override these types with the vector, matrix,
/// and color classes of your choice, provided that (a) your vectors
/// have the same data layout as a simple Float[n]; (b) your
/// matrices have the same data layout as Float[n][n]; and (c) your
/// classes have most of the obvious constructors and overloaded
/// operators one would expect from a C++ vector/matrix/color class.
using Vec2     = Imath::Vec2<Float>;
using Vec3     = Imath::Vec3<Float>;
using Color3   = Imath::Color3<Float>;
using Matrix33 = Imath::Matrix33<Float>;
using Matrix44 = Imath::Matrix44<Float>;



/// Assume that we are dealing with OpenImageIO's texture system.  It
/// doesn't literally have to be OIIO's... it just needs to have the
/// same API as OIIO's TextureSystem class, it's a purely abstract class
/// anyway.
using OIIO::TextureSystem;
using OIIO::TextureOpt;

// And some other things we borrow from OIIO...
using OIIO::ErrorHandler;
using OIIO::ustring;
using OIIO::ustringhash;
using OIIO::string_view;
using OIIO::span;
using OIIO::cspan;

using OIIO::TypeDesc;
using OIIO::TypeUnknown;
using OIIO::TypeFloat;
using OIIO::TypeColor;
using OIIO::TypePoint;
using OIIO::TypeVector;
using OIIO::TypeNormal;
using OIIO::TypeMatrix;
using OIIO::TypeFloat4;
using OIIO::TypeString;
using OIIO::TypeInt;
using OIIO::TypeFloat2;
using OIIO::TypeVector2;
using OIIO::TypeVector4;
using OIIO::TypeUInt64;

using OIIO::Strutil::print;

template<typename Str, typename... Args>
OSL_NODISCARD inline std::string
fmtformat(const Str& fmt, Args&&... args)
{
    return OIIO::Strutil::fmt::format(fmt, std::forward<Args>(args)...);
}

// TODO: notice the fmt argument is not templatised, this is because
// no implicit conversion exists betwen OSL::string_view and ::fmt::string_view
// so we choose to only accept OSL::string_view and explicitly convert to ::fmt::string_view
// OIIO should fix this if possible.
template<typename OutIt, typename... Args>
OSL_NODISCARD inline auto
fmtformat_to_n(OutIt &out, size_t n, const string_view& fmt, Args&&... args)
{
    // DOES NOT EXIST AS PUBLIC API
    //return OIIO::Strutil::fmt::format_to_n(out, n, fmt, std::forward<Args>(args)...);
    // So call directly into underlying fmt library OIIO is using
    // TODO:  Add format_to_n as a public API in OIIO
    return ::fmt::format_to_n(out, n, ::fmt::string_view{fmt.begin(), fmt.length()}, std::forward<Args>(args)...);
}


/// OSL_USTRINGREP_IS_HASH will be 1 if the build-time option was enabled to
/// make the ustringrep be a ustringhash, rather than a ustring directly.
#define OSL_USTRINGREP_IS_HASH 0

#if OSL_USTRINGREP_IS_HASH
/// ustringrep is the class we use to represent strings in the shader.
using ustringrep = ustringhash;
#else
/// ustringrep is the class we use to represent strings in the shader.
using ustringrep = ustring;
#endif

/// ustringhash_pod is the type we use to pass hash data in llvm function calls.
using ustringhash_pod = size_t;

#if OSL_USTRINGREP_IS_HASH
/// ustring_pod is the type we use to pass string data in llvm function calls.
using ustring_pod = ustringhash_pod;
#else
/// ustring_pod is the type we use to pass string data in llvm function calls.
using ustring_pod = const char*;
#endif



/// Convenience function to convert to a ustring.
inline ustring
ustring_from(ustringhash h)
{
    return ustring::from_hash(h.hash());
}

/// Convenience function to convert to a ustring.
inline ustring
ustring_from(ustring u)
{
    return u;
}

/// Convenience function to convert to a ustring.
inline ustring
ustring_from(string_view s)
{
    return ustring(s);
}


/// Convenience function to convert to a ustringhash.
inline ustringhash
ustringhash_from(ustringhash u)
{
    return u;
}

/// Convenience function to convert USTRINGHASH_POD to a ustringhash.
inline ustringhash
ustringhash_from(ustringhash_pod u) 
{
#if OIIO_VERSION_GREATER_EQUAL(2, 4, 10)
    return ustringhash{u};
#else
    // No constructor taking the pod type existed previously
    // so bitcast it.
    return OSL::bitcast<ustringhash>(u);
#endif
}

/// Convenience function to convert to a ustringhash.
inline ustringhash
ustringhash_from(ustring u)
{
    ustringhash ret;
    // Not legal to call uhash on an empty ustring
    if(!u.empty())
        ret = u.uhash();
    return ret;
}


/// Convenience function to convert to a ustringrep.
inline ustringrep
ustringrep_from(ustringhash h)
{
#if OSL_USTRINGREP_IS_HASH
    return h;
#else
    return ustring::from_hash(h.hash());
#endif
}

/// Convenience function to convert to a ustringrep.
inline ustringrep
ustringrep_from(ustring u)
{
#if OSL_USTRINGREP_IS_HASH
    return u.hash();
#else
    return u;
#endif
}

/// Convenience function to convert to a ustringrep.
inline ustringrep
ustringrep_from(string_view s)
{
    ustring u(s);
#if OSL_USTRINGREP_IS_HASH
    return u.hash();
#else
    return u;
#endif
}



using TypeDesc_pod = int64_t;

static_assert(sizeof(TypeDesc_pod) == sizeof(TypeDesc),
              "TypeDesc size differs from its POD counterpart");

/// Convenience function to convert to a TypeDesc.
inline TypeDesc
TypeDesc_from(TypeDesc_pod type)
{
    return OSL::bitcast<OSL::TypeDesc>(type);
}



// N.B. SymArena is not really "configuration", but we cram it here for
// lack of a better home.

/// The different memory arenas where symbols may live on the app side
enum class SymArena {
    Unknown = 0,        // Unknown/uninitialized value
    Absolute,           // Absolute address
    Heap,               // Belongs to context heap
    Outputs,            // Belongs to output arena
    UserData,           // UserData arena
    Interactive,        // Interactively edited variables
    // ShaderGlobals,   // RESERVED
};



/////////////////////////////////////////////////////////////////////////
// Define various macros and templates that need to be different for
// various C++ versions or compilers, or to make up for things not
// yet available in earlier C++ standards.

#if OSL_CPLUSPLUS_VERSION >= 17
    // fold expression to expand
    #define __OSL_EXPAND_PARAMETER_PACKS(EXPRESSION) \
        (void((EXPRESSION)) , ... );
#else
    // Choose to use initializer list to expand our parameter pack so that
    // the order they are evaluated in the order they appear
    #define __OSL_EXPAND_PARAMETER_PACKS(EXPRESSION) \
        { \
            using expander = int [sizeof...(IntListT)]; \
            (void)expander{((EXPRESSION),0)...}; \
        }
#endif


namespace pvt {

template<int... IntegerListT>
using int_sequence = std::integer_sequence<int, IntegerListT...>;

template<int EndBeforeT>
using make_int_sequence =  std::make_integer_sequence<int, EndBeforeT>;

 template<bool... BoolListT >
using bool_sequence = std::integer_sequence<bool, BoolListT... >;

#if (OSL_CPLUSPLUS_VERSION >= 17)
    template<class... ListT>
    using conjunction = std::conjunction<ListT...>;
#else
    template<class... ListT>
    using conjunction = std::is_same<bool_sequence<true,ListT::value...>, bool_sequence<ListT::value..., true>>;
#endif

    // We need the SFINAE type to be different for
    // enable_if_type from disable_if_type so that we can apply both to
    // the same template signature to avoid
    // "error: invalid redeclaration of member function template"
    // NOTE: std::enable_if_t is a c++14 library feature, our baseline
    // and we wish to remain compatible with c++11 header libraries.
    // Also we are using std::true_type vs. void as the default type
    template <bool TestT, typename TypeT = std::true_type>
    using enable_if_type = typename std::enable_if<TestT, TypeT>::type;

} // namespace pvt

// Instead of relying on compiler loop unrolling, we can statically call functor
// for each integer in a sequence
template <template<int> class ConstantWrapperT, int... IntListT, typename FunctorT>
static OSL_FORCEINLINE OSL_HOSTDEVICE void static_foreach(pvt::int_sequence<IntListT...>, const FunctorT &iFunctor) {
     __OSL_EXPAND_PARAMETER_PACKS( iFunctor(ConstantWrapperT<IntListT>{}) );
}

template <template<int> class ConstantWrapperT, int N, typename FunctorT>
static OSL_FORCEINLINE OSL_HOSTDEVICE void static_foreach(const FunctorT &iFunctor) {
    static_foreach<ConstantWrapperT>(pvt::make_int_sequence<N>(), iFunctor);
}

template<int N>
using ConstIndex = std::integral_constant<int, N>;


// During development it can be useful to output extra information
// NOTE:  internal use only
#ifdef OSL_DEV
    #define OSL_DEV_ONLY(...) __VA_ARGS__
#else
    #define OSL_DEV_ONLY(...)
#endif


OSL_NAMESPACE_EXIT


namespace std {  // not necessary in C++17, then we can just say std::hash
#ifndef OIIO_USTRING_HAS_STDHASH
// Not a new enough OIIO to define std::hash<ustring>, so we'll do it.
// This can be removed once OIIO minimum is 2.4.5+ or >=2.5.0.1
template<> struct hash<OSL::ustring> {
    std::size_t operator()(OSL::ustring u) const noexcept { return u.hash(); }
};

template<> struct hash<OSL::ustringhash> {
    OSL_HOSTDEVICE constexpr std::size_t
    operator()(OSL::ustringhash u) const noexcept
    {
        return u.hash();
    }
};
#endif

// Not necessary once minimum OIIO defines operator< for ustringhash.
// This can be removed once OIIO minimum is 2.4.5+ or >=2.5.0.1
template<> struct less<OSL::ustringhash> {
    OSL_HOSTDEVICE constexpr bool operator()(OSL::ustringhash u,
                                             OSL::ustringhash v) const noexcept
    {
        return u.hash() < v.hash();
    }
};
}  // namespace std


#ifndef OIIO_HAS_USTRINGHASH_FORMATTER
// OIIO is too old to have fmt custom formatter for ustringhash.
// This can be removed once OIIO minimum is 2.4.5+ or >=2.5.0.1
FMT_BEGIN_NAMESPACE
template<> struct formatter<OIIO::ustringhash> : formatter<fmt::string_view, char> {
    template<typename FormatContext>
    auto format(const OIIO::ustringhash& h, FormatContext& ctx)
        -> decltype(ctx.out()) const
    {
        OIIO::ustring u(h);
        return formatter<fmt::string_view, char>::format({ u.data(), u.size() },
                                                         ctx);
    }
};
FMT_END_NAMESPACE
#endif
