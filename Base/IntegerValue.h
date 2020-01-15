#ifndef INTEGER_VALUE_CLASS_HEADER
#define INTEGER_VALUE_CLASS_HEADER

#define NOMINMAX

#include <limits>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <stdint.h>

// Description: a class for performing operations on integers with 
// overflow control, safe comparisons, and conversions

namespace ToolSetLib
{

namespace Base
{

template< typename value_type, bool is_throw_exception>
class IntegerValue
{
public:

    class VersionInfo
    {
    public:
        static const uint32_t api =         0;
        static const uint32_t features =    2;
        static const uint32_t revision =    2;
    };

private:

    value_type  m_integer_value = 0;

    bool        m_is_overflow =   false;


    unsigned char
    GetAbsoluteValue(char a_value) const
    {
        if( a_value >= 0)
            return (unsigned char) a_value;

        if( a_value == std::numeric_limits<decltype(a_value)>::min())
        {
            static_assert( ((char)-127-1) == std::numeric_limits<decltype(a_value)>::min(), "unsupported char min value");

            return (unsigned char) 128;
        }

        return  (unsigned char)((char) 0 - a_value);
    }


    unsigned char
    GetAbsoluteValue(unsigned char a_value) const
    {
        return (unsigned char) a_value;
    }


    unsigned short
    GetAbsoluteValue(short a_value) const
    {
        if( a_value >= 0)
            return (unsigned short) a_value;
        
        if( a_value == std::numeric_limits<decltype(a_value)>::min())
        {
            static_assert( ((short)-32767-1) == std::numeric_limits<decltype(a_value)>::min(), "unsupported short min value");

            return (unsigned short) 32768;
        }

        return  (unsigned short)((short) 0 - a_value);
    }


    unsigned short
    GetAbsoluteValue(unsigned short a_value) const
    {
        return (unsigned short) a_value;
    }


    /*
    unsigned long
    GetAbsoluteValue(long a_value) const
    {
        if( a_value >= 0)
            return (unsigned long) a_value;

        if( a_value == std::numeric_limits<decltype(a_value)>::min())
        {
            static_assert( ((long)-2147483647 - 1) == std::numeric_limits<decltype(a_value)>::min(),"unsupported long min value");
            
            return (unsigned long)2147483648u;
        }

        return  (unsigned long)((long) 0 - a_value);
    }


    unsigned long
    GetAbsoluteValue(unsigned long a_value) const
    {
        return (unsigned long) a_value;
    }
    */


    unsigned int
    GetAbsoluteValue(int a_value) const
    {
        if( a_value >= 0)
            return (unsigned int) a_value;

        if( a_value == std::numeric_limits<decltype(a_value)>::min())
        {
            static_assert( ((int)-2147483647 - 1) == std::numeric_limits<decltype(a_value)>::min(),"unsupported int min value");
            
            return (unsigned int)2147483648u;
        }

        return  (unsigned int)((int) 0 - a_value);
    }


    unsigned int
    GetAbsoluteValue(unsigned int a_value) const
    {
        return (unsigned int) a_value;
    }


    uint64_t
    GetAbsoluteValue(int64_t a_value) const
    {
        if( a_value >= 0)
            return (uint64_t) a_value;
        
        if( a_value == std::numeric_limits<decltype(a_value)>::min())
        {
            static_assert( ((int64_t)-9223372036854775807LL - 1) == std::numeric_limits<decltype(a_value)>::min(),"unsupported int64_t min value");
            
            return ( 9223372036854775808ULL);
        }

        return (uint64_t)((int64_t) 0 - a_value);
    }


    uint64_t
    GetAbsoluteValue(uint64_t a_value) const
    {
        return (uint64_t) a_value;
    }


    template <typename in_value_type>
    IntegerValue<value_type, is_throw_exception>& 
    BaseAdd( in_value_type a_value)
    {
        //cout<<"OperatorAdd( in_value_type a_value) "<<a_value<<endl; //<-- remove test code

        static_assert(      std::is_same<in_value_type, char>::value           == true 
                        ||  std::is_same<in_value_type, short>::value          == true 
                        ||  std::is_same<in_value_type, int>::value            == true
                        ||  std::is_same<in_value_type, long>::value           == true
                        ||  std::is_same<in_value_type, int64_t>::value        == true 
                        ||  std::is_same<in_value_type, unsigned char>::value  == true 
                        ||  std::is_same<in_value_type, unsigned short>::value == true 
                        ||  std::is_same<in_value_type, unsigned int>::value   == true
                        ||  std::is_same<in_value_type, unsigned long>::value  == true
                        ||  std::is_same<in_value_type, uint64_t>::value       == true,
            "IntegerValue()::BaseAdd(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue::BaseAdd(): base value is overflow");
        }
        
        if( a_value == 0)
            return (*this);

        if(     m_integer_value >=  0
            &&  a_value         >=  0)
        {
            if( GetAbsoluteValue(a_value) > (GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::max()) - GetAbsoluteValue(m_integer_value)))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseAdd(): positive overflow");

                return (*this);
            }

            m_integer_value = m_integer_value + (decltype(m_integer_value))a_value;

            return (*this);
        }

