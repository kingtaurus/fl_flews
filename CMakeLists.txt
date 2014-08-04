cmake_minimum_required(VERSION 2.8)
project(libflews)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

#http://www.opensource.apple.com/source/X11libs/X11libs-60/libpng/libpng-1.5.1/CMakeLists.txt
set (LIBFLEWS_MAJOR    0)
set (LIBFLEWS_MINOR    3)
set (LIBFLEWS_RELEASE  1)

set(LIBFLEWS_NAME     libflews${LIBFLEWS_MAJOR}${LIBFLEWS_MINOR})
set(LIBFLEWS_VERSION  ${LIBFLEWS_MAJOR}.${LIBFLEWS_MINOR}.${LIBFLEWS_RELEASE})

if(NOT DEFINED FLTK_INCLUDE_DIR)
	find_package(FLTK REQUIRED)
endif(NOT DEFINED FLTK_INCLUDE_DIR)

include_directories(${FLTK_INCLUDE_DIR})

macro(_flews_generate_symlink_code CODE SRC DEST)
  if(WIN32 AND NOT CYGWIN)
    set(_flews_gsc_message "Copying ${SRC} to ${DEST} if needed")
    set(_flews_gsc_operation "copy_if_different")
  else()
    set(_flews_gsc_message "Symlinking ${SRC} to ${DEST}")
    set(_flews_gsc_operation "create_symlink")
  endif()

  set(${CODE} "
    message(STATUS \"${_flews_gsc_message}\")
    execute_process(COMMAND \${CMAKE_COMMAND} -E ${_flews_gsc_operation}
      \"${SRC}\" \"${DEST}\")
    ")
endmacro()

SET (CMAKE_C_FLAGS                "-std=c99 -pedantic -O0 -g3 -Wall -W -fPIC")
SET (CMAKE_C_FLAGS_DEBUG          "-std=c99 -pedantic -O0 -g3 -fPIC")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-std=c99 -pedantic -Os -DNDEBUG -fPIC")
SET (CMAKE_C_FLAGS_RELEASE        "-std=c99 -pedantic -O3 -DNDEBUG -fPIC")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-std=c99 -pedantic -O2 -g2 -fPIC")

SET (CMAKE_CXX_FLAGS                "-std=c++11 -pedantic -O0 -g3 -Wall -Wextra -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fPIC")
SET (CMAKE_CXX_FLAGS_DEBUG          "-std=c++11 -pedantic -O0 -g3 -Wall -Wextra -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fPIC")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-std=c++11 -pedantic -Os -DNDEBUG -fPIC")
SET (CMAKE_CXX_FLAGS_RELEASE        "-std=c++11 -pedantic -O3 -DNDEBUG -fPIC")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-std=c++11 -pedantic -O2 -g2 -fPIC")


if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)

set(prefix          ${CMAKE_INSTALL_PREFIX})
set(exec_prefix     ${CMAKE_INSTALL_PREFIX})
set(libdir          ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(includedir      ${CMAKE_INSTALL_PREFIX}/include)
set(PACKAGE_VERSION ${LIBFLEWS_VERSION})
set(LIBS            "-L${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} -lflews -lm -ldl")
set(Cflags          "-I${includedir}")
set(Cxxlags         "-I${includedir}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libflews.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}.pc @ONLY)
_flews_generate_symlink_code(FLEWS_PC_INSTALL_CODE
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}.pc
  ${CMAKE_CURRENT_BINARY_DIR}/libflews.pc)
install(CODE ${FLEWS_PC_INSTALL_CODE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(src)

