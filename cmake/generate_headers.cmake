#######################################################################
# Optional generation of ETL's fixed-arity headers using Cog.
#######################################################################
function(etl_generate_headers)
    set(source_directory "${CMAKE_CURRENT_SOURCE_DIR}/include")
    # Validate requested capacities. With no settings, leave the headers untouched.
    set(generation_enabled FALSE)
    foreach(option ETL_NUMBER_OF_HANDLERS ETL_NUMBER_OF_TYPES ETL_IS_ONE_OF)
        if(NOT DEFINED ${option})
            set(${option} "" CACHE STRING
                "ETL generator capacity (empty skips this group unless required by another)")
        endif()
        if(NOT "${${option}}" STREQUAL "")
            if(NOT "${${option}}" MATCHES "^[1-9][0-9]*$" OR ${option} LESS 16)
                message(FATAL_ERROR "${option} must be a decimal integer of at least 16, or empty")
            endif()
            set(generation_enabled TRUE)
        endif()
    endforeach()
    if(generation_enabled)
        # Fill in dependent capacities: handlers <= types <= is_one_of.
        # For example, 32 handlers also require at least 32 types and is_one_of entries.
        # Inferred values stay local instead of being saved in the CMake cache.
        set(required_capacity 16)
        foreach(option ETL_NUMBER_OF_HANDLERS ETL_NUMBER_OF_TYPES ETL_IS_ONE_OF)
            if("${${option}}" STREQUAL "")
                if(required_capacity GREATER 16)
                    set(${option} "${required_capacity}")
                endif()
            elseif(${option} LESS required_capacity)
                message(FATAL_ERROR
                    "${option} must be at least ${required_capacity} to support the configured "
                    "handlers/types. Leave it empty to select a compatible default.")
            else()
                set(required_capacity "${${option}}")
            endif()
        endforeach()

        # Find Python on the build machine, not the cross-compilation target.
        if(CMAKE_VERSION VERSION_LESS 3.12)
            find_package(PythonInterp 3 REQUIRED)
            set(host_python "${PYTHON_EXECUTABLE}")
        else()
            find_package(Python3 REQUIRED COMPONENTS Interpreter)
            set(host_python "${Python3_EXECUTABLE}")
        endif()

        # Create a reusable Python environment without changing the host installation.
        set(venv_directory "${CMAKE_CURRENT_BINARY_DIR}/generated/venv")
        if(CMAKE_HOST_WIN32)
            set(venv_python "${venv_directory}/Scripts/python.exe")
        else()
            set(venv_python "${venv_directory}/bin/python")
        endif()
        if(NOT EXISTS "${venv_python}")
            execute_process(COMMAND "${host_python}" -m venv "${venv_directory}"
                RESULT_VARIABLE result ERROR_VARIABLE error)
            if(NOT "${result}" STREQUAL "0")
                message(FATAL_ERROR
                    "ETL could not create its generator virtual environment. "
                    "Install Python 3's venv support and retry.\n${error}")
            endif()
        endif()

        # Install the pinned Cog version only when it is missing or different.
        execute_process(COMMAND "${venv_python}" -I -m pip show cogapp
            RESULT_VARIABLE result OUTPUT_VARIABLE package ERROR_QUIET)
        if(NOT "${result}" STREQUAL "0" OR NOT package MATCHES "Version: 3[.]6[.]0[\r\n]")
            execute_process(COMMAND "${venv_python}" -I -m pip install
                --disable-pip-version-check cogapp==3.6.0
                RESULT_VARIABLE result OUTPUT_VARIABLE output ERROR_VARIABLE error)
            if(NOT "${result}" STREQUAL "0")
                message(FATAL_ERROR
                    "ETL could not install cogapp==3.6.0 in ${venv_directory}. "
                    "Provide package-index access or an offline wheel via PIP_FIND_LINKS.\n${output}\n${error}")
            endif()
        endif()

        # Generate directly into include/etl/private, just like generate.bat.
        # Match each Cog parameter to its capacity and generator templates.
        set(Handlers_capacity "${ETL_NUMBER_OF_HANDLERS}")
        set(NTypes_capacity "${ETL_NUMBER_OF_TYPES}")
        set(IsOneOf_capacity "${ETL_IS_ONE_OF}")
        set(Handlers_headers fsm_fwd_decl_cpp03 fsm_friend_decl_cpp03 fsm_cpp03
            message_router_cpp03 message_packet_cpp03)
        set(NTypes_headers largest_type_cpp03 largest_alignment_cpp03 largest_cpp03
            smallest_cpp03 type_lookup_cpp03 type_select_cpp03 variant_pool_cpp03)
        set(IsOneOf_headers type_traits_cpp03)
        foreach(parameter Handlers NTypes IsOneOf)
            set(capacity "${${parameter}_capacity}")
            if(NOT "${capacity}" STREQUAL "")
                foreach(header IN LISTS ${parameter}_headers)
                    execute_process(
                        COMMAND "${venv_python}" -I -m cogapp -d -e
                            "-o${source_directory}/etl/private/${header}.h"
                            "-D${parameter}=${capacity}"
                            "${source_directory}/etl/generators/${header}_generator.h"
                        RESULT_VARIABLE result ERROR_VARIABLE error)
                    if(NOT "${result}" STREQUAL "0")
                        message(FATAL_ERROR "ETL generation of ${header}.h failed: ${result}\n${error}")
                    endif()
                endforeach()
            endif()
        endforeach()
    endif()
endfunction()