        if(     m_integer_value >=  0
            &&  a_value         <=  0)
        {
            auto abs_a_value = GetAbsoluteValue(a_value);

            //cout<<"-- here: "<<(int)a_value<<", "<<(int)abs_a_value<<", "<<(int)GetAbsoluteValue(m_integer_value)<<", "<<std::numeric_limits<decltype(m_integer_value)>::is_signed<<endl; //<-- remove test code

            if( GetAbsoluteValue(m_integer_value) >= abs_a_value)
            {
                //cout<<"-- here: 1.1 "<<endl;//<-- remove test code
                m_integer_value = m_integer_value - (decltype(m_integer_value)) abs_a_value;

                return (*this);
            }
            
            //cout<<"-- here: 1.2 "<<endl;//<-- remove test code

            if( std::numeric_limits<decltype(m_integer_value)>::is_signed == false)
            {
               // cout<<"-- here: 1.3 "<<endl;//<-- remove test code

                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseAdd(): negative result for unsigned value");

                return (*this);
            }

            //cout<<"-- here: 1.4 "<<endl;//<-- remove test code

            decltype(abs_a_value) negative_value_abs = abs_a_value - GetAbsoluteValue(m_integer_value);

            if( negative_value_abs > GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min()))
            {
                //cout<<"-- here: 1.5 "<<endl;//<-- remove test code

                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseAdd(): negative overflow");

                return (*this);
            }

            //cout<<"-- here: 1.6 "<<endl;//<-- remove test code

