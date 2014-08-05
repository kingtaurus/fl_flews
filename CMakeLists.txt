cmake_minimum_required(VERSION 2.8)
project(libflews)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(OpenGL REQUIRED)

#http://www.opensource.apple.com/source/X11libs/X11libs-60/libpng/libpng-1.5.1/CMakeLists.txt
set (LIBFLEWS_MAJOR    0)
set (LIBFLEWS_MINOR    3)
set (LIBFLEWS_RELEASE  1)

set(LIBFLEWS_NAME     libflews${LIBFLEWS_MAJOR}${LIBFLEWS_MINOR})
set(LIBFLEWS_VERSION  ${LIBFLEWS_MAJOR}.${LIBFLEWS_MINOR}.${LIBFLEWS_RELEASE})


if(NOT DEFINED FLTK_INCLUDE_DIR)
	find_package(FLTK REQUIRED)
endif(NOT DEFINED FLTK_INCLUDE_DIR)

INCLUDE_DIRECTORIES(${FLTK_INCLUDE_DIR})

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

SET (CMAKE_C_FLAGS                "-std=c99 -pedantic -Wall -W -fPIC")
SET (CMAKE_C_FLAGS_DEBUG          "-O0 -g3")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -g0 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O3 -g0 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g2")

SET (CMAKE_CXX_FLAGS                "-std=c++11 -Wall -Wextra -Wno-shadow -Wunused-variable -Wno-unused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fPIC")
SET (CMAKE_CXX_FLAGS_DEBUG          "-O0 -g3")
SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -g0 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELEASE        "-O3 -g0 -DNDEBUG")
SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g2")

IF(NOT CMAKE_BUILD_TYPE)
	SET(CMAKE_BUILD_TYPE DEBUG
		CACHE STRING "Choose the type of build : NONE DEBUG RELEASE RELWITHDEBINFO MINSIZEREL."
    FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

IF(APPLE)
  message(STATUS "Building for APPLE")
  LINK_DIRECTORIES(${FLTK_LIBRARY_DIRS} /opt/local/lib)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
# execute_process(COMMAND fltk-config --use-gl --use-glut --use-forms --ldflags
#                OUTPUT_VARIABLE FLTK_LINK)
  SET(CMAKE_CXX_LINK_FLAGS "-lc++ -lSystem -framework OpenGL -framework GLUT")
ENDIF(APPLE)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
	set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)

set(prefix          ${CMAKE_INSTALL_PREFIX})
set(exec_prefix     ${CMAKE_INSTALL_PREFIX})
set(libdir          ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
set(includedir      ${CMAKE_INSTALL_PREFIX}/include)
set(PACKAGE_VERSION ${LIBFLEWS_VERSION})
set(LIBS            "-L${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR} -lflews -lm -ldl")
set(Cflags          "-I${includedir} -std=c99")
set(Cxxflags        "-I${includedir} -std=c++11")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libflews.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}.pc @ONLY)
_flews_generate_symlink_code(FLEWS_PC_INSTALL_CODE
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}.pc
  ${CMAKE_CURRENT_BINARY_DIR}/libflews.pc)
install(CODE ${FLEWS_PC_INSTALL_CODE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(src)

