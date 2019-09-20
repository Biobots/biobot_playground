#include <future>
#include <iostream>

template<typename T, T v>
struct integral_constant
{
    static constexpr T value = v;
    typedef T value_type;
    typedef integral_constant type;
};

typedef integral_constant<int, 2> two_type;
typedef integral_constant<int, 6> six_type;

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

template<bool b, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T>
{
    typedef T type;
};

static_assert(two_type::value * 3 == six_type::value, "err");

template<class T>
struct is_const : public false_type {};

template<class T>
struct is_const<const T> : public true_type {};

template<class ...T> struct common_type;

template<class T>
struct common_type<T>
{
    //std::decay返回退化后的基�?类型
    typedef typename std::decay<T>::type type;
};

template<class T, class U>
struct common_type<T, U>
{
private:
    static T&& t();
    static U&& u();
    static bool f();
public:
    typedef typename std::decay<decltype(f() ? t() : u())>::type type;
};

template<class T, class U, class ...V>
struct common_type<T, U, V...>
{
    typedef typename common_type<typename common_type<T, U>::type, V...>::type type;
};

template<typename T, typename U = std::string>
struct has_to_string : false_type {};

template<typename T>
struct has_to_string<T, decltype(std::declval<T>().to_string())> : true_type {};

struct A {
    std::string to_string();
};

struct B {

};

template<typename F, typename T1, typename T2>
auto flip(F f, T1 &&t1, T2 &&t2) -> decltype(f(std::declval<T1>(), std::declval<T2>()))
{
    return f(std::forward<T2>(t2), std::forward<T1>(t1));
}

int testFunc(int a, int b)
{
    std::cout << a << b << std::endl;
    return a - b;
}

int main()
{
    std::cout << has_to_string<A>::value << std::endl;
    std::cout << has_to_string<B>::value << std::endl;
    std::cout << flip(testFunc, 10, 2) << std::endl;
	return 0;
}