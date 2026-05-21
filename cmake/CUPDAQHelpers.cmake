# ------------------------------------------------------------
# CUPDAQHelpers.cmake
# Platform detection, privilege detection, and auto-install helpers.
# Included by the top-level CMakeLists.txt; sets the following variables:
#   CUPDAQ_IS_ROOT, CUPDAQ_IS_UBUNTU, CUPDAQ_OS_ID
#   CUPDAQ_BASH/APT/DNF/YUM/SNAP_EXECUTABLE, CUPDAQ_DEBIAN_PKG_EXECUTABLE
# Defines functions:
#   cupdaq_install_system_packages(<pkg>...)
#   cupdaq_install_system_packages_snap(<pkg>...)
# ------------------------------------------------------------

find_program ( CUPDAQ_BASH_EXECUTABLE bash )
find_program ( CUPDAQ_APT_EXECUTABLE apt )
find_program ( CUPDAQ_APT_GET_EXECUTABLE apt-get )
find_program ( CUPDAQ_DNF_EXECUTABLE dnf )
find_program ( CUPDAQ_YUM_EXECUTABLE yum )
find_program ( CUPDAQ_SNAP_EXECUTABLE snap )

# ---- OS / distro detection ----
if ( EXISTS "/etc/os-release" )
  file ( READ "/etc/os-release" _cupdaq_os_release )
  string ( REGEX MATCH "^ID=(.+)$" _cupdaq_os_id_line "${_cupdaq_os_release}" )
  if ( _cupdaq_os_id_line )
    string ( REGEX REPLACE "^ID=(.+)$" "\\1" CUPDAQ_OS_ID "${_cupdaq_os_id_line}" )
    string ( STRIP "${CUPDAQ_OS_ID}" CUPDAQ_OS_ID )
    string ( REPLACE "\"" "" CUPDAQ_OS_ID "${CUPDAQ_OS_ID}" )
  else ()
    set ( CUPDAQ_OS_ID "" )
  endif ()
else ()
  set ( CUPDAQ_OS_ID "" )
endif ()

if ( CUPDAQ_APT_EXECUTABLE )
  set ( CUPDAQ_DEBIAN_PKG_EXECUTABLE ${CUPDAQ_APT_EXECUTABLE} )
elseif ( CUPDAQ_APT_GET_EXECUTABLE )
  set ( CUPDAQ_DEBIAN_PKG_EXECUTABLE ${CUPDAQ_APT_GET_EXECUTABLE} )
else ()
  set ( CUPDAQ_DEBIAN_PKG_EXECUTABLE "" )
endif ()

if ( CUPDAQ_OS_ID STREQUAL "ubuntu" )
  set ( CUPDAQ_IS_UBUNTU TRUE )
else ()
  set ( CUPDAQ_IS_UBUNTU FALSE )
endif ()

# ---- Privilege detection ----
if ( UNIX AND NOT APPLE )
  execute_process ( COMMAND id -u OUTPUT_VARIABLE _CUPDAQ_UID OUTPUT_STRIP_TRAILING_WHITESPACE )
  if ( _CUPDAQ_UID STREQUAL "0" )
    set ( CUPDAQ_IS_ROOT TRUE )
  else ()
    set ( CUPDAQ_IS_ROOT FALSE )
  endif ()
else ()
  set ( CUPDAQ_IS_ROOT FALSE )
endif ()

# ---- Auto-install helpers ----

function ( cupdaq_install_system_packages )
  set ( _pkg_list ${ARGN} )
  list ( JOIN _pkg_list " " _pkg_str )

  if ( NOT CUPDAQ_IS_ROOT )
    message ( WARNING "CUPDAQ is not running as root; cannot auto-install system packages." )
    return ()
  endif ()

  if ( CUPDAQ_IS_UBUNTU AND CUPDAQ_DEBIAN_PKG_EXECUTABLE )
    if ( NOT CUPDAQ_BASH_EXECUTABLE )
      message ( WARNING "bash not found; cannot execute apt install helper." )
      return ()
    endif ()
    message ( STATUS "Attempting to install system packages with ${CUPDAQ_DEBIAN_PKG_EXECUTABLE}: ${_pkg_str}" )
    execute_process (
      COMMAND ${CUPDAQ_BASH_EXECUTABLE} -lc
        "DEBIAN_FRONTEND=noninteractive ${CUPDAQ_DEBIAN_PKG_EXECUTABLE} update -y && DEBIAN_FRONTEND=noninteractive ${CUPDAQ_DEBIAN_PKG_EXECUTABLE} install -y ${_pkg_str}"
      RESULT_VARIABLE _result
      OUTPUT_VARIABLE _output
      ERROR_VARIABLE  _error
    )
    if ( NOT _result EQUAL 0 )
      message ( WARNING "${CUPDAQ_DEBIAN_PKG_EXECUTABLE} install failed: ${_error}" )
    endif ()
    return ()
  endif ()

  if ( CUPDAQ_DNF_EXECUTABLE OR CUPDAQ_YUM_EXECUTABLE )
    if ( CUPDAQ_DNF_EXECUTABLE )
      set ( _pkg_manager ${CUPDAQ_DNF_EXECUTABLE} )
    else ()
      set ( _pkg_manager ${CUPDAQ_YUM_EXECUTABLE} )
    endif ()
    message ( STATUS "Attempting to install system packages with ${_pkg_manager}: ${_pkg_str}" )
    execute_process (
      COMMAND ${_pkg_manager} install -y ${_pkg_list}
      RESULT_VARIABLE _result
      OUTPUT_VARIABLE _output
      ERROR_VARIABLE  _error
    )
    if ( NOT _result EQUAL 0 )
      message ( WARNING "${_pkg_manager} install failed: ${_error}" )
    endif ()
    return ()
  endif ()

  message ( WARNING "No supported package manager found for auto-installation." )
endfunction ()

function ( cupdaq_install_system_packages_snap )
  set ( _pkg_list ${ARGN} )
  list ( JOIN _pkg_list " " _pkg_str )

  if ( NOT CUPDAQ_IS_ROOT )
    message ( WARNING "CUPDAQ is not running as root; cannot auto-install snap packages." )
    return ()
  endif ()

  if ( NOT CUPDAQ_SNAP_EXECUTABLE )
    message ( WARNING "snap not found; cannot install snap packages." )
    return ()
  endif ()

  message ( STATUS "Attempting to install snap packages with snap: ${_pkg_str}" )
  execute_process (
    COMMAND ${CUPDAQ_SNAP_EXECUTABLE} install ${_pkg_str} --classic
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _output
    ERROR_VARIABLE  _error
  )
  if ( NOT _result EQUAL 0 )
    message ( WARNING "snap install failed: ${_error}" )
  endif ()
endfunction ()
