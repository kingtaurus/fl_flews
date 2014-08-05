
include_directories(FL/)

#set(libflews_public_headers
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Ball.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_flews.h
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Multi_Value_Slider.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Pack_2D.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Quaternion.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Spin.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Value_Input_Spin.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/Fl_Value_Slider_Input.H
#  ${PROJECT_SOURCE_DIR}/inc/FL/gl_image.hpp
#)

set(libflews_sources
	Fl_Ball.cpp
	Fl_Multi_Value_Slider.cpp
	Fl_Quaternion.cpp
	Fl_Spin.cpp
	Fl_Value_Input_Spin.cpp
	Fl_Value_Slider_Input.cpp
	gl_image.cpp
)


if(NOT DEFINED FLTK_INCLUDE_DIR)
	find_package(FLTK REQUIRED)
endif(NOT DEFINED FLTK_INCLUDE_DIR)

add_library( flews        SHARED ${libflews_sources} )
TARGET_LINK_LIBRARIES( flews dl ${FLTK_LIBRARY} ${OPEN_GL_LIBRARIES} )

add_library( flews-static STATIC ${libflews_sources} )
SET_TARGET_PROPERTIES( flews-static PROPERTIES OUTPUT_NAME "flews")
SET_TARGET_PROPERTIES( flews-static PROPERTIES PREFIX      "lib")
TARGET_LINK_LIBRARIES( flews-static ${FLTK_LIBRARY})

install(
     TARGETS flews flews-static
     LIBRARY DESTINATION lib COMPONENT flews
     ARCHIVE DESTINATION lib COMPONENT flews
)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/inc/FL DESTINATION include/FL
        COMPONENT headers
        FILES_MATCHING PATTERN "*" )

#add_executable(template_checks template_checks.cpp)
#set_target_properties(template_checks PROPERTIES COMPILE_DEFINITIONS "STAND_ALONE")
#target_link_libraries(template_checks gcov ${Boost_SYSTEM_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} mpfr gmp)

#SUFFIXES = .cpp .fl
#.fl.cpp:
#        fluid -c $<
#convert .fl through fluid


#FLTK_WRAP_UI(testflews_Ui testflews.fl)
#ADD_LIBRARY(testflews_Ui ${testflews_Ui_FLTK_UI_SRCS})

#ADD_EXECUTABLE(testflews testflews.cxx)
#ADD_DEPENDENCIES(testflews testflews_Ui)
#TARGET_LINK_LIBRARIES(testflews testflews_Ui dl flews-static ${FLTK_LIBRARY} ${OPENGL_LIBRARIES})















