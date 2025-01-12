#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "OSL::oslcomp" for configuration "Release"
set_property(TARGET OSL::oslcomp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OSL::oslcomp PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/oslcomp.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oslcomp.dll"
  )

list(APPEND _cmake_import_check_targets OSL::oslcomp )
list(APPEND _cmake_import_check_files_for_OSL::oslcomp "${_IMPORT_PREFIX}/lib/oslcomp.lib" "${_IMPORT_PREFIX}/bin/oslcomp.dll" )

# Import target "OSL::oslquery" for configuration "Release"
set_property(TARGET OSL::oslquery APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OSL::oslquery PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/oslquery.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oslquery.dll"
  )

list(APPEND _cmake_import_check_targets OSL::oslquery )
list(APPEND _cmake_import_check_files_for_OSL::oslquery "${_IMPORT_PREFIX}/lib/oslquery.lib" "${_IMPORT_PREFIX}/bin/oslquery.dll" )

# Import target "OSL::oslexec" for configuration "Release"
set_property(TARGET OSL::oslexec APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OSL::oslexec PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/oslexec.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oslexec.dll"
  )

list(APPEND _cmake_import_check_targets OSL::oslexec )
list(APPEND _cmake_import_check_files_for_OSL::oslexec "${_IMPORT_PREFIX}/lib/oslexec.lib" "${_IMPORT_PREFIX}/bin/oslexec.dll" )

# Import target "OSL::oslnoise" for configuration "Release"
set_property(TARGET OSL::oslnoise APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OSL::oslnoise PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/oslnoise.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oslnoise.dll"
  )

list(APPEND _cmake_import_check_targets OSL::oslnoise )
list(APPEND _cmake_import_check_files_for_OSL::oslnoise "${_IMPORT_PREFIX}/lib/oslnoise.lib" "${_IMPORT_PREFIX}/bin/oslnoise.dll" )

# Import target "OSL::oslc" for configuration "Release"
set_property(TARGET OSL::oslc APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OSL::oslc PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oslc.exe"
  )

list(APPEND _cmake_import_check_targets OSL::oslc )
list(APPEND _cmake_import_check_files_for_OSL::oslc "${_IMPORT_PREFIX}/bin/oslc.exe" )

# Import target "OSL::oslinfo" for configuration "Release"
set_property(TARGET OSL::oslinfo APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(OSL::oslinfo PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/oslinfo.exe"
  )

list(APPEND _cmake_import_check_targets OSL::oslinfo )
list(APPEND _cmake_import_check_files_for_OSL::oslinfo "${_IMPORT_PREFIX}/bin/oslinfo.exe" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
