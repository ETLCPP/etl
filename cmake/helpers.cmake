function(determine_version VER_FILE_NAME)
    file(READ ${VER_FILE_NAME} VERSION_FROM_FILE)
    string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${VERSION_FROM_FILE})
    if(NOT CMAKE_MATCH_1)
        message(WARNING
            "Major version could not be determined, "
            "version.txt might have invalid format"
        )
        return()
    endif()

    set(VER_MAJOR ${CMAKE_MATCH_1})

    string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${VERSION_FROM_FILE})
    if(NOT CMAKE_MATCH_1)
        message(WARNING
            "Minor version could not be determined, "
            "version.txt might have invalid format"
        )
        return()
    endif()
    set(VER_MINOR ${CMAKE_MATCH_1})

    string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${VERSION_FROM_FILE})
    if(NOT CMAKE_MATCH_1)
        message(WARNING
            "Patch version could not be determined, "
            "version.txt might have invalid format"
        )
        return()
    endif()
    set(VER_PATCH ${CMAKE_MATCH_1})

    set(ETL_VERSION ${VER_MAJOR}.${VER_MINOR}.${VER_PATCH} CACHE STRING
        "ETL version determined from version.txt")
    message(STATUS "Determined ETL version ${ETL_VERSION} from version.txt file")
endfunction()
