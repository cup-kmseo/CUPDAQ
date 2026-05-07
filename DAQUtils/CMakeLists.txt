# ------------------------------------------------------------
# DAQUtils - Header-only utility library
# ------------------------------------------------------------
set ( MODULE_NAME DAQUtils )
message ( STATUS "Configuring ${MODULE_NAME} ..." )

file ( COPY ${CMAKE_CURRENT_SOURCE_DIR}/inc/
     DESTINATION ${CUPDAQ_HEADER_OUTPUT_PATH}
     FILES_MATCHING PATTERN "*.hh" )

add_library ( ${MODULE_NAME} INTERFACE )

target_include_directories ( ${MODULE_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install ( DIRECTORY inc/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hh"
)