set(FUZZ_TARGETS 
    hash_fuzzer
    string_fuzzer
)

foreach(TARGET ${FUZZ_TARGETS})
    add_executable("${TARGET}" "${TARGET}.cpp")
    # Apply the determined flags
    set_target_properties(${TARGET} PROPERTIES
        COMPILE_FLAGS "${FUZZING_COMPILE_FLAGS}"
        LINK_FLAGS "${FUZZING_LINK_FLAGS}"
    )
    # Link QuantLib and any other necessary libraries
    target_link_libraries(${TARGET} PRIVATE etl::etl)
    target_sources(${TARGET} PRIVATE etl_profile.h)
endforeach()
