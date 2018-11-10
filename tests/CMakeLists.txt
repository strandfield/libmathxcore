
add_executable(tests test-framework.h test-framework.c main.c test_rationals.c)
target_link_libraries(tests libmathxcore)