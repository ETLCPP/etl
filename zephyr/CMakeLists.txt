add_subdirectory_ifdef(CONFIG_ETL ${CMAKE_CURRENT_LIST_DIR}/.. etl)
zephyr_link_interface_ifdef(CONFIG_ETL etl)
