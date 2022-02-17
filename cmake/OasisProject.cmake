function(OasisProject projectName)
    project(Oasis)

    set(Engine_VERSION_MAJOR 2)
    set(Engine_VERSION_MINOR 0)

    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

    add_library(Oasis SHARED)
    if (WIN32)
    add_executable(${projectName} WIN32)
    else()
    add_executable(${projectName})
    endif()

    target_link_libraries(Oasis Opengl32.lib)

    target_link_libraries(${projectName} Oasis)
    target_include_directories(${projectName} PRIVATE Oasis/src)

    set_property(TARGET Oasis PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${projectName} PROPERTY CXX_STANDARD 17)

    target_compile_definitions(Oasis PRIVATE BUILD_DLL)

    # PLATFORM DEFINITIONS AND SETTINGS
    if (WIN32)
    target_compile_definitions(Oasis PRIVATE PLATFORM_WINDOWS)
    target_compile_definitions(${projectName} PRIVATE PLATFORM_WINDOWS)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${projectName})
    set_property(TARGET ${projectName} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
    endif()

    add_subdirectory(${OASIS_PATH})

    add_subdirectory(${OASIS_PATH}/vendor/rapidyaml)
    target_link_libraries(Oasis ryml::ryml)
endfunction(OasisProject)