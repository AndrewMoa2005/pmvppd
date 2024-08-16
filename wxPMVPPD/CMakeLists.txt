cmake_minimum_required(VERSION 3.0)

project(wxPMVPPD VERSION 1.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    main.cpp
    widget.hpp
    widget.cpp
    engine_pmv_ppd.c
    engine_pmv_ppd.h
    res.rc
    resource/mwidget.h
    resource/mwidget.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/resource
)

find_package(wxWidgets REQUIRED COMPONENTS base core)
#find_package(wxWidgets REQUIRED)
include(${wxWidgets_USE_FILE})

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(MSVC)
#    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS" "/ENTRY:mainCRTStartup")
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS")
elseif(WIN32)
    target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
endif()

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${wxWidgets_LIBRARIES}
)
