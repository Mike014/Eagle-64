add_library(eagle_fixed STATIC
    fixed/fixed_point.c
    fixed/fixed_point.asm)

target_include_directories(eagle_fixed PUBLIC fixed)

