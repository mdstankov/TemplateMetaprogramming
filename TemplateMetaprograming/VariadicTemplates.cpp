// Containter of types
// no use of For and While loops

#include <iostream>
#include <type_traits>
#include <tuple>



//Print varied number of elements 
void printn( ) // Last element of the pack
{
	std::cout << "\n";
}

template<typename LAST> //reqored before the ... ver 
void printn(LAST &&t ) // Last element of the pack
{
	std::cout << std::forward<LAST>(t) << "\n";
}

template<typename T0, typename ... T1toN> //variable nr of elements
void printn( T0 &&t, T1toN... rest ) // T1toN... parameter pack
{
	std::cout << std::forward<T0>(t) << ", ";
	printn( std::forward<T1toN>(rest)...); // printn( std::forward<T1>(rest1), std::forward<T2>(rest2), ...)
}
/////////////////////////////////////

// Print tuple 
template<typename TUPLE, std::size_t ... indeces>
void print_tuple_impl( TUPLE &&t, std::index_sequence<indeces...>)
{
	print(std::get<indeces>(std::forward<TUPLE>(t))...); //printn( unpack ))
}

template<typename TUPLE>
void print_tupple(TUPLE t)
{
	//Perfect forwarding for perforamcne reasons
	// remove_reference_t, because we can't use tuple_size on reference
	print_tuple_impl( std::forward<TUPLE>(t), std::make_index_sequence<std::tuple_size<std::remove_reference_t<TUPLE>>::value>{}); //0,1,2...
}
//////////////////////////////////////////
//
//int main()
//{
//	printn(9, "hello", 1.5f, true, 1.2);
//
//	auto tuple = std::make_tuple(9, "hello", 1.5f, true, 1.2);
//	//print_tuple(tuple);
//
//	return 1;
//}