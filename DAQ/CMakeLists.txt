set ( MODULE_NAME DAQ )
message ( STATUS "Configuring ${MODULE_NAME} ..." )

# Use CONFIGURE_DEPENDS to automatically track changes in source files
file ( GLOB MODULE_SOURCES CONFIGURE_DEPENDS src/*.cc )

# Build shared library
add_library ( ${MODULE_NAME} SHARED
    ${MODULE_SOURCES}
)

# Include paths
target_include_directories ( ${MODULE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${ROOT_INCLUDE_DIRS}
)

if ( CUPDAQ_ENABLE_HDF5 )
    target_include_directories ( ${MODULE_NAME} PUBLIC ${HDF5_INCLUDE_DIRS} )
endif ()

# Link dependencies and inherit global flags
target_link_libraries ( ${MODULE_NAME}
    PUBLIC
        yaml-cpp
        cppzmq
        ${ROOT_LIBRARIES}
        OnlConsts
        Notice
        DAQUtils
        DAQSystem
        DAQConfig
        DAQTrigger
        OnlObjs
        RawObjs
        OnlHistogramer
)

if ( CUPDAQ_ENABLE_HDF5 )
    target_link_libraries ( ${MODULE_NAME} PUBLIC ${HDF5_LIBRARIES} HDF5Utils )
endif ()

target_compile_definitions ( ${MODULE_NAME}
    PUBLIC
        ${ROOT_DEFINITIONS}
)

# ---- Install ----
install ( TARGETS ${MODULE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install ( DIRECTORY ${MODULE_NAME}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${MODULE_NAME}
    FILES_MATCHING PATTERN "*.hh"
)
file ( GLOB DAQ_SCRIPTS "scripts/*" )

install ( PROGRAMS ${DAQ_SCRIPTS}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Runtime )

add_subdirectory ( test )