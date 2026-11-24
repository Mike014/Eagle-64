add_library(eagle_sim_lib STATIC
    lander.cpp
    config.cpp
    simulation.cpp)
target_include_directories(eagle_sim_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(eagle_sim main.cpp)
target_link_libraries(eagle_sim PRIVATE eagle_sim_lib)
