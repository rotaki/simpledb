
###############################################################################
# HEADER project definition
###############################################################################

cmake_minimum_required(VERSION 3.16)
set(SMARTDB_DESCRIPTION "simpledb is a tiny relational DBMS for experiment")
project(
  simpledb
  VERSION 1.0.0.0
  DESCRIPTION "${SMARTDB_DESCRIPTION}"
  )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER_ID "Clang")
set(CMAKE_CXX_COMPILER_ID "Clang")

###############################################################################
# HEADER safety checks
###############################################################################

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" PATH_TO_CMAKELISTS_TXT)
if (EXISTS "${PATH_TO_CMAKELISTS_TXT}")
  message(FATAL_ERROR "Run CMake from a build subdirectory \"mkdir build; cd build; cmake ..\" \
Some junk files were created in this folder (CMakeCache.txt, CMakeFiles); you should delete those.")
endif()

###############################################################################
# HEADER System info
###############################################################################

message(STATUS
  "Welcome to simpledb \n\
Version: ${PROJECT_VERSION}
")

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSOIN}")
message(STATUS "System: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")

function(print_sys_info QUERY_TARGET)
  cmake_host_system_information(RESULT SYS_INFO QUERY ${QUERY_TARGET})
  message(STATUS "System: ${QUERY_TARGET}: ${SYS_INFO}")
endfunction()

print_sys_info("NUMBER_OF_LOGICAL_CORES;NUMBER_OF_PHYSICAL_CORES")
print_sys_info("HOSTNAME;FQDN")
print_sys_info("AVAILABLE_VIRTUAL_MEMORY;TOTAL_VIRTUAL_MEMORY")
print_sys_info("AVAILABLE_PHYSICAL_MEMORY;TOTAL_PHYSICAL_MEMORY")
print_sys_info("IS_64BIT;HAS_IA64")
print_sys_info("HAS_FPU;HAS_MMX;HAS_MMX_PLUS")
print_sys_info("HAS_SSE;HAS_SSE2;HAS_SSE_FP;HAS_SSE_MMX")
print_sys_info("HAS_AMD_3DNOW;HAS_AMD_3DNOW_PLUS")
print_sys_info("HAS_SERIAL_NUMBER;PROCESSOR_SERIAL_NUMBER")
print_sys_info("PROCESSOR_NAME;PROCESSOR_DESCRIPTION")
print_sys_info("OS_NAME;OS_RELEASE;OS_VERSION;OS_PLATFORM")

###############################################################################
# HEADER CMake options and global variables
# CMake build types, specify with -DCMAKE_BUILD_TYPE={option}
#  Debug (default), Release, RelWithDebInfo, FastDebug.
#  In practice people only use Debug or Release
#
# CMake options, specify with -DSMARTDB_{option}=ON.
#  LOG_LEVEL: Enable logging. Default 6.
###############################################################################

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

set(LOG_LEVEL
  "1"
  CACHE STRING "Log level. 0(off) 1(error) 2(warn) 3(into) 4(debug) 5(trace) 6(all)")

set(BUILD_SUPPORT_DIR "${CMAKE_SOURCE_DIR}/build_support")
# set(BUILD_SUPPORT_DATA_DIR "${BUILD_SUPPORT_DIR}/data")

# Everything else in this section will populate the following global variables.
set(SMARTDB_COMPILE_OPTIONS "")
set(SMARTDB_LINK_LIBRARIES "")
set(SMARTDB_LINK_OPTIONS "")
set(SMARTDB_INCLUDE_DIRECTORIES "")

