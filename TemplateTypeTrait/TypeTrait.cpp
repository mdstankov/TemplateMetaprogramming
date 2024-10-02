// Templates & Specilisations
// Type traits to queryproperties
// Metafunctions to manipulate types

#include <iostream>
#include <type_traits>

// Type traits to queryproperties
template<typename T>
struct is_pointer
{
	static constexpr bool value = false;
};

template<typename T>
struct is_pointer<T*>
{
	static constexpr bool value = true;
};
//////////////////////////////////

// Metafunctions to manipulate types
template<typename T>
struct strip_pointer
{
	using type = T;
};

template<typename T>
struct strip_pointer<T*>
{
	//returns the type without the pointer
	using type = T;
};
/////////////////////////////////////

template<typename T>
void print1(T t)
{
	// Templates & Specilisations
	using T_without_pointer = strip_pointer<T>::type;

	//Type trait std::is_pointer<T>::value when youse from <type_traits> no need for custom def
	if constexpr(is_pointer<T>::value && std::is_floating_point<T_without_pointer>::value)
	{
		std::cout << *t;
	}
	else if constexpr(is_pointer<T>::value)
	{
		std::cout << *t;
	}
	else
	{
		std::cout << t;
	}

	//Type trait std::is_pointer<T>::value when youse from <type_traits> no need for custom def
	//if constexpr(is_pointer<T>::value)
	//{
	//	std::cout << *t;
	//}
	//else
	//{
	//	std::cout << t;
	//}
}

template<typename A, typename B, typename C>
void print(A a, B b, C c)
{
	print1(a);
	std::cout << " ";
	print1(b);
	std::cout << " ";
	print1(c);
	std::cout << "\n";
}


//int main()
//{
//	print(1,2,3);
//	std::string hello = "hello";
//	print(hello, 2, hello);
//	print(&hello, 2, hello);
//
//	float a = 10;
//	double b = 2;
//	print(&hello, &a, &b);
//
//	return 1;
//}
