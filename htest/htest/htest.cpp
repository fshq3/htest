#include "htest.h"
using namespace std;

namespace htest_lib
{
    std::string c_case::operator[](int index)
    {
        return case_list[index].first;
    }
    void c_case::call(int index)
    {
        test_start();
        (this->*(case_list[index]).second)();
        test_end();
    }
    void c_case::init()
    {
        case_list.clear();
        _name=test_regist();
    }
    //本测试包名称
    std::string c_case::name()
    {
        return _name;
    }
    //测试用例名称
    std::string c_case::case_name(int index)
    {
        return case_list[index].first;
    }
    int c_case::size()
    {
        return case_list.size();
    }
    void c_case::test_start()
    {
    }
    void c_case::test_end()
    {
    }

    //c_output_composite
    void c_output_composite::add(i_output *r)
    {
        output_list.push_back(r);
    }
    void c_output_composite::out(c_message msg)
    {
        ctest_for(i,0,output_list.size())
        {
            output_list[i]->out(msg);
        }
    }
    void c_output_composite::end()
    {
        ctest_for(i,0,output_list.size())
        {
            output_list[i]->end();
        }
    }

    void c_output_composite::start(int total_case)
    {
        ctest_for(i,0,output_list.size())
        {
            output_list[i]->start(total_case);
        }
    }
    void c_output_composite::start_case(i_case *pcase,int index)
    {
        ctest_for(i,0,output_list.size())
        {
            output_list[i]->start_case(pcase,index);
        }
    }
    void c_output_composite::end_case(i_case *pcase,int index)
    {
        ctest_for(i,0,output_list.size())
        {
            output_list[i]->end_case(pcase,index);
        }
    }

    //c_test 
    i_suit * c_test::add(i_case *c)
    {
        case_list.push_back(c);
        return this;
    }
    i_suit * c_test::add(i_output *o)
    {
        output.add(o);
        return this;
    }
    void c_test::run_case(i_case * pc,int index)
    {
        output.start_case(pc,index);
        pc->test_start();
        try
        {
            pc->call(index);
            c_message msg(MT_OK,"-",0,"",pc->name(),pc->case_name(index));
            output.out(msg);
        }
        catch(c_message error)
        {
            error.pack_name=pc->name();
            error.case_name=pc->case_name(index);
            output.out(error);
        }
        catch(std::exception *e)
        {
            c_message error(MT_EXCEPTION,e->what(),0,"",pc->name(),pc->case_name(index));
            output.out(error);
        }
        catch(...)
        {
            c_message error(MT_EXCEPTION,"unknow",0,"",pc->name(),pc->case_name(index));
            //error.output(cout);
            output.out(error);
        }
        pc->test_end();
        output.end_case(pc,index);

    }
    void c_test::run_packet(int index)
    {
        i_case * pc = case_list[index];
        pc->init();
        ctest_for(j,0,pc->size())
        {
            run_case(pc,j);
        }
    }
    int c_test::get_total_case()
    {
        int res=0;
        ctest_for(i,0,case_list.size())
        {
            case_list[i]->init();
            res+=case_list[i]->size();
            cout<<case_list[i]->size()<<endl;
        }
        return res;
    }
    void c_test::run()
    {
        output.start(get_total_case());
        ctest_for(i,0,case_list.size())
        {
            run_packet(i);
        }
        output.end();
    }
    int c_test::size()
    {
        return case_list.size();
    }


    //c_std_output

    c_std_output::c_std_output():sout(cout)
    {
        ok(false);

    }
    c_std_output::c_std_output(ostream & _out):sout(_out)
    {
        ok(false);
    }
    c_std_output & c_std_output::exception(bool condition)
    {
        accept[MT_EXCEPTION]=condition;
        return *this;
    }
    /* c_std_output & assert(bool condition)
       {
       accept[MT_ASSERT]=condition;
       return *this;
       }*/
    c_std_output & c_std_output::ok(bool condition)
    {
        accept[MT_OK]=condition;
        return *this;
    }
    c_std_output & c_std_output::error(bool condition)
    {
        accept[MT_ERROR]=condition;
        return *this;
    }
    bool c_std_output::exception()
    {
        return is_accept(MT_EXCEPTION);
    }
    /*bool assert()
      {
      return is_accept(MT_ASSERT);
      }*/
    bool c_std_output::ok()
    {
        return is_accept(MT_OK);
    }
    bool c_std_output::error()
    {
        return is_accept(MT_ERROR);
    }

    c_std_output& c_std_output::set_accept(std::string type,bool condition)
    {
        accept[type]=condition;
        return *this;
    }
    bool c_std_output::is_accept(std::string type)
    {
        if(accept.find(type)==accept.end())
            return true;
        return accept[type];
    }
	string c_std_output::color(string type)
	{
		if(type!=MT_OK)
			return COLOR_RED;
		return COLOR_GREEN;

	}
    void c_std_output::out(c_message msg)
    {
        counter[msg.type]++;
        if(is_accept(msg.type)){
            sout<<"\n"<<color(msg.type)<<msg.str();
			sout<<COLOR_BLUE<<"\n________________________________________\n"<<COLOR_NONE;
		}
    }
    bool c_std_output::is_ok()
    {
        map<std::string,int>::iterator it,end;
        for(it=counter.begin(),end=counter.end(); it!=end; it++)
        {
            if(MT_OK!=it->first && it->second>0)
                return false;
        }
        return true;
    }
    double c_std_output::get_percent()
    {
        return case_count/double(total_case)*100;
    }
    void c_std_output::start_case(i_case*pc,int index)
    {
        case_count++;
        sout<<"\r"<<COLOR_NONE<<get_percent()<<"%"<<case_count<<":"<<pc->name()<<"->"<<pc->case_name(index)<<endl;
    }
    void c_std_output::end_case(i_case*,int index)
    {
        sout<<"\r"<<".\t\t\t\t\t\t\t\t\t.";
    }
    void c_std_output::start(int count)
    {
        sout<<"=======================test start========================"<<count<<endl;
        counter.clear();
        case_count=0;
        total_case=count;
        start_ms=clock();
    }
    void c_std_output::end()
    {
        sout<<"\nrun time:"<<double(clock()-start_ms)/CLOCKS_PER_SEC<<"s"<<endl;
        sout<<"======================================="<<endl;
        if(is_ok())
        {
            sout<<COLOR_GREEN<<"ok"<<"("<<counter[MT_OK]<<")"<<COLOR_NONE<<endl;
        }else
        {
			sout<<COLOR_RED;
            map<std::string,int>::iterator it,end;
            for(it=counter.begin(),end=counter.end(); it!=end; it++)
            {
                sout<<it->first<<":"<<it->second<<endl;
            }
            sout<<"total:"<<case_count<<COLOR_NONE<<endl;
        }

    }

}