            if( negative_value_abs == GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min()))
            {
                m_integer_value = std::numeric_limits<decltype(m_integer_value)>::min();
                return (*this);
            }

            m_integer_value =   (decltype(m_integer_value)) 0
                            -   (decltype(m_integer_value)) negative_value_abs;

            return (*this);
        }

        if(     m_integer_value <= 0
            &&  a_value         >= 0)
        {
            auto abs_num_value = GetAbsoluteValue(m_integer_value);

            if( abs_num_value >= GetAbsoluteValue(a_value))
            {
                m_integer_value = m_integer_value + (decltype(m_integer_value)) a_value;

                return (*this);
            }
            
            auto positive_value = a_value - abs_num_value;

            if( GetAbsoluteValue(positive_value) > GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::max()))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseAdd(): positive overflow for source negative value");

                return (*this);
            }

            m_integer_value =   (decltype(m_integer_value)) positive_value;

            return (*this);
        }

        if(     m_integer_value <= 0
            &&  a_value         <= 0)
        {
            if( a_value < (std::numeric_limits<decltype(m_integer_value)>::min() - m_integer_value))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseAdd(): negative overflow for negative values");
                
                return (*this);
            }

            m_integer_value = m_integer_value + (decltype(m_integer_value))a_value;

            return (*this);
        }
        
        m_is_overflow = true;

        if( is_throw_exception == true)
            throw std::runtime_error("IntegerValue::BaseAdd(): internal error: an undefined result");

        return (*this);
    }


    template <typename in_value_type>
    IntegerValue<value_type, is_throw_exception>& 
    BaseSub( in_value_type a_value)
    {
        //cout<<"OperatorSub( in_value_type a_value) "<<m_numeric_value<<", "<<a_value<< endl; //<-- remove test code

        static_assert(      std::is_same<in_value_type, char>::value           == true 
                        ||  std::is_same<in_value_type, short>::value          == true 
                        ||  std::is_same<in_value_type, int>::value            == true
                        ||  std::is_same<in_value_type, long>::value           == true
                        ||  std::is_same<in_value_type, int64_t>::value        == true 
                        ||  std::is_same<in_value_type, unsigned char>::value  == true 
                        ||  std::is_same<in_value_type, unsigned short>::value == true 
                        ||  std::is_same<in_value_type, unsigned int>::value   == true
                        ||  std::is_same<in_value_type, unsigned long>::value  == true
                        ||  std::is_same<in_value_type, uint64_t>::value       == true,
            "IntegerValue()::BaseSub(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::BaseSub(): base value is overflow");

            return (*this);
        }
        
        if( a_value == 0)
            return (*this);

        if(     m_integer_value <=  0
            &&  a_value         >=  0)
        {
            if( GetAbsoluteValue(a_value) > GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min() - m_integer_value))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseSub(): negative overflow");
                
                return (*this);
            }

            m_integer_value = m_integer_value - (decltype(m_integer_value))a_value;

            return (*this);
        }

        if(     m_integer_value <=  0
            &&  a_value         <=  0)
        {
            if( m_integer_value <= a_value)
            {
                m_integer_value = m_integer_value - a_value;

                return (*this);
            }

            auto sub_value = GetAbsoluteValue(a_value) - GetAbsoluteValue((decltype(a_value)) m_integer_value);

            if( sub_value > GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::max()))
            {
                m_is_overflow = true;

                throw std::runtime_error("IntegerValue::BaseSub(): positive overflow for negative value");

                return (*this);
            }

            m_integer_value =   (decltype(m_integer_value)) sub_value;

            return (*this);
        }

        if(     m_integer_value >= 0
            &&  a_value         >= 0)
        {
            if( GetAbsoluteValue(m_integer_value) >= GetAbsoluteValue(a_value))
            {
                m_integer_value =   m_integer_value - (decltype(m_integer_value)) a_value;

                return (*this);
            }

            if( std::numeric_limits<decltype(m_integer_value)>::is_signed == false)
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseSub(): negative overflow for unsigned value");

                return (*this);
            }

            auto negative_value_abs = a_value - (decltype(a_value))m_integer_value;

            if(GetAbsoluteValue(negative_value_abs) > GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min()))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseSub(): negative overflow for positive value");
                
                return (*this);
            }

            m_integer_value =   (decltype(m_integer_value)) 0
                            -   (decltype(m_integer_value)) negative_value_abs;

            return (*this);
        }
        
        if(     m_integer_value >= 0
            &&  a_value         <= 0)
        {
            auto abs_a_value = GetAbsoluteValue(a_value);

            if( abs_a_value > (std::numeric_limits<decltype(m_integer_value)>::max() - m_integer_value))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseSub(): positite overflow");

                return (*this);
            }

            m_integer_value = m_integer_value + (decltype(m_integer_value))abs_a_value;

            return (*this);
        }

        m_is_overflow = true;

        if( is_throw_exception == true)
            throw std::runtime_error("IntegerValue::BaseSub(): internal error: an undefined result");

        return (*this);
    }


    template <typename param_type>
    IntegerValue<value_type, is_throw_exception>& 
    BaseMul( param_type a_value)
    {
        //cout<<"Mul( in_value_type a_value) "<<a_value<<endl; //<-- remove test code

        static_assert(  std::is_same<param_type,char>::value           == true
                    ||  std::is_same<param_type,short>::value          == true
                    ||  std::is_same<param_type,int>::value            == true
                    ||  std::is_same<param_type,long>::value           == true
                    ||  std::is_same<param_type,int64_t>::value        == true
                    ||  std::is_same<param_type,unsigned char>::value  == true
                    ||  std::is_same<param_type,unsigned short>::value == true
                    ||  std::is_same<param_type,unsigned int>::value   == true
                    ||  std::is_same<param_type,unsigned long>::value  == true
                    ||  std::is_same<param_type,uint64_t>::value       == true,
            "IntegerValue::BaseMul(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::BaseMul(): base value is overflow");

            return (*this);
        }
        
        if( a_value == 0)
        {
            m_integer_value = 0;

            return (*this);
        }

        if( a_value ==  1)
        {
            return (*this);
        }

        if( m_integer_value ==  0)
        {
            return (*this);
        }


        if( a_value < 0)
        {
            if( std::numeric_limits<decltype(m_integer_value)>::is_signed == false)
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseMul(): negative result for unsigned value");

                return (*this);
            }
        }
        

        if(     m_integer_value >  0
            &&  a_value         >  0)
        {
            auto max_mutliplier = std::numeric_limits<decltype(m_integer_value)>::max() / m_integer_value;

            if( GetAbsoluteValue(a_value) > GetAbsoluteValue(max_mutliplier))
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseMul(): positive overflow");
                
                return (*this);
            }

            m_integer_value = m_integer_value * (decltype(m_integer_value))a_value;

            return (*this);
        }


        if(     m_integer_value >  0
            &&  a_value         <  0)
        {
            auto max_mutliplier = GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min()) / GetAbsoluteValue(m_integer_value);

            if( GetAbsoluteValue(a_value) > max_mutliplier)
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseMul(): negative overflow for positive value");
                
                return (*this);
            }

            m_integer_value = m_integer_value * (decltype(m_integer_value))a_value;

            return (*this);
        }

        if(     m_integer_value < 0
            &&  a_value         > 0)
        {
            auto max_mutliplier = GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min()) / GetAbsoluteValue(m_integer_value);

            if( GetAbsoluteValue(a_value) > max_mutliplier)
            {
                m_is_overflow = true;

                if(is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseMul(): negative overflow");
                
                return (*this);
            }

            m_integer_value = m_integer_value * (decltype(m_integer_value))a_value;

            return (*this);
        }

        if(     m_integer_value < 0
            &&  a_value         < 0)
        {
            auto max_mutliplier = GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::max()) / GetAbsoluteValue(m_integer_value);

            auto abs_a_value = GetAbsoluteValue(a_value);

            if( abs_a_value > max_mutliplier)
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseMul(): positive overflow for negative value");
                
                return (*this);
            }

            m_integer_value = m_integer_value * (decltype(m_integer_value))a_value;

            return (*this);
        }
        
        m_is_overflow = true;

        if( is_throw_exception == true)
            throw std::runtime_error("IntegerValue::BaseMul(): internal error: an undefined result");

        return (*this);
    }


    template <typename param_type>
    IntegerValue<value_type, is_throw_exception>& 
    BaseDiv( param_type a_value)
    {
        //cout<<"Div( in_value_type a_value) "<<a_value<<endl; //<-- remove test code

        static_assert(  std::is_same<param_type,char>::value           == true
                    ||  std::is_same<param_type,short>::value          == true
                    ||  std::is_same<param_type,int>::value            == true
                    ||  std::is_same<param_type,long>::value           == true
                    ||  std::is_same<param_type,int64_t>::value        == true
                    ||  std::is_same<param_type,unsigned char>::value  == true
                    ||  std::is_same<param_type,unsigned short>::value == true
                    ||  std::is_same<param_type,unsigned int>::value   == true
                    ||  std::is_same<param_type,unsigned long>::value  == true
                    ||  std::is_same<param_type,uint64_t>::value       == true,
            "IntegerValue::BaseDiv(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::BaseMul(): base value is overflow");

            return (*this);
        }
        
        if( a_value == 0)
        {
            m_is_overflow = true;

            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::BaseDiv(): division by zero");

            return (*this);
        }

        if( a_value ==  1)
        {
            return (*this);
        }

        if( m_integer_value ==  0)
        {
            return (*this);
        }


        if( a_value < 0)
        {
            if( std::numeric_limits<decltype(m_integer_value)>::is_signed == false)
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseDiv(): negative result for unsigned value");

                return (*this);
            }
        }
        

        if(     m_integer_value >  0
            &&  a_value         >  0)
        {
            if( GetAbsoluteValue(a_value) > GetAbsoluteValue(m_integer_value))
            {
                m_integer_value = 0;
                return (*this);
            }

            m_integer_value = m_integer_value / (decltype(m_integer_value))a_value;

            return (*this);
        }


        if(     m_integer_value >  0
            &&  a_value         <  0)
        {
            auto abs_a_value = GetAbsoluteValue(a_value);

            if( abs_a_value > GetAbsoluteValue(m_integer_value))
            {
                m_integer_value = 0;
                return (*this);
            }

            m_integer_value = m_integer_value / (decltype(m_integer_value))a_value;

            return (*this);
        }

        if(     m_integer_value < 0
            &&  a_value         > 0)
        {
            auto abs_integer_value = GetAbsoluteValue(m_integer_value);

            if( GetAbsoluteValue(a_value) > abs_integer_value)
            {
                m_integer_value = 0;
                return (*this);
            }

            if( a_value == abs_integer_value)
            {
                m_integer_value = -1;
                return (*this);
            }

            m_integer_value = m_integer_value / (decltype(m_integer_value))a_value;

            return (*this);
        }

        if(     m_integer_value < 0
            &&  a_value         < 0)
        {
            if( GetAbsoluteValue(a_value) > GetAbsoluteValue(m_integer_value))
            {
                m_integer_value = 0;
                return (*this);
            }

            if(     m_integer_value ==  std::numeric_limits<decltype(m_integer_value)>::min()
                &&  a_value         ==  -1)
            {
                m_is_overflow = true;

                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::BaseDiv(): positive overflow for negative value");
                
                return (*this);
            }

            m_integer_value = m_integer_value / (decltype(m_integer_value))a_value;

            return (*this);
        }
        
        m_is_overflow = true;

        if( is_throw_exception == true)
            throw std::runtime_error("IntegerValue::BaseDiv(): internal error: an undefined result");

        return (*this);
    }

