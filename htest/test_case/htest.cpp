#include "htest.h"
using namespace htest_lib;
class test_test_ac:public c_std_case
{
public:
	string test_regist()
	{
		add("test a",&test_test_ac::test_a);
		add("test b",&test_test_ac::test_a);
		add("test c",&test_test_ac::test_a);
		add("test d",&test_test_ac::test_a);
		add("test e",&test_test_ac::test_a);
		return "test_test_ac";
	}

	void test_a()
	{
		int i=100000000;
		while(i--)
		;
	}
}_test_test_ac;
