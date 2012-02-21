#include "../htest.h"
using namespace htest_lib;
class test_test:public c_std_case
{
public:
	string test_regist()
	{
		ctest_case_add(test_test::test_assert);
		ctest_case_add(test_test::test_assert_message);
		ctest_case_add(test_test::test_exception);
		ctest_case_add(test_test::test_ok);
		ctest_case_add(test_test::test_fail);
		add("test equal",&test_test::test_equal);
		return "test_test";
	}
	void test_equal()
	{
		int i=1;
		ctest_assert_equal(2,i);
	}
	void test_fail()
	{
		ctest_fail("Ê§°Ü");
	}
	void test_ok()
	{
		int i=10000000;
		while(i--)
		;
	}
	void test_assert()
	{
		ctest_assert(false);
	}
	void test_assert_message()
	{
		ctest_assert_message(false,"²âÊÔ");
	}
	void test_exception()
	{
		throw new std::bad_exception();
	}
}_test_test;
