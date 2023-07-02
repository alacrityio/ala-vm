# ##################################################################################################
# Create the ala-vm interpreter driver.
# ##################################################################################################
add_executable(ala-vm-interp ${CMAKE_CURRENT_SOURCE_DIR}/interp.cpp)
target_link_libraries(ala-vm-interp ala-vm)

add_executable(bench-interp ${CMAKE_CURRENT_SOURCE_DIR}/bench_interp.cpp)
target_link_libraries(bench-interp ala-vm)

add_executable(hello-driver ${CMAKE_CURRENT_SOURCE_DIR}/hello_driver.cpp)
target_link_libraries(hello-driver ala-vm)

add_executable(ala-vm-addr2line addr2line.cpp)
target_link_libraries(ala-vm-addr2line ala-vm)
