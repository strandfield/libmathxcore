
#ifndef LIBMATHXCORE_TEST_FRAMEWORK_H
#define LIBMATHXCORE_TEST_FRAMEWORK_H

struct test_case_struct
{
  const char *suite;
  const char *name;
  void(*callback)();
};

typedef struct test_case_struct test_case_t;

#define Test(Suite, Name) void test_##Suite##_##Name##(); \
  test_case_t Suite##Name = {.suite = #Suite, .name = #Name, .callback = test_##Suite##_##Name## }; \
  void test_##Suite##_##Name##()


struct test_suite_struct
{
  const char *name;
  test_case_t* tests[];
};

typedef struct test_suite_struct test_suite_t;

#define EXPAND( x ) x
#define FE_1(Suite, X) &Suite##X
#define FE_2(Suite, X, ...) &Suite##X,EXPAND(FE_1(Suite, __VA_ARGS__))
#define FE_3(Suite, X, ...) &Suite##X,EXPAND(FE_2(Suite, __VA_ARGS__))
#define FE_4(Suite, X, ...) &Suite##X,EXPAND(FE_3(Suite, __VA_ARGS__))
#define FE_5(Suite, X, ...) &Suite##X,EXPAND(FE_4(Suite, __VA_ARGS__))
#define FE_6(Suite, X, ...) &Suite##X,EXPAND(FE_5(Suite, __VA_ARGS__))
#define FE_7(Suite, X, ...) &Suite##X,EXPAND(FE_6(Suite, __VA_ARGS__))
#define FE_8(Suite, X, ...) &Suite##X,EXPAND(FE_7(Suite, __VA_ARGS__))
#define FE_9(Suite, X, ...) &Suite##X,EXPAND(FE_8(Suite, __VA_ARGS__))
#define FE_10(Suite, X, ...) &Suite##X,EXPAND(FE_9(Suite, __VA_ARGS__))
#define FE_11(Suite, X, ...) &Suite##X,EXPAND(FE_10(Suite, __VA_ARGS__))
#define FE_12(Suite, X, ...) &Suite##X,EXPAND(FE_11(Suite, __VA_ARGS__))
#define FE_13(Suite, X, ...) &Suite##X,EXPAND(FE_12(Suite, __VA_ARGS__))
#define FE_14(Suite, X, ...) &Suite##X,EXPAND(FE_13(Suite, __VA_ARGS__))
#define FE_15(Suite, X, ...) &Suite##X,EXPAND(FE_14(Suite, __VA_ARGS__))
#define FE_16(Suite, X, ...) &Suite##X,EXPAND(FE_15(Suite, __VA_ARGS__))
#define FE_17(Suite, X, ...) &Suite##X,EXPAND(FE_16(Suite, __VA_ARGS__))
#define FE_18(Suite, X, ...) &Suite##X,EXPAND(FE_17(Suite, __VA_ARGS__))
#define FE_19(Suite, X, ...) &Suite##X,EXPAND(FE_18(Suite, __VA_ARGS__))
#define FE_20(Suite, X, ...) &Suite##X,EXPAND(FE_19(Suite, __VA_ARGS__))
#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,NAME,...) NAME 
#define CONCAT_FOR_EACH(Suite,...) \
  EXPAND(GET_MACRO(__VA_ARGS__,FE_20,FE_19,FE_18,FE_17,FE_16,FE_15,FE_14,FE_13,FE_12,FE_11,FE_10,FE_9,FE_8,FE_7,FE_6,FE_5,FE_4,FE_3,FE_2,FE_1)(Suite,__VA_ARGS__))

#define TestSuite(Suite, ...) test_suite_t Suite = {.name = #Suite, .tests = {CONCAT_FOR_EACH(Suite, __VA_ARGS__), 0}}

void init_test_framework();
void register_test(test_suite_t *suite);
void run_all_tests();

int test_assert(int cond, int line);
#define Assert(cond) if(!test_assert(cond, __LINE__)) return

#endif // LIBMATHXCORE_TEST_FRAMEWORK_H
