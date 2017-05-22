//
// Created by Raaw on 5/17/2017.
//

#ifndef NPCT_UTILS_STATIC_IF_H
#define NPCT_UTILS_STATIC_IF_H
#include <type_traits>

namespace npct::utils {


	//
	// static_if implementation
	// Solution from
	//
	// Does this conditionally compiles?
	// YES!
	template <typename T, typename F>
	auto static_if(std::true_type, T t, F f) { return t; }

	template <typename T, typename F>
	auto static_if(std::false_type, T t, F f) { return f; }

	template <bool B, typename T, typename F>
	auto static_if(T t, F f) { return static_if(std::integral_constant<bool, B>{}, t, f); }

	template <bool B, typename T>
	auto static_if(T t) { return static_if(std::integral_constant<bool, B>{}, t, [](auto&&...){}); }


}


#endif //NPCT_UTILS_STATIC_IF_H
