
//Get if TUPLE contains a specific type 
#include <iostream>
#include <vector>
#include <string>
#include <tuple>


//runtime ver, as base no std::find or for/while loops transtates bad to templates
//bool contains( const std::string& search, const std::vector<std::string>& v, size_t start_from = 0)
//{
//	//no empty list, assumed handled
//	if(v[start_from] == search)
//	{
//		return true;
//	}
//	else
//	{
//		if(start_from == v.size() - 1)
//		{
//			return false;
//		}
//		else
//		{
//			return contains(search, v, start_from+1);
//		}
//	}
//	return false;
//}

//template ver
// Defining IF statements in template metaprogram, if is reserved keyworkd
template<bool condition, typename THEN, typename ELSE>
struct if_;

template<typename THEN, typename ELSE>
struct if_<true, THEN, ELSE>
{
	using type = THEN;
};

template<typename THEN, typename ELSE>
struct if_<false, THEN, ELSE>
{
	using type = ELSE;
};
//////////////////////


template<typename SEARCH, typename TUPLE, size_t start_from = 0>
struct contains_type :
	if_< // if_ is interited by baseclass so complier know its type
		std::is_same<std::tuple_element_t<start_from, TUPLE>, SEARCH>::value,
		//THEN
		std::true_type, /*we inherit*/
		//ELSE
		typename if_< //IF inner if statement, typename is needed by compilier to know its type, 'dependant name(type)'
			(start_from == std::tuple_size<TUPLE>::value -1 ),
			//THEN
			std::false_type,
			//ELSE
			contains_type<SEARCH, TUPLE, start_from + 1>
		>::type
	>::type
{};

//empty tuple case, we imidietly inherit from false type
template<typename SEARCH>
struct contains_type<SEARCH, std::tuple<>, 0> : std::false_type {};


int main()
{
	//Runtime
	//std::vector<std::string> vec {"int", "float", "bool"};	
	//std::cout << std::boolalpha << contains("bool", vec) << "\n";
	//std::cout << std::boolalpha << contains("double", vec) << "\n";

	//Template version
	std::tuple<int , bool, float> tuple;
	std::cout << std::boolalpha << contains_type<bool, decltype(tuple)>::value << "\n";
	std::cout << std::boolalpha << contains_type<double, decltype(tuple)>::value << "\n";
	std::cout << std::boolalpha << contains_type<float, decltype(tuple)>::value << "\n";
	std::cout << std::boolalpha << contains_type<float, std::tuple<>>::value << "\n";

}

