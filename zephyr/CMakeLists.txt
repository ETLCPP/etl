if(CONFIG_ETL)
  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. etl)
  zephyr_link_libraries(etl::etl)

  zephyr_compile_definitions_ifdef(CONFIG_ETL_DEBUG ETL_DEBUG)
  zephyr_compile_definitions_ifdef(CONFIG_ETL_CHECK_PUSH_POP ETL_CHECK_PUSH_POP)
  zephyr_compile_definitions_ifdef(CONFIG_ETL_LOG_ERRORS ETL_LOG_ERRORS)
endif()