# Add compilation flags to SMARTDB_COMPILE_OPTIONS based on the current CMAKE_BUILD_TYPE.
string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "DEBUG")
    list(APPEND SMARTDB_COMPILE_OPTIONS "-ggdb" "-O0" "-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "FASTDEBUG")
    list(APPEND SMARTDB_COMPILE_OPTIONS "-ggdb" "-O1" "-fno-omit-frame-pointer" "-fno-optimize-sibling-calls")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")
    list(APPEND SMARTDB_COMPILE_DEFINITIONS "-DNDEBUG")
    list(APPEND SMARTDB_COMPILE_OPTIONS "-O3")
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RELWITHDEBINFO")
    list(APPEND SMARTDB_COMPILE_DEFINITIONS "-DNDEBUG")
    list(APPEND SMARTDB_COMPILE_OPTIONS "-ggdb" "-O2")
else ()
    message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
endif ()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

#log
list(APPEND SMARTDB_COMPILE_DEFINITIONS "-DLOG_LEVEL=${LOG_LEVEL}")
if ("${LOG_LEVEL}" STREQUAL "0")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (NONE)")
elseif ("${LOG_LEVEL}" STREQUAL "1")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (ERROR)")
elseif ("${LOG_LEVEL}" STREQUAL "2")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (WARN)")
elseif ("${LOG_LEVEL}" STREQUAL "3")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (INFO)")
elseif ("${LOG_LEVEL}" STREQUAL "4")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (DEBUG)")
elseif ("${LOG_LEVEL}" STREQUAL "5")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (TRACE)")
elseif ("${LOG_LEVEL}" STREQUAL "6")
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (ALL)")
else()
  message(STATUS "LOG_LEVEL: ${LOG_LEVEL} (UNKWN)")
endif()

###############################################################################
#                            Download dependencies                            #
###############################################################################

# Some libraries just won't shut up, so this redefines the message function.
set(MESSAGE_QUIET OFF)
function(message)
  list(GET ARGV 0 MessageType)
  if (NOT MESSAGE_QUIET OR
      MessageType STREQUAL FATAL_ERROR OR
      MessageType STREQUAL SEND_ERROR OR
      MessageType STREQUAL WARNING OR
      MessageType STREQUAL AUTHOR_WARNING)
    list(GET ARGV 1 Message)
    list(REMOVE_AT ARGV 0)
    _message(${MessageType} "${ARGV}")
  endif ()
endfunction()

include(FetchContent)           # FetchContent_ functions.

function(add_simpledb_dep NAME GIT_URL GIT_TAG)
  string(TOLOWER "${NAME}" NAME_LOWER)
  FetchContent_Declare(${NAME}
    QUIET
    SOURCE_DIR ${CMAKE_BINARY_DIR}/_deps/src/${NAME}
    BINARY_DIR ${CMAKE_BINARY_DIR}/_deps/build/${NAME}
    SUBBUILD_DIR ${CMAKE_BINARY_DIR}/_deps/build/${NAME}
    GIT_REPOSITORY ${GIT_URL}
    GIT_TAG ${GIT_TAG}
    )
  set(MESSAGE_QUIET ON)
  FetchContent_MakeAvailable(${NAME})
  unset(MESSAGE_QUIET)
  # Get the date of the last git commit for the GIT_TAG branch.
  execute_process(COMMAND git log -1 --format=%cd --date=short                        # Get the last commit's date.
    WORKING_DIRECTORY ${${NAME_LOWER}_SOURCE_DIR}                               # From the specified git dir.
    OUTPUT_VARIABLE GIT_LAST_COMMIT)                                            # Save to GIT_LAST_COMMIT.
  string(STRIP "${GIT_LAST_COMMIT}" GIT_LAST_COMMIT)                                  # Remove any trailing newlines.

  message(STATUS "[FOUND] ${NAME} (${GIT_URL} ${GIT_TAG} ${GIT_LAST_COMMIT})")        # Print a success message.
endfunction()

add_simpledb_dep(googletest https://github.com/google/googletest.git release-1.10.0)

###############################################################################
#                             Smartdb Libraries                            #
###############################################################################

file(GLOB_RECURSE
  SMARTDB_SRCS
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/include/*.hpp)

list(REMOVE_ITEM SMARTDB_SRCS ${PROJECT_SOURCE_DIR}/src/main/simpledb.cpp)

# create OBJECT library (used for creating both static and shared library)
add_library(simpledb_objlib OBJECT ${SMARTDB_SRCS})

target_compile_definitions(simpledb_objlib PUBLIC #PUBLIC: all consumers of the library inherit the following
  ${SMARTDB_COMPILE_DEFINITIONS}
  )

target_compile_options(simpledb_objlib PUBLIC
  # "-march=native"               # enable machine-specific instruction sets and optimization
  "-mcx16"                      # allow CMPXCHG16B (16-byte compare and exchange)
  ${SMARTDB_COMPILE_OPTIONS}
  )

# option and definition difference
target_compile_options(simpledb_objlib PRIVATE
  "-fPIC"
  "-Werror"
  "-Wall"
  )

target_compile_features(simpledb_objlib PUBLIC
  cxx_std_17
  )

target_include_directories(simpledb_objlib PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  )


target_include_directories(simpledb_objlib SYSTEM PUBLIC # SYSTEM PUBLIC: inherit without error checking
  ${SMARTDB_INCLUDE_DIRECTORIES}
  )

target_link_options(simpledb_objlib PUBLIC
  ${SMARTDB_LINK_OPTIONS}
  )


# create STATIC library

add_library(simpledb_static STATIC $<TARGET_OBJECTS:simpledb_objlib>)
target_link_libraries(simpledb_static PUBLIC simpledb_objlib)
target_compile_options(simpledb_static PUBLIC
  "-fvisibility=hidden"
  )
target_link_options(simpledb_static PUBLIC
  "-fvisibility=hidden"
  )

#create SHARED library

add_library(simpledb_shared SHARED $<TARGET_OBJECTS:simpledb_objlib>)
target_link_libraries(simpledb_shared PUBLIC simpledb_objlib)

###############################################################################
#                                 Main binary                                 #
###############################################################################

add_executable(simpledb ./app/main.cpp)
target_compile_options(simpledb PRIVATE "-Werror" "-Wall")
target_link_libraries(simpledb simpledb_static)
set_target_properties(simpledb PROPERTIES CXX_EXTENTIONS OFF ENABLE_EXPORTS ON)

###############################################################################
#                                     Test                                    #
###############################################################################

include(GoogleTest)             # use gtest_add_tests

file(GLOB_RECURSE
  SMARTDB_TEST_SRCS
  ${PROJECT_SOURCE_DIR}/test/*.cpp
  )

add_executable(simpledb_test ${SMARTDB_TEST_SRCS})
target_link_libraries(simpledb_test PRIVATE simpledb_shared gtest)

gtest_add_tests(TARGET simpledb_test)

###############################################################################
#                         Generated file destinations                         #
###############################################################################

set_target_properties(
  simpledb_static
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

set_target_properties(
  simpledb_shared
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

set_target_properties(
  simpledb
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )

set_target_properties(
  simpledb_test
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
  )


# ###############################################################################
# #                                    Linter                                   #
# ###############################################################################

# file(GLOB_RECURSE LINT_FILES
#   "${PROJECT_SOURCE_DIR}/app/*.cpp"
#   "${PROJECT_SOURCE_DIR}/app/*.hpp"
#   "${PROJECT_SOURCE_DIR}/src/*.cpp"
#   "${PROJECT_SOURCE_DIR}/include/*.hpp"
#   "${PROJECT_SOURCE_DIR}/test/*.hpp"
#   "${PROJECT_SOURCE_DIR}/test/*.cpp"
#   )

# find_program(CPPLINT_BIN NAMES cpplint.py HINTS ${BUILD_SUPPORT_DIR})
# if ("${CPPLINT_BIN}" STREQUAL "CPPLINT_BIN-NOTFOUND")
#   message(STATUS "[MISSING] cpplint not found, no cpplint")
# else()
#   add_custom_target(check-lint
#     COMMENT "Running: cpplint"
#     COMMAND echo '${LINT_FILES}' | xargs -n12 -P8
#     python3 ${CPPLINT_BIN}
#     --verbose=2 --linelength=120 --quiet
#     --filter=-legal/copyright,-build/header_guard
#     USES_TERMINAL
#     )
#   message(STATUS "[ADDED] check-lint (${CPPLINT_BIN})")
# endif()
# unset(${CPPLINT_BIN})


###############################################################################
#                                 Clang-format                                #
###############################################################################

find_program(CLANG_FORMAT_BIN clang-format)
if ("${CLANG_FORMAT_BIN}" STREQUAL "CLANG_FORMAT_BIN-NOTFOUND")
  message(STATUS "[MISSING] clang-format not found, no check-format")
else()
  string(CONCAT FORMAT_DIRS
    "${CMAKE_CURRENT_SOURCE_DIR}/app "
    "${CMAKE_CURRENT_SOURCE_DIR}/src "
    "${CMAKE_CURRENT_SOURCE_DIR}/include "
    "${CMAKE_CURRENT_SOURCE_DIR}/test"
    )

  # target format is only build when `make format` is issued
  add_custom_target(format
    COMMENT "Running: format"
    COMMAND ${BUILD_SUPPORT_DIR}/run-clang-format.py
    --clang-format-executable ${CLANG_FORMAT_BIN}
    -r ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    -i                          # format file instead of printing diff
    -q
    USES_TERMINAL
    )

  add_custom_target(check-format ALL
    COMMENT "Running: check-format"
    COMMAND ${BUILD_SUPPORT_DIR}/run-clang-format.py
    --clang-format-executable ${CLANG_FORMAT_BIN}
    -r ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/test
    USES_TERMINAL
    )

  message(STATUS "[ADDED] format and check-format (${CLANG_FORMAT_BIN})")
  unset(FORMAT_DIRS)
  
endif()
unset(CLANG_FORMAT_BIN)

###############################################################################
#                                  Clang-tidy                                 #
###############################################################################

find_program(CLANG_TIDY_BIN clang-tidy)
if ("${CLANG_TIDY_BIN}" STREQUAL "CLANG_TIDY_BIN-NOTFOUND")
  message(STATUS "[MISSING] clang-tidy not found, no check-clang-tidy")
else()
  # target clang-tidy is only build when `make clang-tidy` is issued
  add_custom_target(check-tidy
    COMMENT "Running clang-tidy"
    COMMAND ${BUILD_SUPPORT_DIR}/run-clang-tidy.py
    -clang-tidy-binary ${CLANG_TIDY_BIN}
    -p ${CMAKE_BINARY_DIR}
    USES_TERMINAL)
  # gtest is issued before check-clang-tidy
  add_custom_command(TARGET check-tidy DEPENDS gtest)
  message(STATUS "[ADDED] check-tidy (${CLANG_TIDY_BIN})")
endif()
unset(CLANG_TIDY_BIN)

###############################################################################
#             Apply +x permissions to all scripts in build_support            #
###############################################################################

file(GLOB_RECURSE
        BUILD_SUPPORT_SCRIPTS
        CONFIGURE_DEPENDS
        ${BUILD_SUPPORT_DIR}/*.py
        )

foreach (_var IN LISTS BUILD_SUPPORT_SCRIPTS)
    execute_process(COMMAND chmod +x "${_var}")
endforeach ()
