#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

/**
 * 处理类似于000123的输入
 * @param  str [description]
 * @return     [description]
 */
std::string del_zeros_before(const std::string& str)
{
    auto i = str.find_first_not_of('0');
    /* 没有找到则返回string中定义的静态常量string::npos = -1 */
    if(i == string::npos)
        return std::string("");
    else
        return str.substr(i, str.size() - i);
}

/**
 * 字符串对应的大数相加，没有正负之分
 * @param  lhs [description]
 * @param  rhs [description]
 * @return     [description]
 */
const std::string str_add_str(const std::string& lhs, 
                      const std::string& rhs)
{
    if(lhs.empty())
        return rhs;
    if(rhs.empty())
        return lhs;

    int left_len =  lhs.size();
    int right_len =  rhs.size();
    // int len = max(left_len, right_len);
    bool isOverFlow = false;
    std::string ret;

    int i = left_len - 1;
    int j = right_len - 1;
    for(; i >= 0 || j >= 0; --i, --j)
    {
        int tmp = ((i >= 0 ? lhs[i] : '0') - '0') + 
                  ((j >= 0 ? rhs[j] : '0') - '0');
        if(isOverFlow)
            tmp++;
        if(tmp >= 10)
        {
            isOverFlow = true;
            ret.insert(ret.begin(), tmp - 10 + '0');
        }
        else
        {
            isOverFlow = false;
            ret.insert(ret.begin(), tmp + '0');
        }
    }

    return ret;
}

/**
 * 减数和被减数大小任意，但是没有处理字符串前面多余的零
 * @param  lhs  [description]
 * @param  rhs  [description]
 * @param  flag [记录相减之后的符号，true表示正数]
 * @return      [description]
 */
const std::string str_minus_str(const std::string& lhs, 
                      const std::string& rhs, bool& flag)
{
    int left_len =  lhs.size();
    int right_len =  rhs.size();
    bool isOverFlow = false;
    std::string ret;


    int i = left_len - 1;
    int j = right_len - 1;
    for(; i >= 0 || j >= 0; --i, --j)
    {
        int tmp = ((i >= 0 ? lhs[i] : '0') - '0') - 
                  ((j >= 0 ? rhs[j] : '0') - '0');
        if(isOverFlow)
            --tmp;
        if(tmp < 0)
        {
            isOverFlow = true;
            ret.insert(ret.begin(), tmp + 10 + '0');
        }
        else
        {
            isOverFlow = false;
            ret.insert(ret.begin(), tmp + '0');
        }
    }
    int ret_len = ret.size();
    if(isOverFlow)
    {
        for (int k = 0; k < ret_len - 1; ++k)
        {
            ret[k] = 9 - (ret[k] - '0') + '0';
        }
        ret[ret_len - 1] = 10 - (ret[ret_len - 1] - '0') + '0';
        flag = false;
    }
    else
        flag = true;
    return ret;
}

class bigNum
{
public:
    bigNum();
    bigNum(const std::string& str);
    bigNum(const bigNum& other);
    bigNum& operator=(const bigNum& other);
    bigNum operator++();
    bigNum operator++(int);
    bigNum operator--();
    bigNum operator--(int);


    std::string str() const { return val; }
    bool getSymb() const 
    { 
        return symb;
    }
    void setSymb(bool s) { symb = s; }

private:
    void init(const std::string& str);

private:
    std::string val;
    bool symb {true};
};

void bigNum::init(const std::string& str)
{
    if(str.empty())
        val = "";
    else
    {
        if(str[0] >= '0' && str[0] <= '9')
        {
            val = del_zeros_before(str);
        }
        else
        {
            switch(str[0])
            {
                case '-' : 
                        symb = false;
                        break;
                case '+' :
                        symb = true;
                        break;
                default :
                        throw(std::logic_error("非法的正负号！")); 
                        break;
            }
            val = del_zeros_before(str.substr(1, str.size() - 1));
        }       
    }
}

bigNum::bigNum() : val("") {}
bigNum::bigNum(const std::string& str)
{
    try
    {
        init(str);
    }
    catch(std::logic_error& e)
    {
        std::cout << e.what() << endl;
        val = "";
    }
}


bigNum::bigNum(const bigNum& other)
{
    val = other.str();
    symb = other.getSymb();
}

bigNum& bigNum::operator=(const bigNum& other)
{
    val = other.str();
    symb = other.getSymb();
    return *this;
}

bigNum bigNum::operator++()
{
    std::string tmp;
    if(symb)
        tmp = str_add_str(val, "1");
    else
    {
        bool flag = true;
        tmp = str_minus_str(val, "1", flag);
    }
    val = tmp;
    return *this;
}
bigNum bigNum::operator++(int)
{

}
bigNum bigNum::operator--()
{

}
bigNum bigNum::operator--(int)
{

}

ostream& operator<<(ostream& os, const bigNum& num)
{
    if(!num.getSymb())
        os << "-";
    os << num.str();
    return os;
}

/**
 * 还得考虑负数的情况
 */
const bigNum operator+(const bigNum& lhs, const bigNum& rhs)
{
    if(lhs.getSymb() == rhs.getSymb())
    {
        string tmp = str_add_str(lhs.str(), rhs.str());
        bigNum ret = bigNum(tmp);
        ret.setSymb(lhs.getSymb());
        return ret;
    }
    //异号相加等于相减
    else 
    {
        bool flag = true;
        string tmp = str_minus_str(lhs.str(), rhs.str(),flag);
        bigNum ret = bigNum(tmp);
        if(lhs.getSymb() == flag)
            ret.setSymb(true);
        else
            ret.setSymb(false);
        return ret;
    }
}

const bigNum operator-(const bigNum& lhs, const bigNum& rhs)
{
    std::string left_str = lhs.str();
    std::string right_str = rhs.str();
    string tmp;
    //异号相减等于相加
    if(lhs.getSymb() != rhs.getSymb())
    {
        tmp = str_add_str(left_str, right_str);
        bigNum ret = bigNum(tmp);
        ret.setSymb(lhs.getSymb());
        return ret;
    }
    else
    {
        bool flag = true;
        tmp = str_minus_str(left_str, right_str, flag);
        bigNum ret = bigNum(tmp);
        if(lhs.getSymb() == flag)
            ret.setSymb(true);
        else
            ret.setSymb(false);
        return ret;
    }

}




int main(int argc, char const *argv[])
{
    bigNum a = bigNum("-00235");    
    bigNum b = bigNum("-012"); 
    cout << a << endl;
    cout << b << endl;
    cout << a + b << endl;
    cout << b + a << endl;
    cout << a - b << endl;
    cout << b - a << endl;

    // bool flag = true;
    // string s = str_minus_str("32", "", flag);
    // if(!flag)
    //     cout << "-" << s << endl;

    
    return 0;
}