#ifndef NPCT_UTILS_STRING_LITERAL_H
#define NPCT_UTILS_STRING_LITERAL_H

namespace npct::utils
{
	template <char ... C>
	struct string_literal {
		static constexpr const char value[sizeof...(C)] = { C... };
		constexpr operator const char* (void) const {
			return value;
		}
	};
	template <char ... C> constexpr const char string_literal<C...>::value[sizeof...(C)];

	template <char... Cs>
	constexpr string_literal<Cs...> operator ""_create() {
		return {};
	}


}

#endif //NPCT_UTILS_STRING_LITERAL_H