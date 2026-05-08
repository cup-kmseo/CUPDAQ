set ( MODULE_NAME DAQSystem )
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
)

target_link_libraries ( ${MODULE_NAME}
    PUBLIC
        OnlConsts
        DAQUtils
        Notice
        DAQConfig
)

# ---- Install ----
install ( TARGETS ${MODULE_NAME}
    EXPORT CUPDAQTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install ( DIRECTORY inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hh"
)