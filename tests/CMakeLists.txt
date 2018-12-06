
add_executable(tests test-framework.h test-framework.c test-framework-int.c test-framework-float.c main.c test_rationals.c test_floats.c test_float_functions.c)
target_link_libraries(tests libmathxcore)