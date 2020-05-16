################################################################################
# @file
# @author      Wynand Marais
# @copyright   (c) 2020 Wynand Marais, MIT License
################################################################################
add_library(hawk_comp_c_bitmask OBJECT c_bit_mask.c)
target_compile_options(hawk_comp_c_bitmask PRIVATE -O3)

add_library(hawk_comp_hawk_bitmask OBJECT hawk_bit_mask.cpp)
target_compile_options(hawk_comp_hawk_bitmask PRIVATE -O3)

add_library(hawk_comp_cpp_bitset OBJECT cpp_bitset.cpp)
target_compile_options(hawk_comp_hawk_bitmask PRIVATE -O3)
