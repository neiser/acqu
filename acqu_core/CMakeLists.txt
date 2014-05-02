cmake_minimum_required (VERSION 2.6)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/subdir-header.cmake)
message(STATUS "Configuring acqu_core...")

# the following feature helps us to get rid of
# getenv("acqu"), getenv("acqu_sys"), getenv("ROOTSYS")
# because we define this variables now at "configure" time!
set(ACQU_CONFIG_FILE "${CMAKE_CURRENT_BINARY_DIR}/AcquRoot/src/CMakeConfig.h")
configure_file("${CMAKE_SOURCE_DIR}/cmake/CMakeConfig.h.in" "${ACQU_CONFIG_FILE}")

# add a precompiler flag to indicate that 
# actual VME access is enabled (this allows running AcquDAQ on non-VME machines)
site_name(MY_HOSTNAME)
if(MY_HOSTNAME MATCHES "^vme-")
  add_definitions(-DVME_HOST)
endif()

# use all "src" subfolders as include directories
# this is also used by ROOT_PREPARE_LIB as arguments to rootcint 
file(GLOB ALLSRCDIRS "*/src")
include_directories(${ALLSRCDIRS} "${CMAKE_CURRENT_BINARY_DIR}/AcquRoot/src")

# This builds automagically all the ROOT-enabled libraries
macro(CompileCoreLib MOD)
  # this is standard, except that we change the naming
  # of the target. all the magic happens in ROOT_PREPARE_LIB
  ROOT_PREPARE_LIB("${MOD}/src" "${MOD}/src" "${MOD}/src"
    "${MOD}Dict.cc" SRCS EXESRCS)
  add_library("lib${MOD}" SHARED ${SRCS} ${ACQU_CONFIG_FILE})

  set_target_properties("lib${MOD}"
    PROPERTIES COMPILE_FLAGS "${ARGN}" LIBRARY_OUTPUT_NAME ${MOD})
  target_link_libraries("lib${MOD}" ${ROOT_LIBRARIES} ${EXTRA_LIBS})
  # track this library for the following exe's
  list(APPEND CORE_LIBS "lib${MOD}")
  list(APPEND CORE_EXESRCS ${EXESRCS})
endmacro()

# some variables to track and configure
# the following macro calls
set(EXTRA_LIBS EG Foam)
set(CORE_EXESRCS)
set(CORE_LIBS)

# treat ACQU_DAQ a bit special to increase portability
if(ACQU_DAQ)
  list(APPEND EXTRA_LIBS rt)
  CompileCoreLib(AcquDAQ "-O0") # AcquDAQ must not be optimized (hardware access gets scrambled)  
else()
  add_definitions(-DNO_ACQU_DAQ)
endif()

# those core libs are always required,
# but they depend on NO_ACQU_DAQ to be defined!
CompileCoreLib(AcquRoot)
CompileCoreLib(AcquMC)


# EPICS/libezca is only included in DAQ mode and if EPICS is found
# if DAQ, look for EPICS
# if not found, no support is built in, thus is not required here
if(ACQU_DAQ)
  find_package(EPICS)
  # this sets many EPICS_* variables used in acqu_core
  if(EPICS_FOUND)
    include_directories(${EPICS_INCLUDES})
    link_directories(${EPICS_LIBRARY_DIR})
    add_definitions(-DACQUROOT_EPICS)
    list(APPEND EXTRA_LIBS ${EPICS_LIBRARIES})
    CompileCoreLib(ezcaRoot)
  endif()
endif()


# install some scripts to the bin directory
# by creating symlinks
file(GLOB CORE_EXTRA_SCRIPTS "scripts/*")
foreach(f ${CORE_EXTRA_SCRIPTS})
  get_filename_component(f_name ${f} NAME)
  add_custom_target(link_${f_name} ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${f}" "${EXECUTABLE_OUTPUT_PATH}/${f_name}")
endforeach()

# now build the binaries (but only if DAQ mode is on!),
# the libs EG and Foam are yet another ROOT libraries
if(ACQU_DAQ)
  foreach(exesrc ${CORE_EXESRCS})
    # guess the executable name from the source file.
    # Remove leading "Main" (if any) and trailing ".cc"
    # but not for MainHV which builds to AcquHV...args!
    get_filename_component(EXENAME ${exesrc} NAME_WE)
    STRING(REGEX REPLACE "^MainHV$" "AcquHV" EXENAME "${EXENAME}")
    STRING(REGEX REPLACE "^Main(.+)" "\\1" EXENAME "${EXENAME}")
    
    # add the exe as target, only in DAQ mode
    # so targets here do not to interfere with
    # targets in acqu_user
    add_executable(${EXENAME} ${exesrc})
    set_target_properties(${EXENAME}
      PROPERTIES OUTPUT_NAME ${EXENAME})
    target_link_libraries(${EXENAME} ${CORE_LIBS})
  endforeach()
else()
  # in non-DAQ mode, just track the incs/libs for acqu_user
  SET_ACQU_USER_VARS("${CORE_LIBS}")
endif()



