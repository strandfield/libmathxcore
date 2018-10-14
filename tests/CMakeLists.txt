
add_executable(tests test-framework.h test-framework.c main.c)
target_link_libraries(tests libmathxcore)