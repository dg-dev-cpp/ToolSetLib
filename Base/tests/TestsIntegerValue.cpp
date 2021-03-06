#include "TestsIntegerValue.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;


namespace ToolSetLib
{

namespace Base
{

namespace Tests
{

unsigned char
GetAbsoluteValueTest(char a_value)
{
    if (a_value >= 0)
        return (unsigned char)a_value;

    if (a_value == std::numeric_limits<decltype(a_value)>::min())
    {
        static_assert(((char)-127 - 1) == std::numeric_limits<decltype(a_value)>::min(), "unsupported char min value");

        return (unsigned char)128;
    }

    return  (unsigned char)((char)0 - a_value);
}


unsigned char
GetAbsoluteValueTest(unsigned char a_value)
{
    return (unsigned char)a_value;
}


unsigned short
GetAbsoluteValueTest(short a_value)
{
    if (a_value >= 0)
        return (unsigned short)a_value;

    if (a_value == std::numeric_limits<decltype(a_value)>::min())
    {
        static_assert(((short)-32767 - 1) == std::numeric_limits<decltype(a_value)>::min(), "unsupported short min value");

        return (unsigned short)32768;
    }

    return  (unsigned short)((short)0 - a_value);
}


unsigned short
GetAbsoluteValueTest(unsigned short a_value)
{
    return (unsigned short)a_value;
}


unsigned int
GetAbsoluteValueTest(int a_value)
{
    if (a_value >= 0)
        return (unsigned int)a_value;

    if (a_value == std::numeric_limits<decltype(a_value)>::min())
    {
        static_assert(((int)-2147483647 - 1) == std::numeric_limits<decltype(a_value)>::min(), "unsupported int min value");

        return (unsigned int)2147483648u;
    }

    return  (unsigned int)((int)0 - a_value);
}


unsigned int
GetAbsoluteValueTest(unsigned int a_value)
{
    return (unsigned int)a_value;
}


uint64_t
GetAbsoluteValueTest(int64_t a_value)
{
    if (a_value >= 0)
        return (uint64_t)a_value;

    if (a_value == std::numeric_limits<decltype(a_value)>::min())
    {
        static_assert(((int64_t)-9223372036854775807LL - 1) == std::numeric_limits<decltype(a_value)>::min(), "unsupported int64_t min value");

        return (9223372036854775808ULL);
    }

    return (uint64_t)(0LL - a_value);
}


uint64_t
GetAbsoluteValueTest(uint64_t a_value)
{
    return (uint64_t)a_value;
}


template<typename Param1Type, typename Param2Type>
bool
TestIntegerTypesOperations()
{
    Param1Type param1;
    Param2Type param2;

    cout<<"info: TestIntegerTypesOperations(): test operations for next types pair: "<<endl;
    cout<<"   "<<setw(10)<<left<<param1.GetTypeInfo().first<<", signed: "<<param1.IsValueSigned()
        <<", min: "<<setw(12)<<param1.GetMinAs64i()<<", max: "<<setw(12)<<param1.GetMaxAs64ui()<<endl;
    cout<<"   "<<setw(10)<<left<<param2.GetTypeInfo().first<<", signed: "<<param2.IsValueSigned()
        <<", min: "<<setw(12)<<param2.GetMinAs64i()<<", max: "<<setw(12)<<param2.GetMaxAs64ui()<<endl;
    
    #define MAX_TEST_VALUES 10002

    uint64_t total_incorrect_exceptions =   0;
    uint64_t total_incorrect_values =   0;
    uint64_t total_incorrect_overflow = 0;
    uint64_t total_not_overflow =       0;

    for(    int64_t count_val1  =   0;
                    count_val1  <=  MAX_TEST_VALUES;
                    count_val1  +=  1)
    {
        int64_t    param1_test_value = 0;

        if( count_val1 <= 255LL)
        {
            param1_test_value = count_val1 - 128LL;
        }

        if(     count_val1  >   255LL
            &&  count_val1  <=  5000LL)
        {
            param1_test_value = param1.GetMinAs64i() + (count_val1 - 256LL);

            if(     param1_test_value           >   0
                &&  (uint64_t)param1_test_value >   param1.GetMaxAs64ui())
            continue;
        }

        if(     count_val1  >   5000LL
            &&  count_val1  <=  10000LL)
        {
            if(     (uint64_t)count_val1    >   param1.GetMaxAs64ui()
                ||  param1.GetMaxAs64ui()   >   std::numeric_limits<decltype(param1_test_value)>::max())
                continue;

            param1_test_value = param1.GetMaxAs64ui() - ((uint64_t)count_val1 - 5001ULL);
        }

        if( count_val1  == 10001)
        {
            param1_test_value = std::numeric_limits<decltype(param1_test_value)>::min();
        }

        if( count_val1  == 10002)
        {
            param1_test_value = std::numeric_limits<decltype(param1_test_value)>::max();
        }

        try
        {
            param1.Clear();
            param1.Add(param1_test_value);
        }
        catch(...)
        {}

        for(    int64_t count_val2 =    0;
                        count_val2 <=   MAX_TEST_VALUES;
                        count_val2 +=   1)
        {
            int64_t    param2_test_value = 0;

            if( count_val2 <= 255LL)
            {
                param2_test_value = count_val2 - 128LL;
            }

            if(     count_val2  >   255LL
                &&  count_val2  <=  5000LL)
            {
                param2_test_value = param2.GetMinAs64i() + (count_val2 - 256LL);

                if(     param2_test_value           >   0
                    &&  (uint64_t)param2_test_value >   param2.GetMaxAs64ui())
                    continue;
            }

            if(     count_val2  >   5000LL
                &&  count_val2  <=  10000LL)
            {
                if(     (uint64_t)count_val2    >   param2.GetMaxAs64ui()
                    ||  param2.GetMaxAs64ui()   >   std::numeric_limits<decltype(param2_test_value)>::max())
                    continue;

                param2_test_value = param2.GetMaxAs64ui() - ((uint64_t)count_val2 - 5001ULL);
            }


            if (count_val2 == 10001)
            {
                param2_test_value = std::numeric_limits<decltype(param2_test_value)>::min();
            }

            if (count_val2 == 10002)
            {
                param2_test_value = std::numeric_limits<decltype(param2_test_value)>::max();
            }

            try
            {
                param2 = param2_test_value;
            }
            catch(...)
            {}

            try
            {
                // test compare operations
                
                //Param1Type result_param;

                //try
                //{
                //    result_param += param1;
                //    result_param.Sub(param2);
                //}
                //catch(...)
                //{}
                
                //auto check_result = param1_test_value - param2_test_value;

                if(     param1_test_value       <   param1.GetMinAs64i()
                    ||  param2_test_value       <   param2.GetMinAs64i()
                    || (param1_test_value > 0 && (uint64_t)param1_test_value > param1.GetMaxAs64ui())
                    || (param2_test_value > 0 && (uint64_t)param2_test_value > param2.GetMaxAs64ui()))
                {
                    if(     param1.IsOverflow() ==  false
                        &&  param2.IsOverflow() ==  false)
                    {
                        bool is_error = true;

                        if( string(param1.GetTypeInfo().first)  ==  "uint64_t")
                        {
                            //try
                            //{
                            //    if(     param1_test_value > 0 && param2_test_value < 0
                            //        &&  (std::numeric_limits<uint64_t>::max() - param1.GetUInt64()) >= GetAbsoluteValueTest(param2.GetInt64()))
                            //        is_error = false;
                            //}
                            //catch(...)
                            //{}
                        }

                        if( is_error == true)
                        {
                            total_not_overflow += 1;

                            cout<<"   error: compare result not overflow: "<<param1_test_value << ", "<< param2_test_value<<endl;
                            cout<<"      param1.GetTypeInfo(): "<<param1.GetTypeInfo().first<<endl;
                            cout<<"      param1.IsOverflow(): "<<param1.IsOverflow()<< ", "<<param1.GetInt64()<<endl;
                            cout<<"      param2.IsOverflow(): "<<param2.IsOverflow()<< ", "<<param2.GetInt64()<<endl;
                        }
                    }
                }
                else
                {
                    if(     param1.IsOverflow() ==  true
                        ||  param2.IsOverflow() ==  true)
                    {
                        total_incorrect_overflow += 1;

                        cout<<"   error: compare result overflow: "<<param1_test_value << ","<< param2_test_value<<endl;
                    }
                    else
                    {
                        if( param1_test_value < param2_test_value)
                        {
                            if(     param1              < param2
                                &&  param1_test_value   < param2)
                            {}
                            else
                            {
                                total_incorrect_values += 1;

                                cout << "   error: compare result incorrect: " << param1_test_value << " < " << param2_test_value << endl;
                            }
                        }

                        if( param1_test_value > param2_test_value)
                        {
                            if(     param1              >   param2
                                &&  param1_test_value   >   param2)
                            {}
                            else
                            {
                                total_incorrect_values += 1;

                                cout << "   error: compare result incorrect: " << param1_test_value << " > " << param2_test_value << endl;
                            }
                        }

                        if( param1_test_value == param2_test_value)
                        {
                            if(     param1              == param2
                                &&  param1_test_value   == param2)
                            {}
                            else
                            {
                                total_incorrect_values += 1;

                                cout << "   error: compare result incorrect: " << param1_test_value << " == " << param2_test_value << endl;
                            }
                        }

                        if( param1_test_value >= param2_test_value)
                        {
                            if(     param1              >= param2
                                &&  param1_test_value   >= param2)
                            {}
                            else
                            {
                                total_incorrect_values += 1;

                                cout << "   error: compare result incorrect: " << param1_test_value << " >= " << param2_test_value << endl;
                            }
                        }

                        if( param1_test_value <= param2_test_value)
                        {
                            if(     param1              <= param2
                                &&  param1_test_value   <= param2)
                            {}
                            else
                            {
                                total_incorrect_values += 1;

                                cout << "   error: compare result incorrect: " << param1_test_value << " <= " << param2_test_value << endl;
                            }
                        }
                    }
                }

            }
            catch(std::runtime_error& re)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test compare exception: "<<re.what()<<endl;
            }
            catch(...)
            {
                total_incorrect_exceptions += 1;
                cout<<"   error: test compare general exception"<<endl;
            }



            try
            {
                // test sub operations

                Param1Type result_param;

                try
                {
                    result_param += param1;
                    result_param.Sub(param2);
                }
                catch(...)
                {}
                
                auto check_result = param1_test_value - param2_test_value;

                if(     param1_test_value       <   param1.GetMinAs64i()
                    ||  param2_test_value       <   param2.GetMinAs64i()
                    || (param1_test_value > 0 && (uint64_t)param1_test_value > param1.GetMaxAs64ui())
                    || (param2_test_value > 0 && (uint64_t)param2_test_value > param2.GetMaxAs64ui())
                    || check_result < result_param.GetMinAs64i()
                    || (check_result > 0 && (uint64_t)check_result > result_param.GetMaxAs64ui())
                    || (param1_test_value > 0 && param2_test_value < 0 
                        &&  GetAbsoluteValueTest(std::numeric_limits<decltype(param1_test_value)>::max() - (uint64_t) param1_test_value) < GetAbsoluteValueTest(param2_test_value))
                    || (param1_test_value < 0 && param2_test_value > 0
                        &&  GetAbsoluteValueTest(std::numeric_limits<decltype(param1_test_value)>::min() - param1_test_value) < GetAbsoluteValueTest(param2_test_value)))
                {
                    if( result_param.IsOverflow() ==  false)
                    {
                        bool is_error = true;

                        if( string(param1.GetTypeInfo().first)  ==  "uint64_t")
                        {
                            try
                            {
                                if(     param1_test_value > 0 && param2_test_value < 0
                                    &&  (std::numeric_limits<uint64_t>::max() - param1.GetUInt64()) >= GetAbsoluteValueTest(param2.GetInt64()))
                                    is_error = false;
                            }
                            catch(...)
                            {}
                        }

                        if( is_error == true)
                        {
                            total_not_overflow += 1;

                            cout<<"   error: sub result not overflow: "<<param1_test_value << " - "<< param2_test_value<<endl;
                            cout<<"      result_param.GetValue():   "<<to_string(result_param.GetValue())<<endl;
                            cout<<"      param1.GetTypeInfo(): "<<param1.GetTypeInfo().first<<endl;
                            cout<<"      param1.IsOverflow(): "<<param1.IsOverflow()<< ", "<<param1.GetInt64()<<endl;
                            cout<<"      param2.IsOverflow(): "<<param2.IsOverflow()<< ", "<<param2.GetInt64()<<endl;
                        }
                    }
                }
                else
                {
                    if( result_param.IsOverflow() == true)
                    {
                        total_incorrect_overflow += 1;

                        cout<<"   error: sub result overflow: "<<param1_test_value << " - "<< param2_test_value<<" = "<<check_result<<endl;
                    }
                    else
                    {
                        if( check_result != result_param.GetInt64())
                        {
                            total_incorrect_values += 1;

                            cout<<"   error: sub result incorrect: "<<param1_test_value << " - "<< param2_test_value<<" = "<<check_result
                                <<", current result: "<< result_param.GetInt64()<<endl;
                        }
                    }
                }
            }
            catch(std::runtime_error& re)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test sub exception: "<<re.what()<<endl;
            }
            catch(...)
            {
                total_incorrect_exceptions += 1;
                cout<<"   error: test sub general exception"<<endl;
            }


            try
            {
                // test div operations

                Param1Type result_param;

                try
                {
                    result_param += param1;
                    result_param.Div(param2);
                }
                catch(...)
                {}
                
                int64_t check_result = 0;

                if( param2_test_value != 0)
                {
                    if ( param1_test_value == INT64_MIN && param2_test_value == -1)
                    {
                    }
                    else
                    {
                        check_result = param1_test_value / param2_test_value;
                    }
                }

                if(     param2_test_value       ==  0
                    ||  param1_test_value       <   param1.GetMinAs64i()
                    ||  param2_test_value       <   param2.GetMinAs64i()
                    || (param1_test_value > 0 && (uint64_t)param1_test_value > param1.GetMaxAs64ui())
                    || (param2_test_value > 0 && (uint64_t)param2_test_value > param2.GetMaxAs64ui())
                    || check_result < result_param.GetMinAs64i()
                    || (check_result > 0 && (uint64_t)check_result > result_param.GetMaxAs64ui())
                    || result_param.GetTypeInfo().second == false && param1_test_value > 0 && param2_test_value < 0
                    || param1_test_value == INT64_MIN && param2_test_value == -1)
                {
                    if( result_param.IsOverflow() ==  false)
                    {
                        bool is_error = true;

                        if( string(param1.GetTypeInfo().first)  ==  "uint64_t")
                        {
                            try
                            {
                                //if(     param1_test_value > 0 && param2_test_value > 0
                                //    &&  (std::numeric_limits<uint64_t>::max() / param1.GetUInt64()) >= GetAbsoluteValueTest(param2.GetInt64()))
                                //    is_error = false;
                            }
                            catch(...)
                            {}
                        }

                        if( is_error == true)
                        {
                            total_not_overflow += 1;

                            cout<<"   error: div result not overflow: "<<param1_test_value << " / "<< param2_test_value<<endl;
                            cout<<"      result_param.GetValue():   "<<to_string(result_param.GetValue())<<endl;
                            cout<<"      param1.GetTypeInfo(): "<<param1.GetTypeInfo().first<<endl;
                            cout<<"      param1.IsOverflow(): "<<param1.IsOverflow()<< ", "<<param1.GetInt64()<<endl;
                            cout<<"      param2.IsOverflow(): "<<param2.IsOverflow()<< ", "<<param2.GetInt64()<<endl;
                        }
                    }
                }
                else
                {
                    if( result_param.IsOverflow() == true)
                    {
                        total_incorrect_overflow += 1;

                        cout<<"   error: div result overflow: "<<param1_test_value << " / "<< param2_test_value<<" = "<<check_result<<endl;
                    }
                    else
                    {
                        if( check_result != result_param.GetInt64())
                        {
                            total_incorrect_values += 1;

                            cout<<"   error: div result incorrect: "<<param1_test_value << " / "<< param2_test_value<<" = "<<check_result
                                <<", current result: "<< result_param.GetInt64()<<endl;
                        }
                    }
                }
            }
            catch(std::runtime_error& re)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test div operation exception: "<<re.what()<<endl;
            }
            catch(...)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test div operation general exception"<<endl;
            }

