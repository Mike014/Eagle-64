add_library(eagle_fixed STATIC
    fixed/fixed_point.c)

target_include_directories(eagle_fixed PUBLIC fixed)