public:
 
    IntegerValue()
    {
        static_assert(      std::is_same<value_type, char>::value           == true 
                        ||  std::is_same<value_type, short>::value          == true 
                        ||  std::is_same<value_type, int>::value            == true
                        ||  std::is_same<value_type, long>::value           == true
                        ||  std::is_same<value_type, int64_t>::value        == true 
                        ||  std::is_same<value_type, unsigned char>::value  == true 
                        ||  std::is_same<value_type, unsigned short>::value == true 
                        ||  std::is_same<value_type, unsigned int>::value   == true
                        ||  std::is_same<value_type, unsigned long>::value  == true
                        ||  std::is_same<value_type, uint64_t>::value       == true,
            "IntegerValue::IntegerValue(): unsupported template param type");
    }


    template <typename value_type2, bool a_is_throw_exception>
    IntegerValue(const IntegerValue<value_type2, a_is_throw_exception>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue(): (const IntegerValue<value_type, bool>& a_value): overflow argument");

            return;
        }

        m_integer_value = 0;

        BaseAdd<value_type2>(a_value.m_integer_value);

        return;
    }


    IntegerValue(int64_t a_value)
    {
        BaseAdd<int64_t>(a_value);
    }


    IntegerValue(uint64_t a_value)
    {
        BaseAdd<uint64_t>(a_value);
    }


    IntegerValue(int a_value)
    {
        BaseAdd<int>(a_value);
    }


    IntegerValue(unsigned int a_value)
    {
        BaseAdd<unsigned int>(a_value);
    }


    IntegerValue(short a_value)
    {
        BaseAdd<short>(a_value);
    }


    IntegerValue(unsigned short a_value)
    {
        BaseAdd<unsigned short>(a_value);
    }


    IntegerValue(char a_value)
    {
        BaseAdd<char>(a_value);
    }


    IntegerValue(unsigned char a_value)
    {
        BaseAdd<unsigned char>(a_value);
    }


    bool IsOverflow() const
    {
        return m_is_overflow;
    }


    IntegerValue<value_type,is_throw_exception>& Clear()
    {
        m_is_overflow =   false;
        m_integer_value = 0;

        return (*this);
    }


    void operator = (int64_t a_value)
    {
        Clear();
        BaseAdd<int64_t>(a_value);
    }


    void operator = (uint64_t a_value)
    {
        Clear();
        BaseAdd<uint64_t>(a_value);
    }


    void operator = (int a_value)
    {
        Clear();
        BaseAdd<int>(a_value);
    }


    void operator = (unsigned int a_value)
    {
        Clear();
        BaseAdd<unsigned int>(a_value);
    }


    void operator = (short a_value)
    {
        Clear();
        BaseAdd<short>(a_value);
    }


    void operator = (unsigned short a_value)
    {
        Clear();
        BaseAdd<unsigned short>(a_value);
    }


    void operator = (char a_value)
    {
        Clear();
        BaseAdd<char>(a_value);
    }


    void operator = (unsigned char a_value)
    {
        Clear();
        BaseAdd<unsigned char>(a_value);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& Add(const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& Add(const IntegerValue<param_type, a_is_throw>& ): overflow argument");
        }

        BaseAdd<param_type>(a_value.GetValue((param_type)0));

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& Add(param_type a_value)
    {
        BaseAdd<param_type>(a_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& operator + (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator +: overflow argument");
        }

        BaseAdd<param_type>(a_value.m_integer_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& operator += (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator +=: overflow argument");
        }

        BaseAdd<param_type>(a_value.m_integer_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& operator + (param_type a_value)
    {
        BaseAdd<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type,is_throw_exception>& operator += (param_type a_value)
    {
        BaseAdd<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& operator -(param_type a_value)
    {
        BaseSub<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type,is_throw_exception>& operator -= (param_type a_value)
    {
        BaseSub<param_type>(a_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& Sub(const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& Sub(const IntegerValue<param_type, a_is_throw>& ) : overflow argument");
        }

        BaseSub<param_type>(a_value.GetValue(0));

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type,is_throw_exception>& Sub(param_type a_value)
    {
        BaseSub<param_type>(a_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& operator - (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator - : overflow argument");
        }

        BaseSub<param_type>(a_value.m_integer_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& operator -= (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator -= : overflow argument");
        }

        BaseSub<param_type>(a_value.m_numeric_value);

        return (*this);
    }


    //>-- begin multiple operators


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>&  Mul(const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& Mul(const IntegerValue<param_type, a_is_throw>& ) : overflow argument");
        }

        BaseMul<param_type>(a_value.GetValue(0));

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& Mul(param_type a_value)
    {
        BaseMul<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& operator * (param_type a_value)
    {
        BaseMul<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type,is_throw_exception>& 
    operator *= (param_type a_value)
    {
        BaseMul<param_type>(a_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& 
    operator * (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator *: overflow argument");
        }

        BaseMul<param_type>(a_value.m_integer_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& 
    operator *= (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator *=: overflow argument");
        }

        BaseMul<param_type>(a_value.m_numeric_value);

        return (*this);
    }

    //<-- end multiple operators


    //>-- begin divide operators

    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& Div(const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& Div(const IntegerValue<param_type, a_is_throw>& ): overflow argument");
        }

        BaseDiv<param_type>(a_value.GetValue(0));

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& Div(param_type a_value)
    {
        BaseDiv<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type, is_throw_exception>& operator / (param_type a_value)
    {
        BaseDiv<param_type>(a_value);

        return (*this);
    }


    template<typename param_type>
    IntegerValue<value_type,is_throw_exception>& operator /= (param_type a_value)
    {
        BaseDiv<param_type>(a_value);

        return (*this);
    }

    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& operator / (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator / : overflow argument");
        }

        BaseDiv<param_type>(a_value.m_integer_value);

        return (*this);
    }


    template<typename param_type, bool a_is_throw>
    IntegerValue<value_type, is_throw_exception>& operator /= (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            m_is_overflow = true;

            if(is_throw_exception == false)
                return (*this);

            throw std::runtime_error("IntegerValue& (const IntegerValue<param_type, a_is_throw>& ) operator /= : overflow argument");
        }

        BaseDiv<param_type>(a_value.m_numeric_value);

        return (*this);
    }

    //<-- end divide operators


    //>-- begin conversion methods

    int64_t GetInt64()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetInt64(): need to use GetInt64(int64_t default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt64(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int64_t>::min())
            {
                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::GetInt64(): source value less than int64_t::min()");

                return 0;
            }

            return (int64_t) m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int64_t>::max()))
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt64(): source value more than int64_t::max()");

            return 0;
        }

        return (int64_t) m_integer_value;
    }


    int64_t GetInt64( int64_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return a_default_value;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int64_t>::min())
            {
                return a_default_value;
            }

            return (int64_t)m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int64_t>::max()))
        {
            return a_default_value;
        }

        return (int64_t) m_integer_value;
    }


    uint64_t GetUInt64()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetUInt64(): need to use GetUInt64(uint64_t a_default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt64(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt64(): negative source value");

            return 0;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint64_t>::max())
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt64(): source value more than uint64_t::max()");

            return 0;
        }

        return (uint64_t) m_integer_value;
    }

    
    uint64_t GetUInt64( uint64_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            return a_default_value;
        }

        if( GetAbsoluteValue( m_integer_value) > std::numeric_limits<uint64_t>::max())
        {
            return a_default_value;
        }

        return (uint64_t) m_integer_value;
    }

    
    int32_t GetInt32()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetInt32(): need to use GetInt32(int32_t default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt32(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int32_t>::min())
            {
                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::GetInt32(): source value less than int32_t::min()");

                return 0;
            }

            return (int32_t) m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int32_t>::max()))
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt32(): source value more than int32_t::max()");

            return 0;
        }

        return (int32_t) m_integer_value;
    }


    int32_t GetInt32( int32_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return a_default_value;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int32_t>::min())
            {
                return a_default_value;
            }

            return (int32_t)m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int32_t>::max()))
        {
            return a_default_value;
        }

        return (int32_t) m_integer_value;
    }


    uint32_t GetUInt32()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetUInt32(): need to use GetUInt32(uint32_t a_default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt32(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt32(): negative source value");

            return 0;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint32_t>::max())
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt32(): source value more than uint32_t::max()");

            return 0;
        }

        return (uint32_t) m_integer_value;
    }

    
    uint32_t GetUInt32( uint32_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            return a_default_value;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint32_t>::max())
        {
            return a_default_value;
        }

        return (uint32_t) m_integer_value;
    }


    int16_t GetInt16()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetInt16(): need to use GetInt16(int16_t default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt16(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int16_t>::min())
            {
                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::GetInt16(): source value less than int16_t::min()");

                return 0;
            }

            return (int16_t) m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int16_t>::max()))
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt16(): source value more than int16_t::max()");

            return 0;
        }

        return (int16_t) m_integer_value;
    }


    int16_t GetInt16( int16_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return a_default_value;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int16_t>::min())
            {
                return a_default_value;
            }

            return (int16_t)m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int16_t>::max()))
        {
            return a_default_value;
        }

        return (int16_t) m_integer_value;
    }


    uint16_t GetUInt16()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetUInt16(): need to use GetUInt16(uint16_t a_default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt16(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt16(): negative source value");

            return 0;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint16_t>::max())
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt16(): source value more than uint16_t::max()");

            return 0;
        }

        return (uint16_t) m_integer_value;
    }

    
    uint16_t GetUInt16( uint16_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            return a_default_value;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint16_t>::max())
        {
            return a_default_value;
        }

        return (uint16_t) m_integer_value;
    }

    
    int8_t GetInt8()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetInt8(): need to use GetInt8(int8_t default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt8(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int8_t>::min())
            {
                if( is_throw_exception == true)
                    throw std::runtime_error("IntegerValue::GetInt8(): source value less than int8_t::min()");

                return 0;
            }

            return (int8_t) m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int8_t>::max()))
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetInt8(): source value more than int8_t::max()");

            return 0;
        }

        return (int8_t) m_integer_value;
    }


    int8_t GetInt8( int8_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return a_default_value;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( m_integer_value < std::numeric_limits<int8_t>::min())
            {
                return a_default_value;
            }

            return (int8_t)m_integer_value;
        }

        if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(std::numeric_limits<int8_t>::max()))
        {
            return a_default_value;
        }

        return (int8_t) m_integer_value;
    }


    uint8_t GetUInt8()
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetUInt8(): need to use GetUInt8(uint8_t a_default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt8(): overflow source value");

            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt8(): negative source value");

            return 0;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint8_t>::max())
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetUInt8(): source value more than uint8_t::max()");

            return 0;
        }

        return (uint8_t) m_integer_value;
    }

    
    uint8_t GetUInt8( uint8_t a_default_value)
    {
        if( m_is_overflow == true)
        {
            return 0;
        }

        if( m_integer_value == 0)
            return 0;

        if( m_integer_value < 0)
        {
            return a_default_value;
        }

        if( GetAbsoluteValue(m_integer_value) > std::numeric_limits<uint8_t>::max())
        {
            return a_default_value;
        }

        return (uint8_t) m_integer_value;
    }

    //<-- end conversion methods

    
    value_type GetValue() const
    {
        static_assert( is_throw_exception == true, "IntegerValue::GetValue(): need to use GetValue(value_type default_value) method for no throw code");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetValue(): overflow source value");

            return 0;
        }

        return m_integer_value;
    }

    
    value_type GetValue( value_type a_default_value) const
    {
        if( m_is_overflow == true)
        {
            return a_default_value;
        }
 
        return m_integer_value;
    }


    //<-- begin compare methods

    template <typename ls_value_type>
    bool IsLessThan( ls_value_type a_value, bool ais_argument_exception) const
    {
        static_assert(      std::is_same<ls_value_type, char>::value           == true 
                        ||  std::is_same<ls_value_type, short>::value          == true 
                        ||  std::is_same<ls_value_type, int>::value            == true
                        ||  std::is_same<ls_value_type, long>::value           == true
                        ||  std::is_same<ls_value_type, int64_t>::value        == true 
                        ||  std::is_same<ls_value_type, unsigned char>::value  == true 
                        ||  std::is_same<ls_value_type, unsigned short>::value == true 
                        ||  std::is_same<ls_value_type, unsigned int>::value   == true
                        ||  std::is_same<ls_value_type, unsigned long>::value  == true
                        ||  std::is_same<ls_value_type, uint64_t>::value       == true,
            "IntegerValue()::IsLessThan(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::IsLessThan(): base value is overflow");

            return false;
        }

        if( ais_argument_exception == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::IsLessThan(): argument value is overflow");

            return false;
        }
        
        if( m_integer_value == 0 && a_value == 0)
            return false;

        if( m_integer_value == 0)
        {
            if( a_value > 0)
                return true;

            return false;
        }

        if(     m_integer_value >   0
            &&  a_value         >=  0)
        {
            if( GetAbsoluteValue(m_integer_value) < GetAbsoluteValue(a_value))
                return true;

            return false;
        }

        if(     m_integer_value >   0
            &&  a_value         <=  0)
        {
            return false;
        }

        if(     m_integer_value <   0
            &&  a_value         <=  0)
        {
            if( GetAbsoluteValue(m_integer_value) <= GetAbsoluteValue(a_value))
                return false;
            
            return true;
        }

        if(     m_integer_value <   0
            &&  a_value         >=  0)
        {
            return true;
        }

        if(     is_throw_exception      ==  true
            ||  ais_argument_exception  ==  true)
            throw std::runtime_error("IntegerValue::IsLessThan(): internal error: an undefined result");

        return false;
    }


    template<typename param_type, bool a_is_throw>
    bool 
    operator < (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("bool (const IntegerValue<param_type, a_is_throw>& ) operator < : overflow argument");

            return false;
        }

        return IsLessThan<param_type>( a_value.GetValue(0), a_value.IsOverflow());
    }


    template<typename param_type>
    bool 
    operator < (param_type a_value)
    {
        return IsLessThan<param_type>( a_value, false);
    }


    template <typename im_value_type>
    bool IsMoreThan( im_value_type a_value, bool ais_argument_exception) const
    {
        static_assert(      std::is_same<im_value_type, char>::value           == true 
                        ||  std::is_same<im_value_type, short>::value          == true 
                        ||  std::is_same<im_value_type, int>::value            == true
                        ||  std::is_same<im_value_type, long>::value           == true
                        ||  std::is_same<im_value_type, int64_t>::value        == true 
                        ||  std::is_same<im_value_type, unsigned char>::value  == true 
                        ||  std::is_same<im_value_type, unsigned short>::value == true 
                        ||  std::is_same<im_value_type, unsigned int>::value   == true
                        ||  std::is_same<im_value_type, unsigned long>::value  == true
                        ||  std::is_same<im_value_type, uint64_t>::value       == true,
            "IntegerValue()::IsMoreThan(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::IsMoreThan(): base value is overflow");

            return false;
        }

        if( ais_argument_exception == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::IsMoreThan(): argument value is overflow");

            return false;
        }
        
        if( m_integer_value == 0 && a_value == 0)
            return false;

        if( m_integer_value == 0)
        {
            if( a_value < 0)
                return true;

            return false;
        }

        if(     m_integer_value >   0
            &&  a_value         >=  0)
        {
            if( GetAbsoluteValue(m_integer_value) > GetAbsoluteValue(a_value))
                return true;

            return false;
        }

        if(     m_integer_value >   0
            &&  a_value         <=  0)
        {
            return true;
        }

        if(     m_integer_value <   0
            &&  a_value         <=  0)
        {
            if( GetAbsoluteValue(m_integer_value) < GetAbsoluteValue(a_value))
                return true;
            
            return false;
        }

        if(     m_integer_value <   0
            &&  a_value         >=  0)
        {
            return false;
        }

        if(     is_throw_exception      ==  true
            ||  ais_argument_exception  ==  true)
            throw std::runtime_error("IntegerValue::IsMoreThan(): internal error: an undefined result");

        return false;
    }


    template<typename param_type, bool a_is_throw>
    bool 
    operator > (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("bool (const IntegerValue<param_type, a_is_throw>& ) operator > : overflow argument");

            return false;
        }

        return IsMoreThan<param_type>( a_value.GetValue(0), a_value.IsOverflow());
    }


    template<typename param_type>
    bool 
    operator > (param_type a_value)
    {
        return IsMoreThan<param_type>( a_value, false);
    }


    template <typename eq_value_type>
    bool IsEqual( eq_value_type a_value, bool ais_argument_exception) const
    {
        static_assert(      std::is_same<value_type, char>::value           == true 
                        ||  std::is_same<value_type, short>::value          == true 
                        ||  std::is_same<value_type, int>::value            == true
                        ||  std::is_same<value_type, long>::value           == true
                        ||  std::is_same<value_type, int64_t>::value        == true 
                        ||  std::is_same<value_type, unsigned char>::value  == true 
                        ||  std::is_same<value_type, unsigned short>::value == true 
                        ||  std::is_same<value_type, unsigned int>::value   == true
                        ||  std::is_same<value_type, unsigned long>::value  == true
                        ||  std::is_same<value_type, uint64_t>::value       == true,
            "IntegerValue()::IsEqual(): unsupported value type");

        if( m_is_overflow == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::IsEqual(): base value is overflow");

            return false;
        }

        if( ais_argument_exception == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::IsEqual(): argument value is overflow");

            return false;
        }
        
        if( m_integer_value == 0 && a_value == 0)
            return true;

        if( m_integer_value == 0)
        {
            return false;
        }

        if(     (m_integer_value > 0 &&  a_value   >  0)
            ||  (m_integer_value < 0 &&  a_value   <  0))
        {
            if( GetAbsoluteValue(m_integer_value) == GetAbsoluteValue(a_value))
                return true;

            return false;
        }

        return false;
    }


    template<typename param_type, bool a_is_throw>
    bool 
    operator == (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("bool (const IntegerValue<param_type, a_is_throw>& ) operator == : overflow argument");

            return false;
        }

        return IsEqual<param_type>( a_value.GetValue(0), a_value.IsOverflow());
    }


    template<typename param_type>
    bool 
    operator == (param_type a_value)
    {
        return IsEqual<param_type>( a_value, false);
    }


    template<typename param_type, bool a_is_throw>
    bool 
    operator != (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("bool (const IntegerValue<param_type, a_is_throw>& ) operator != : overflow argument");

            return false;
        }

        if( true == IsEqual<param_type>( a_value.m_integer_value, a_value.IsOverflow()))
            return false;

        return true;
    }


    template<typename param_type>
    bool 
    operator != (param_type a_value)
    {
        if( true == IsEqual<param_type>( a_value, false))
            return false;

        return true;
    }


    template<typename param_type, bool a_is_throw>
    bool 
    operator >= (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("bool (const IntegerValue<param_type, a_is_throw>& ) operator >= : overflow argument");

            return false;
        }

        if( true == IsEqual<param_type>( a_value.GetValue(0), a_value.IsOverflow()))
            return true;
        
        return IsMoreThan<param_type>( a_value.GetValue(0), a_value.IsOverflow());
    }


    template<typename param_type>
    bool 
    operator >= (param_type a_value)
    {
        if( true == IsEqual<param_type>( a_value, false))
            return true;

        return IsMoreThan<param_type>( a_value, false);
    }


    template<typename param_type, bool a_is_throw>
    bool 
    operator <= (const IntegerValue<param_type, a_is_throw>& a_value)
    {
        if( a_value.IsOverflow() == true)
        {
            if( is_throw_exception == true)
                throw std::runtime_error("bool (const IntegerValue<param_type, a_is_throw>& ) operator <= : overflow argument");

            return false;
        }

        if( true == IsEqual<param_type>( a_value.GetValue(0), a_value.IsOverflow()))
            return true;
        
        return IsLessThan<param_type>( a_value.GetValue(0), a_value.IsOverflow());
    }


    template<typename param_type>
    bool 
    operator <= (param_type a_value)
    {
        if( true == IsEqual<param_type>( a_value, false))
            return true;

        return IsLessThan<param_type>( a_value, false);
    }

    //<-- end compare methods

    std::pair<const char*, bool>
    GetTypeInfo()
    {
        if( typeid(value_type) == typeid(char))
        {
            return {"char", true};
        }

        if( typeid(value_type) == typeid(short))
        {
            return {"short", true};
        }

        if(typeid(value_type) == typeid(int))
        {
            return {"int", true};
        }

        if(typeid(value_type) == typeid(long))
        {
            return {"long", true};
        }

        if(typeid(value_type) == typeid(int64_t))
        {
            return {"int64_t", true};
        }

        if(typeid(value_type) == typeid(int8_t))
        {
            return {"int8_t", true};
        }

        if(typeid(value_type) == typeid(int16_t))
        {
            return {"int16_t", true};
        }

        if(typeid(value_type) == typeid(int32_t))
        {
            return {"int32_t", true};
        }

        if( typeid(value_type) == typeid(unsigned char))
        {
            return {"char", false};
        }

        if( typeid(value_type) == typeid(unsigned short))
        {
            return {"short", false};
        }

        if(typeid(value_type) == typeid(unsigned int))
        {
            return {"int", false};
        }

        if(typeid(value_type) == typeid(unsigned long))
        {
            return {"long", false};
        }

        if(typeid(value_type) == typeid(uint64_t))
        {
            return {"uint64_t", false};
        }

        if(typeid(value_type) == typeid(uint8_t))
        {
            return {"uint8_t", false};
        }

        if(typeid(value_type) == typeid(uint16_t))
        {
            return {"uint16_t", false};
        }

        if(typeid(value_type) == typeid(uint32_t))
        {
            return {"uint32_t", false};
        }
    
        return {"unknown", false};
    }

    
    bool
    IsValueSigned()
    {
        return std::numeric_limits<decltype(m_integer_value)>::is_signed;
    }


    int64_t
    GetMinAs64i()
    {
        if( GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::min()) 
                > GetAbsoluteValue(std::numeric_limits<int64_t>::min()))
        {
            if(is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetMinAs64i(): internal error: base type min value is too small");

            return 1;
        }

        return (int64_t) std::numeric_limits<decltype(m_integer_value)>::min();
    }


    uint64_t
    GetMaxAs64ui()
    {
        if( GetAbsoluteValue(std::numeric_limits<decltype(m_integer_value)>::max()) 
            > std::numeric_limits<uint64_t>::max())
        {
            if( is_throw_exception == true)
                throw std::runtime_error("IntegerValue::GetMaxAs64ui(): internal error: base type max value too large");

            return 1;
        }

        return (uint64_t) std::numeric_limits<decltype(m_integer_value)>::max();
    }
};


