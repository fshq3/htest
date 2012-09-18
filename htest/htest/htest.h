#ifndef __C_TEST_H
#define __C_TEST_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
namespace htest_lib
{
using namespace std;
#ifdef WIN32
	#include "windows.h"
	#define COLOR_RED (SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0C),"")
	#define COLOR_GREEN (SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0A),"")
	#define COLOR_YELLOW (SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x0E),"")
	#define COLOR_BLUE (SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x09),"")
	#define COLOR_NONE (SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),0x07),"")
#else	
	#define COLOR_RED "\033[1;31m"
	#define COLOR_GREEN "\033[1;32m"
	#define COLOR_YELLOW "\033[1;33m"
	#define COLOR_BLUE "\033[1;34m"
	#define COLOR_NONE "\033[0m"

#endif
#define ctest_for(index,begin,end)  for(unsigned int index=(unsigned int)begin;index<(unsigned int)end;index++)
#define ctest_foreach(type,value,index) for(type::iterator index=value.begin(),end=value.end();index!=end; index++)
//#define test_for(index,end) test_for(index,0,end)
    #define ctest_assert_basic(_cond,_type,_descript,_message)\
        if(!(_cond))\
        {\
            c_message assert;\
            assert.type=_type;\
            assert.file_name=__FILE__;\
            assert.line=__LINE__;\
            assert.descript=_descript;\
			stringstream msg;\
			msg<<_message;\
            assert.message=msg.str();\
            throw assert;\
        };
    #define ctest_assert_message(cond,msg) ctest_assert_basic(cond,"assert",#cond,msg)
    #define ctest_assert_condition(cond) ctest_assert_basic(cond,"assert",#cond,"")
    #define ctest_fail(message) ctest_assert_basic(false,"assert","fail",message)
    #define ctest_assert_m(_expected,_operator,_actual,_message) \
    {\
        stringstream cond;\
        cond<<#_expected<<#_operator<<#_actual\
        <<"{"<<_expected<<#_operator<<_actual<<"}";\
        ctest_assert_basic(((_expected)_operator(_actual)),"assert",cond.str(),_message)\
    }
    #define ctest_assert(_expected,_operator,_actual) ctest_assert_m(_expected,_operator,_actual,"");\

    #define ctest_assert_array(_count,_first,_operator,_second,_ex_msg)\
        {\
            ctest_for(i,0,_count)\
            {\
                stringstream cond,msg;\
                cond<<#_first<<"["<<i<<"]"<<#_operator<<#_second<<"["<<i<<"]";\
                msg<<_first[i]<<#_operator<<_second[i]<<"|ex "<<_ex_msg;\
                ctest_assert_basic(((_first[i])_operator(_second[i])),"assert",cond.str(),msg.str())\
            }\
        }\

    #define ctest_assert_equal(_expected,_actual) ctest_assert(_expected,==,_actual)
    #define ctest_case_add(function) add(#function,&function);
/*
CPPUNIT_ASSERT(condition)：判断condition的值是否为真，如果为假则生成错误信息。

CPPUNIT_ASSERT_MESSAGE(message, condition)：与CPPUNIT_ASSERT类似，但结果为假时报告messsage信息。

CPPUNIT_FAIL(message)：直接报告messsage错误信息。

CPPUNIT_ASSERT_EQUAL(expected, actual)：判断expected和actual的值是否相等，如果不等输出错误信息。

CPPUNIT_ASSERT_EQUAL_MESSAGE(message, expected, actual)：与CPPUNIT_ASSERT_EQUAL类似，但断言失败时输出message信息。

CPPUNIT_ASSERT_DOUBLES_EQUAL(expected, actual, delta)：判断expected与actual的偏差是否小于delta，用于浮点数比较。

CPPUNIT_ASSERT_THROW(expression, ExceptionType)：判断执行表达式expression后是否抛出ExceptionType异常。

CPPUNIT_ASSERT_NO_THROW(expression)：断言执行表达式expression后无异常抛出。*/
    //const char * msg_types[]={"error", "exception", "assert", "ok"};
#define MT_ERROR        "error"
#define MT_EXCEPTION    "exception"
#define MT_ASSERT       "assert"
#define MT_OK           "ok"
#define MT_UNKNOW       "unknow"



    class c_message
    {
    public:
        string type;//error exception assert ok
        string file_name;
        int line;
        string descript;
        string message;
        string pack_name;
        string case_name;
        c_message(){}
        c_message(string type, string file_name, int line, string descript, string pack_name, string case_name)
            :type(type), file_name(file_name), line(line), descript(descript), pack_name(pack_name), case_name(case_name)
        {
        }
        std::string str()
        {
            stringstream res;
            res<<type<<":"<<descript<<"\n"
                <<message<<"\n"
                <<"(" <<pack_name<<","<<case_name<<")"
                <<"["<<dec<<line<<":"<<file_name<<"]"<<"\n";
            return res.str();
        }
    };



