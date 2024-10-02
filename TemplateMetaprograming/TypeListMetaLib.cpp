// Typeless container, + contains type meta func

#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <tuple>

// Helper template and defines typemember to be equal to T
// So we inherih inside of defining type member each time
template<typename T>
struct has_type{
	using type = T;
};

/// Meta If
template<bool condition, typename THEN, typename ELSE>
struct if_;

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE> : has_type<THEN>{};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE> : has_type<ELSE>{};
////////////////////////////////

template<typename...>
struct type_list{};

/// EMPTY /// 
template<typename LIST>
struct empty : std::false_type{};

template<> //specialisation 
struct empty<type_list<>> : std::true_type{};

template<typename LIST> // Alies
static constexpr bool empty_v = empty<LIST>::value;

static_assert(empty<type_list<>>::value);
static_assert(empty<type_list<float>>::value == false);

/// FRONT /// 
template<typename LIST> 
struct front;

template<typename T0, typename ... T1toN> //List with at least 1 element
struct front<type_list<T0, T1toN...>> : has_type<T0> {};

template<typename LIST> // Alies 
using front_t = typename front<LIST>::type;


static_assert( std::is_same_v<front<type_list<int, bool>>::type , int> );

/// POP FRONT /// 
template<typename LIST>
struct pop_front;

template<typename T0, typename ... T1toN> //List with at least 1 element
struct pop_front<type_list<T0, T1toN...>> : has_type<type_list<T1toN...>>{};
// we can't modify list in complile time setting, so we return new list, by defining type member alies

template<typename LIST> // Alias
using pop_front_t = typename pop_front<LIST>::type;

static_assert( std::is_same_v< pop_front<type_list<int,bool,float>>::type , type_list<bool, float>> );

/// BACK ///
template<typename LIST> 
struct back : has_type<typename back<pop_front_t<LIST>>::type> {};

// recursion
template<typename T0>
struct back<type_list<T0>> : has_type <T0>{};

// DOESN'T work, compiler will match all arguments to T0toN_1, and fail to decude anything for Tn
// template<typename... T0toN_1, typename Tn>
// struct back<type_list<T0toN_1..., Tn>> : has_type<Tn>{]; //gets the last type

template<typename LIST> // Alias
using back_t = typename back<LIST>::type;

static_assert( std::is_same_v<back<type_list<int, bool, float>>::type , float> );
static_assert( std::is_same_v<back_t<type_list<int, bool, float>> , float> );

/// AT ///

template<typename LIST, size_t index>
struct at: has_type<typename at<pop_front_t<LIST>, index-1>::type>{};

//struct at, before has _type
//{   //takes elements 1 by 1 till index = 0
//	using type = typename at<pop_front_t<LIST>,index-1>::type;
//};

template<typename LIST>
struct at<LIST, 0> : has_type<front_t<LIST>>{};

template<typename LIST, size_t index> // Alias
using at_t = typename at<LIST, index>::type;

static_assert(std::is_same_v<at<type_list<int, bool, float>, 1>::type, bool>);
static_assert(std::is_same_v< at_t<type_list<int, bool, float>, 1>, bool>);

/// PUSH BACK ///

template<typename LIST, typename T>
struct push_back;

template<typename ... T0toN, typename T>
struct push_back<type_list<T0toN...>, T> : has_type<type_list<T0toN..., T>>{};

template<typename LIST, typename T> // Alias 
using push_back_t = typename push_back<LIST,T>::type;

static_assert(std::is_same_v<push_back_t<type_list<int, bool>, float>, type_list<int, bool,float>>);
static_assert(std::is_same_v<push_back_t<type_list<>, int>, type_list<int>>);

/// POP BACK ///

template<typename LIST, typename RET_LIST = type_list<>>
struct pop_back;

template<typename T0, typename RET_LIST> 
struct pop_back<type_list<T0>, RET_LIST> : has_type<RET_LIST> {}; // We rebuild the correct list to return

// every time re return we move the type to the front of RET list, recursing till last element
template<typename T0, typename T1, typename ... T2toN, typename RET_LIST>
struct pop_back<type_list<T0,T1, T2toN...>, RET_LIST> : 
	pop_back<type_list<T1, T2toN...>, push_back_t<RET_LIST, T0>>{};

template<typename LIST>
using pop_back_t = typename pop_back<LIST>::type;

static_assert(std::is_same_v<pop_back_t<type_list<int>>, type_list<>>);
static_assert(std::is_same_v<pop_back_t<type_list<int, bool>>, type_list<int>>);


////////////////////////////////

//forward dec
template<typename SEARCH, typename LIST>
struct contains_type;

template<typename SEARCH, typename LIST>
struct non_empty_contains_type: 
	if_ < //IF
	std::is_same_v<SEARCH, front_t<LIST>>,
	//THEN
	std::true_type,
	//Else
	contains_type<SEARCH, pop_front_t<LIST>>
	>::type	
{};


template<typename SEARCH, typename LIST>
struct contains_type :
	if_< // IF
		empty_v<LIST>,
		// THEN
		std::false_type,
		// ELSE
		non_empty_contains_type<SEARCH,LIST>
	>::type
{};




// Runtime version
bool contains( const std::string& search, std::list<std::string> l)
{
	if(l.empty()) 
	{ 
		return false; 
	}
	else
	{
		if( search == l.front() )
		{
			return true;
		}
		else
		{
			l.pop_front();
			return contains( search, l);		
		}
	}
}
////

//int main()
//{
//	// Runtime
//	std::list<std::string> list { "int", "bool", "double" };
//	std::cout << std::boolalpha << contains ("bool", list ) << "\n";
//
//	std::cout << "\n";
//
//	type_list<int, bool, double> types;
//	std::cout << contains_type<bool, decltype(types)>::value << "\n";
//	std::cout << contains_type<float, decltype(types)>::value << "\n";
//
//	return 1;
//}