            try
            {
                // test mul operations

                Param1Type result_param;

                try
                {
                    result_param += param1;
                    result_param.Mul(param2);
                }
                catch(...)
                {}
                
                auto check_result = param1_test_value * param2_test_value;

                if(     param1_test_value       <   param1.GetMinAs64i()
                    ||  param2_test_value       <   param2.GetMinAs64i()
                    || (param1_test_value > 0 && (uint64_t)param1_test_value > param1.GetMaxAs64ui())
                    || (param2_test_value > 0 && (uint64_t)param2_test_value > param2.GetMaxAs64ui())
                    || check_result < result_param.GetMinAs64i()
                    || (check_result > 0 && (uint64_t)check_result > result_param.GetMaxAs64ui())
                    || result_param.GetTypeInfo().second == false && check_result < 0
                    || (param1_test_value > 0 && param2_test_value > 0 
                        &&  GetAbsoluteValueTest(std::numeric_limits<decltype(param1_test_value)>::max() / (uint64_t) param1_test_value) < GetAbsoluteValueTest(param2_test_value))
                    || (param1_test_value < 0 && param2_test_value < 0
                        && GetAbsoluteValueTest(std::numeric_limits<decltype(param1_test_value)>::max() / GetAbsoluteValueTest(param1_test_value)) < GetAbsoluteValueTest(param2_test_value))
                    || (param1_test_value < 0 && param2_test_value > 0
                        &&  GetAbsoluteValueTest(GetAbsoluteValueTest(std::numeric_limits<decltype(param1_test_value)>::min()) / GetAbsoluteValueTest(param1_test_value)) < GetAbsoluteValueTest(param2_test_value))
                    || (param1_test_value > 0 && param2_test_value < 0
                        && GetAbsoluteValueTest(GetAbsoluteValueTest(std::numeric_limits<decltype(param1_test_value)>::min()) / GetAbsoluteValueTest(param1_test_value)) < GetAbsoluteValueTest(param2_test_value)))
                {
                    if( result_param.IsOverflow() ==  false)
                    {
                        bool is_error = true;

                        if( string(param1.GetTypeInfo().first)  ==  "uint64_t")
                        {
                            try
                            {
                                if(     param1_test_value > 0 && param2_test_value > 0
                                    &&  (std::numeric_limits<uint64_t>::max() / param1.GetUInt64()) >= GetAbsoluteValueTest(param2.GetInt64()))
                                    is_error = false;
                            }
                            catch(...)
                            {}
                        }

                        if( is_error == true)
                        {
                            total_not_overflow += 1;

                            cout<<"   error: mul result not overflow: "<<param1_test_value << " * "<< param2_test_value<<endl;
                            cout<<"      result_param.GetValue():   "<<to_string(result_param.GetValue())<<endl;
                            cout<<"      param1.GetTypeInfo(): "<<param1.GetTypeInfo().first<<endl;
                            cout<<"      param1.IsOverflow(): "<<param1.IsOverflow()<< ", "<<param1.GetInt64()<<endl;
                            cout<<"      param2.IsOverflow(): "<<param2.IsOverflow()<< ", "<<param2.GetInt64()<<endl;
                        }
                    }
                }
                else
                {
                    if( result_param.IsOverflow() == true)
                    {
                        total_incorrect_overflow += 1;

                        cout<<"   error: mul result overflow: "<<param1_test_value << " * "<< param2_test_value<<" = "<<check_result<<endl;
                    }
                    else
                    {
                        if( check_result != result_param.GetInt64())
                        {
                            total_incorrect_values += 1;

                            cout<<"   error: mul result incorrect: "<<param1_test_value << " * "<< param2_test_value<<" = "<<check_result
                                <<", current result: "<< result_param.GetInt64()<<endl;
                        }
                    }
                }
            }
            catch(std::runtime_error& re)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test mul operation exception: "<<re.what()<<endl;
            }
            catch(...)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test mul operation general exception"<<endl;
            }


            try
            {
                // test add operations

                Param1Type result_param;

                try
                {
                    result_param += param1;
                    result_param.Add(param2);
                }
                catch(...)
                {}
                
                auto check_result = param1_test_value + param2_test_value;

                if(    param1_test_value       <   param1.GetMinAs64i()
                    || param2_test_value       <   param2.GetMinAs64i()
                    || (param1_test_value > 0 && (uint64_t)param1_test_value > param1.GetMaxAs64ui())
                    || (param2_test_value > 0 && (uint64_t)param2_test_value > param2.GetMaxAs64ui())
                    || check_result < result_param.GetMinAs64i()
                    || (check_result > 0 && (uint64_t)check_result > result_param.GetMaxAs64ui())
                    || (param1_test_value > 0 && param2_test_value > 0 
                        &&  (std::numeric_limits<decltype(param1_test_value)>::max() - (uint64_t) param1_test_value) < param2_test_value)
                    || (param1_test_value < 0 && param2_test_value < 0
                        &&  (std::numeric_limits<decltype(param1_test_value)>::min() - param1_test_value) > param2_test_value))
                {
                    if( result_param.IsOverflow() ==  false)
                    {
                        bool is_error = true;

                        if( string(param1.GetTypeInfo().first)  ==  "uint64_t")
                        {
                            try
                            {
                                if(     param1_test_value > 0 && param2_test_value > 0
                                    &&  (std::numeric_limits<uint64_t>::max() - param1.GetUInt64()) > param2.GetUInt64())
                                    is_error = false;
                            }
                            catch(...)
                            {}
                        }

                        if( is_error == true)
                        {
                            total_not_overflow += 1;

                            cout<<"   error: add result not overflow: "<<param1_test_value << " + "<< param2_test_value<<endl;
                            cout<<"      result_param.GetValue():   "<<to_string(result_param.GetValue())<<endl;
                            cout<<"      param1.GetTypeInfo(): "<<param1.GetTypeInfo().first<<endl;
                            cout<<"      param1.IsOverflow(): "<<param1.IsOverflow()<< ", "<<param1.GetInt64()<<endl;
                            cout<<"      param2.IsOverflow(): "<<param2.IsOverflow()<< ", "<<param2.GetInt64()<<endl;
                        }
                    }
                }
                else
                {
                    if( result_param.IsOverflow() == true)
                    {
                        total_incorrect_overflow += 1;

                        cout<<"   error: add result overflow: "<<param1_test_value << " + "<< param2_test_value<<" = "<<check_result<<endl;
                    }
                    else
                    {
                        if( check_result != result_param.GetInt64())
                        {
                            total_incorrect_values += 1;

                            cout<<"   error: add result incorrect: "<<param1_test_value << " + "<< param2_test_value<<" = "<<check_result
                                <<", current result: "<< result_param.GetInt64()<<endl;
                        }
                    }
                }
            }
            catch(std::runtime_error& re)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test add exception: "<<re.what()<<endl;
            }
            catch(...)
            {
                total_incorrect_exceptions += 1;

                cout<<"   error: test add general exception"<<endl;
            }
        }
    }

    if(     total_incorrect_exceptions  > 0
        ||  total_incorrect_values      > 0
        ||  total_incorrect_overflow    > 0
        ||  total_not_overflow          > 0)
        return false;

    return true;
}


