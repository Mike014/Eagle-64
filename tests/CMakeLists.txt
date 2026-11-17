add_executable(test_fixed_point unit/test_fixed_point.c)
target_include_directories(test_fixed_point PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_fixed_point PRIVATE eagle_fixed)
add_test(NAME fixed_point COMMAND test_fixed_point)

add_executable(test_fixed_point_diff unit/test_fixed_point_diff.c)
target_include_directories(test_fixed_point_diff PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(test_fixed_point_diff PRIVATE eagle_fixed)
add_test(NAME fixed_point_diff COMMAND test_fixed_point_diff)

# Benchmark: compilato ma non registrato in CTest, si lancia a mano.
add_executable(bench_fixed_point bench/bench_fixed_point.c)
target_link_libraries(bench_fixed_point PRIVATE eagle_fixed)