template<typename param1_type, typename param2_type, bool a_is_throw>
bool operator <= (param1_type param1, const IntegerValue<param2_type,a_is_throw>& a_value)
{
    if( a_value.IsEqual(param1, false) == true)
        return true;

    if( a_value.IsMoreThan(param1, false) == true)
        return true;

    return false;
}


template<typename param1_type, typename param2_type, bool a_is_throw>
bool operator < (param1_type param1, const IntegerValue<param2_type,a_is_throw>& a_value)
{
    if( a_value.IsMoreThan(param1, false) == true)
        return true;

    return false;
}

template<typename param1_type,typename param2_type,bool a_is_throw>
bool operator >= (param1_type param1,const IntegerValue<param2_type,a_is_throw>& a_value)
{
    if(a_value.IsEqual(param1,false) == true)
        return true;

    if(a_value.IsLessThan(param1,false) == true)
        return true;

    return false;
}


template<typename param1_type,typename param2_type,bool a_is_throw>
bool operator > (param1_type param1,const IntegerValue<param2_type,a_is_throw>& a_value)
{
    if(a_value.IsLessThan(param1,false) == true)
        return true;

    return false;
}


template<typename param1_type,typename param2_type,bool a_is_throw>
bool operator == (param1_type param1,const IntegerValue<param2_type,a_is_throw>& a_value)
{
    if(a_value.IsEqual(param1,false) == true)
        return true;

    return false;
}