template<typename param_type>
void
TestIntegerTypeOperations()
{
    TestIntegerTypesOperations<param_type, CharValExp>();
    TestIntegerTypesOperations<param_type, UCharValExp>();
    TestIntegerTypesOperations<param_type, ShortValExp>();
    TestIntegerTypesOperations<param_type, UShortValExp>();
    TestIntegerTypesOperations<param_type, Int32ValExp>();
    TestIntegerTypesOperations<param_type, UInt32ValExp>();
    TestIntegerTypesOperations<param_type, Int64ValExp>();
    TestIntegerTypesOperations<param_type, UInt64ValExp>();
}


void
TestIntegerValue()
{

    try
    {
        CharValExp      test_char;
        UCharValExp     test_uchar;
        ShortValExp     test_short;
        UShortValExp    test_ushort;
        Int32ValExp     test_int;
        UInt32ValExp    test_uint;
        Int64ValExp     test_int64;
        UInt64ValExp    test_uint64;
            
        cout<<test_char.GetTypeInfo().first<<", "<<test_char.IsValueSigned()<<", "<<test_char.GetMinAs64i()<<", "<<test_char.GetMaxAs64ui()<<endl;
        cout<<test_uchar.GetTypeInfo().first<<", "<<test_uchar.IsValueSigned()<<", "<<test_uchar.GetMinAs64i()<<", "<<test_uchar.GetMaxAs64ui()<<endl;
        cout<<test_short.GetTypeInfo().first<<", "<<test_short.IsValueSigned()<<", "<<test_short.GetMinAs64i()<<", "<<test_short.GetMaxAs64ui()<<endl;
        cout<<test_int64.GetTypeInfo().first<<", "<<test_int64.IsValueSigned()<<", "<<test_int64.GetMinAs64i()<<", "<<test_int64.GetMaxAs64ui()<<endl;
        cout<<test_uint64.GetTypeInfo().first<<", "<<test_uint64.IsValueSigned()<<", "<<test_uint64.GetMinAs64i()<<", "<<test_uint64.GetMaxAs64ui()<<endl;

        TestIntegerTypeOperations<CharValExp>();
        TestIntegerTypeOperations<UCharValExp>();
        TestIntegerTypeOperations<ShortValExp>();
        TestIntegerTypeOperations<UShortValExp>();
        TestIntegerTypeOperations<Int32ValExp>();
        TestIntegerTypeOperations<UInt32ValExp>();
        TestIntegerTypeOperations<Int64ValExp>();
        TestIntegerTypeOperations<UInt64ValExp>();
    }
    catch(...)
    {
        cout<<"exception: TestIntegerValue(): general exception"<<endl;
    }
}

}

}

}