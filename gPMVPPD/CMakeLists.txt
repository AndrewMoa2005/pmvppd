cmake_minimum_required(VERSION 3.5)

project(gPMVPPD VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

set(SRC_LIST 
    main.c
    engine_pmv_ppd.h
    engine_pmv_ppd.c
    widget.h
    widget.c
    res.rc
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk4)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(${PROJECT_NAME}
    ${SRC_LIST}
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(MSVC)
    target_link_options(${PROJECT_NAME} PRIVATE "/SUBSYSTEM:WINDOWS""/ENTRY:mainCRTStartup")
else()
    target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
endif()

target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})