typedef IntegerValue<char,              false> CharVal;
typedef IntegerValue<unsigned char,     false> UCharVal;
typedef IntegerValue<int8_t,            false> Int8Val;
typedef IntegerValue<uint8_t,           false> UInt8Val;
typedef IntegerValue<short,             false> ShortVal;
typedef IntegerValue<unsigned short,    false> UShortVal;
typedef IntegerValue<int16_t,           false> Int16Val;
typedef IntegerValue<uint16_t,          false> UInt16Val;
typedef IntegerValue<int,               false> IntVal;
typedef IntegerValue<unsigned int,      false> UIntVal;
typedef IntegerValue<int32_t,           false> Int32Val;
typedef IntegerValue<uint32_t,          false> UInt32Val;
typedef IntegerValue<int64_t,           false> Int64Val;
typedef IntegerValue<uint64_t,          false> UInt64Val;

typedef IntegerValue<char,              true> CharValExp;
typedef IntegerValue<unsigned char,     true> UCharValExp;
typedef IntegerValue<short,             true> ShortValExp;
typedef IntegerValue<unsigned short,    true> UShortValExp;
typedef IntegerValue<int16_t,           true> Int16ValExp;
typedef IntegerValue<uint16_t,          true> UInt16ValExp;
typedef IntegerValue<int,               true> IntValExp;
typedef IntegerValue<unsigned int,      true> UIntValExp;
typedef IntegerValue<int32_t,           true> Int32ValExp;
typedef IntegerValue<uint32_t,          true> UInt32ValExp;
typedef IntegerValue<int64_t,           true> Int64ValExp;
typedef IntegerValue<uint64_t,          true> UInt64ValExp;

}

}

#endif