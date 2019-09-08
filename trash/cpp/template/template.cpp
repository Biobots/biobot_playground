#include <future>

//decide return type
template<bool, typename T = void>
struct enable_if {};

template<typename T>
struct enable_if<true, T>
{
	typedef T Type;
};

template<typename T>
struct is_integer
{
	bool val = false;
};

template<>
struct is_integer<unsigned char>
{
	bool val = true;
};

template<>
struct is_integer<char>
{
	bool val = true;
};

template<>
struct is_integer<int>
{
	bool val = true;
};

template<>
struct is_integer<unsigned int>
{
	bool val = true;
};

int main()
{
	return 0;
}