    class i_case
    {
    public:
        virtual void call(int index)=0;//运行下标为index的用例
        virtual void init()=0;
        virtual int size()=0;//用例名称
        virtual void test_start()=0;
        virtual void test_end()=0;
        virtual std::string test_regist()=0;
        virtual std::string name()=0;//本测试包名称
        virtual std::string case_name(int index)=0;//测试用例名称
		virtual ~i_case(){}
    };
    class i_output
    {
    public:
        virtual void start(int count=0)=0;
        virtual void end()=0;
        virtual void start_case(i_case*,int index)=0;
        virtual void end_case(i_case*,int index)=0;
        virtual void out(c_message msg)=0;
		virtual ~i_output(){}
    };
    class i_suit
    {
    public:
        virtual i_suit * add(i_case *c)=0;
        virtual i_suit * add(i_output *r)=0;
        virtual void run()=0;
		virtual ~i_suit(){}
    };





    class c_case:public i_case
    {

        typedef void (c_case::*fun_t) ();
        typedef std::pair<std::string,fun_t> case_t;
        std::string _name;
        std::vector<case_t> case_list;
    public:
        virtual ~c_case(){}
        template<typename T>
        void add(std::string name,void(T::*fun)())
        {
            case_list.push_back(case_t(name,(fun_t)fun));
        }
        std::string operator[](int index);
        void call(int index);
        void init();
        //本测试包名称
        std::string name();
        //测试用例名称
        virtual std::string case_name(int index);
        int size();
        virtual void test_start();
        virtual void test_end();
        virtual std::string test_regist()=0;
    };

    class c_output_composite:public i_output
    {
        std::vector<i_output*> output_list;
    public:
        void add(i_output *r);
        void out(c_message msg);
        void end();

        void start(int total_case);
        void start_case(i_case *pcase,int index);
        void end_case(i_case *pcase,int index);
    };
    class c_test:i_suit
    {
        public:
            template<typename T>
                c_test & add(T & cs)
                {
                    add((i_case*)&cs);
                    return *this;
                }
            i_suit * add(i_case *c);
            virtual i_suit * add(i_output *o);
            void run_case(i_case * pc,int index);
            void run_packet(int index);
            int get_total_case();
            void run();
            int size();

        private:
            std::vector<i_case*> case_list;
            c_output_composite output;
    };


    class c_std_output:public i_output
    {
        protected:
            map<std::string,bool> accept;
            map<std::string,int> counter;
            clock_t start_ms;
            int case_count;
            int total_case;
            ostream & sout;
        public:
            c_std_output();
            c_std_output(ostream & _out);
            c_std_output & exception(bool condition);
            /* c_std_output & assert(bool condition)
               {
               accept[MT_ASSERT]=condition;
               return *this;
               }*/
            c_std_output & ok(bool condition);
            c_std_output & error(bool condition);
            bool exception();
            /*bool assert()
              {
              return is_accept(MT_ASSERT);
              }*/
            bool ok();
            bool error();

            c_std_output& set_accept(std::string type,bool condition);
            bool is_accept(std::string type);
			string color(string type);
            virtual void out(c_message msg);
            bool is_ok();
            double get_percent();
            virtual void start_case(i_case*pc,int index);
            virtual void end_case(i_case*,int index);
            virtual void start(int count);
            virtual void end();
    };



    //facade
    class c_std_test
    {
        public:
            c_test test;
            c_std_output out;
            inline c_std_test()
            {
                test.add(&out);
            }
            inline c_std_test(ostream &_out):out(_out)
            {
                test.add(&out);
            }
            inline c_std_test add(c_case & c)
            {
                test.add(c);
                return *this;
            }
            inline void run()
            {
                test.run();
            }
    };
    /*
       测试的门面接口,继承c_std_case,并实现c_case的函数，并实例化一个对象，然后调用c_std_run::run();运行并输出结果如:
       class testFile:public c_std_case
       {
       public:
       string test_regist()
       {
       add("test block",&testFile::test_block);
       return "testFile";
       }
       void test_block()
       {
       ctest_assert(false);
       }

       }_testFile;
       int main()
       {
       c_std_run::run();
       return 0;
       }

*/
    class c_std_run
    {
        public:
            inline static c_std_test & std_test()
            {
                static c_std_test  test;
                return test;
            }
            inline static void run()
            {
                std_test().run();
            }
    };
    class c_std_case:public c_case
    {
        public:
            inline static void run()
            {
                c_std_run::std_test().run();
            }
            inline c_std_case()
            {
                c_std_run::std_test().add(*this);
            }
    };

}
#endif
