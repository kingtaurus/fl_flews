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

#find_package(FLTK REQUIRED)
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

#flews.pc (to configure the library)

#${FLTK_LIBRARIES}
#${FLTK_HEADERS}

#prefix=@prefix@
#exec_prefix=@exec_prefix@
#libdir=@libdir@
#includedir=@includedir@
#Name: flews
#Description: A set of extended Widgets for the excellent FLTK library
#Version: @PACKAGE_VERSION@
#Libs: -L${libdir} -lflews
#Cflags: -I${includedir}/FL

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libflews.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}.pc @ONLY)
_flews_generate_symlink_code(FLEWS_PC_INSTALL_CODE
  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}.pc
  ${CMAKE_CURRENT_BINARY_DIR}/libflews.pc)
install(CODE ${FLEWS_PC_INSTALL_CODE})

#http://www.opensource.apple.com/source/X11libs/X11libs-60/libpng/libpng-1.5.1/libpng-config.in?txt
#configure_file(${CMAKE_CURRENT_SOURCE_DIR}/libflews-config.in
#  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}-config @ONLY)
#_flews_generate_symlink_code(FLEWS_PC_INSTALL_CODE
#  ${CMAKE_CURRENT_BINARY_DIR}/${LIBFLEWS_NAME}-config
#  ${CMAKE_CURRENT_BINARY_DIR}/libflews-config)
#install(CODE ${FLEWS_CONFIG_INSTALL_CODE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(src)





