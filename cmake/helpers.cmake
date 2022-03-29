function(determine_version VER_FILE_NAME)
    file(READ ${VER_FILE_NAME} ETL_VERSION_RAW)
    # Remove trailing whitespaces and/or newline
    string(STRIP ${ETL_VERSION_RAW} ETL_VERSION)
    set(ETL_VERSION ${ETL_VERSION} CACHE STRING
        "ETL version determined from version.txt"
    )
    message(STATUS "Determined ETL version ${ETL_VERSION} from version.txt file")
endfunction()
