set ( MODULE_NAME DAQ )
message ( STATUS "Configuring ${MODULE_NAME} ..." )

file ( COPY ${CMAKE_CURRENT_SOURCE_DIR}/inc/
     DESTINATION ${CUPDAQ_HEADER_OUTPUT_PATH}
     FILES_MATCHING PATTERN "*.hh" )

file ( GLOB MODULE_SOURCES CONFIGURE_DEPENDS src/*.cc )

add_library ( ${MODULE_NAME} SHARED
    ${MODULE_SOURCES}
)

target_include_directories ( ${MODULE_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        ${ROOT_INCLUDE_DIRS}
)

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

if ( TARGET HDF5Utils )
    target_link_libraries ( ${MODULE_NAME} PUBLIC HDF5Utils )
endif ()

target_compile_definitions ( ${MODULE_NAME}
    PUBLIC
        ${ROOT_DEFINITIONS}
)

# ---- Install ----
install ( TARGETS ${MODULE_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install ( DIRECTORY inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hh"
)
file ( GLOB DAQ_SCRIPTS "scripts/*" )

install ( PROGRAMS ${DAQ_SCRIPTS}
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        COMPONENT Runtime )

add_subdirectory ( test )