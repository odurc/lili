find_package(Threads REQUIRED)
add_mocked_test(lili LINK_LIBRARIES ${LILI_LIBRARY_NAME} Threads::Threads)
