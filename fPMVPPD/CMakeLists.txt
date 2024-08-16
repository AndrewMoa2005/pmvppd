cmake_minimum_required(VERSION 3.5)

project(fPMVPPD VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    main.cxx
    engine_pmv_ppd.c
    engine_pmv_ppd.h
    widget.cxx
    widget.hxx
    resource/mwidget.cxx
    resource/mwidget.hxx
    resource.h
    res.rc
)

if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") AND ( NOT MSVC))
    # set(STATIC_LINK_LIB "-Wl,-Bstatic -lc++ -lc++abi -Wl,-Bdynamic")
    set(STATIC_LINK_LIB -static c++ c++abi -dynamic)
elseif(MINGW)
    # set(STATIC_LINK_LIB "-static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
    set(STATIC_LINK_LIB -static gcc stdc++ winpthread -dynamic)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/resource
)

find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIR})

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
elseif(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${FLTK_LIBRARIES}
    ${STATIC_LINK_LIB}
)
