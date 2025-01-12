// Copyright Contributors to the Open Shading Language project.
// SPDX-License-Identifier: BSD-3-Clause
// https://github.com/AcademySoftwareFoundation/OpenShadingLanguage


#ifndef OSLVERSION_H
#define OSLVERSION_H


// Versions.  There are three different sets of version numbers:
// (a) The version of the OSL language specification itself, i.e.,
//     what is OSL syntax, semantics, and standard library.
// (b) The version of the OSL library, i.e., this very software whose
//     source code you are reading.
// (c) The version of the .oso file format that this library uses
//     for compiled shaders.
// These are all independent, though loosely coupled.

// Version of the language:
#define OSL_VERSION_MAJOR 1
#define OSL_VERSION_MINOR 13
#define OSL_VERSION_PATCH 7
#define OSL_VERSION_TWEAK 0
#define OSL_VERSION (10000 * OSL_VERSION_MAJOR + \
                               100 * OSL_VERSION_MINOR + \
                                     OSL_VERSION_PATCH)

// Version of this library:
#define OSL_LIBRARY_VERSION_MAJOR 1
#define OSL_LIBRARY_VERSION_MINOR 13
#define OSL_LIBRARY_VERSION_PATCH 7
#define OSL_LIBRARY_VERSION_TWEAK 0
#define OSL_LIBRARY_VERSION_RELEASE_TYPE 

#define OSL_LIBRARY_VERSION_CODE (10000 * OSL_VERSION_MAJOR + \
                                    100 * OSL_VERSION_MINOR + \
                                          OSL_VERSION_PATCH)

// Magic macros to make OSL_LIBRARY_VERSION_STRING that looks like "1.2.3.0"
#define OSL_MAKE_VERSION_STRING2(a,b,c,d,e) #a "." #b "." #c "." #d #e
#define OSL_MAKE_VERSION_STRING(a,b,c,d,e) OSL_MAKE_VERSION_STRING2(a,b,c,d,e)
#define OSL_LIBRARY_VERSION_STRING \
    OSL_MAKE_VERSION_STRING(OSL_LIBRARY_VERSION_MAJOR, \
                            OSL_LIBRARY_VERSION_MINOR, \
                            OSL_LIBRARY_VERSION_PATCH, \
                            OSL_LIBRARY_VERSION_TWEAK, \
                            OSL_LIBRARY_VERSION_RELEASE_TYPE)
#define OSL_INTRO_STRING "Open Shading Language " OSL_LIBRARY_VERSION_STRING
#define OSL_COPYRIGHT_STRING "Copyright Contributors to the Open Shading Language project."


// Version numbers for the compiled shader format.  The major number
// should only be changed if old .oso files should not be expected to
// work with current versions of the library (this should be EXTREMELY
// rare, hopefully never).  The minor number may be changed if newly
// compiled shaders won't work for old libraries, which sometimes
// happens but still should not be often.  Adding a new hint or a new
// shader instruction is not considered a change to the file format!
#define OSO_FILE_VERSION_MAJOR 1
#define OSO_FILE_VERSION_MINOR 0


// Symbols documenting specific API or other changes
#define OSL_SUPPORTS_WEIGHTED_CLOSURE_COMPONENTS 1
#define OSL_SHADERGLOBALS_HAS_RENDERER_PTR 1


// Establish the name spaces
namespace OSL_v1_13 { }
namespace OSL = OSL_v1_13;

// Macros to use in each file to enter and exit the right name spaces.
#define OSL_NAMESPACE OSL_v1_13
#define OSL_NAMESPACE_STRING "OSL_v1_13"
#define OSL_NAMESPACE_ENTER namespace OSL_v1_13 {
#define OSL_NAMESPACE_EXIT }


// Which CPP standard (11, 14, etc.) was this copy of OIIO *built* with?
#define OSL_BUILD_CPP 17

// DEPRECATED(2.1): old macros separately giving compatibility.
#define OSL_BUILD_CPP11 (17 >= 11)
#define OSL_BUILD_CPP14 (17 >= 14)
#define OSL_BUILD_CPP17 (17 >= 17)
#define OSL_BUILD_CPP20 (17 >= 20)

#define OSL_SHADER_INSTALL_DIR "C:/db/build/S/VS1564R/Release/osl/share/OSL/shaders"

// Macro helpers to build function names based on other macros
#define __OSL_CONCAT_INDIRECT(A, B) A ## B
#define __OSL_CONCAT(A, B) __OSL_CONCAT_INDIRECT(A,B)
#define __OSL_CONCAT3(A, B, C) __OSL_CONCAT(__OSL_CONCAT(A,B),C)
#define __OSL_CONCAT4(A, B, C, D) __OSL_CONCAT(__OSL_CONCAT3(A,B,C),D)
#define __OSL_CONCAT5(A, B, C, D, E) __OSL_CONCAT(__OSL_CONCAT4(A,B,C,D),E)
#define __OSL_CONCAT6(A, B, C, D, E, F) __OSL_CONCAT(__OSL_CONCAT5(A,B,C,D,E),F)
#define __OSL_CONCAT7(A, B, C, D, E, F, G) __OSL_CONCAT(__OSL_CONCAT6(A,B,C,D,E,F),G)
#define __OSL_CONCAT8(A, B, C, D, E, F, G, H) __OSL_CONCAT(__OSL_CONCAT7(A,B,C,D,E,F,G),H)
#define __OSL_CONCAT9(A, B, C, D, E, F, G, H, I) __OSL_CONCAT(__OSL_CONCAT8(A,B,C,D,E,F,G,H),I)
#define __OSL_CONCAT10(A, B, C, D, E, F, G, H, I, J) __OSL_CONCAT(__OSL_CONCAT9(A,B,C,D,E,F,G,H,I),J)

#if defined(__OSL_TARGET_ISA) && defined(__OSL_WIDTH)
    #define __OSL_WIDE_PVT __OSL_CONCAT5(b,__OSL_WIDTH,_,__OSL_TARGET_ISA,_pvt)
#endif

#define __OSL_INDIRECT_STRINGIFY(x) #x
#define __OSL_STRINGIFY(x) __OSL_INDIRECT_STRINGIFY(x)

#endif /* OSLVERSION_H */
