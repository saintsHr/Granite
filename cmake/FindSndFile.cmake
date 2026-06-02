find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_SNDFILE QUIET sndfile)
endif()

find_path(SndFile_INCLUDE_DIR
    NAMES sndfile.h
    HINTS ${PC_SNDFILE_INCLUDE_DIRS}
)

find_library(SndFile_LIBRARY
    NAMES sndfile
    HINTS ${PC_SNDFILE_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SndFile
    REQUIRED_VARS SndFile_LIBRARY SndFile_INCLUDE_DIR
)

if(SndFile_FOUND AND NOT TARGET SndFile::sndfile)
    add_library(SndFile::sndfile UNKNOWN IMPORTED)
    set_target_properties(SndFile::sndfile PROPERTIES
        IMPORTED_LOCATION "${SndFile_LIBRARY}"
        INTERFACE_INCLUDE_DIRECTORIES "${SndFile_INCLUDE_DIR}"
    )
endif()

mark_as_advanced(SndFile_INCLUDE_DIR SndFile_LIBRARY)
