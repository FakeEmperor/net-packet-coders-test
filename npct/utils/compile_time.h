#ifndef NPCT_UTILS_COMPILETIME_H
#define NPCT_UTILS_COMPILETIME_H

namespace npct::utils::compile_time {

    namespace detail {
        template<class type_, class...Args>
        class constructible_from
        {
            template<class C>
            static C arg();

            template <typename U>
            static std::true_type constructible_test(U *, decltype(U(arg<Args>()...)) * = 0);
            static std::false_type constructible_test(...);

        public:

            typedef decltype(constructible_test(static_cast<type_*>(nullptr))) type;

        };
    }   // namespace detail

    template<class type, class...Args>
    struct constructible
    {
        struct from :
            detail::constructible_from<type, Args...>::type {};
    };

    /** 
     *  \brief Generates has_method_##methodname compile-time check class to test, whether specified class has 
     *         method named with
     *  \param C Class to check. You can add const qualifier to check if it has const method
     *  \param Ret(Args...) Method signature
     *  \see https://stackoverflow.com/questions/87372/check-if-a-class-has-a-member-function-of-a-given-signature
     *  
     */
#define GENERATE_COMPILE_TIME_METHOD_CHECK(methodname, enable_inheritance)                              \
                                                                                                        \
    template<typename, typename T>                                                                      \
    struct has_method_##methodname{                                                                     \
        static_assert(                                                                                  \
            std::integral_constant<T, false>::value,                                                    \
            "Second template parameter needs to be of function type.");                                 \
    };                                                                                                  \
    template<typename C, typename Ret, typename... Args>                                                \
    struct has_method_##methodname<C, Ret(Args...)> {                                                   \
    private:                                                                                            \
        template<typename T>                                                                            \
        using RetnType = decltype(std::declval<T>().methodname(std::declval<Args>()...));               \
        template<typename T>                                                                            \
        static constexpr auto check(T*) -> typename                                                     \
            std::enable_if<                                                                             \
                    ( std::is_base_of<RetnType<T>, Ret>::value and enable_inheritance)                  \
                    or                                                                                  \
                    std::is_same<RetnType<T>, Ret>::value, Ret                                          \
                        >::type;                                                                        \
                                                                                                        \
        template<typename>                                                                              \
        static constexpr std::false_type check(...);                                                    \
                                                                                                        \
        typedef decltype(check<C>(0)) type;                                                             \
    public:                                                                                             \
        static constexpr bool value = type::value;                                                      \
    };                                                                                                  \



    template <typename... Args> struct ambiguate : public Args... {};
    template<typename A, typename = void>
    struct got_type : std::false_type {};

    template<typename A>
    struct got_type<A> : std::true_type {
        typedef A type;
    };

    template<typename T, T>
    struct sig_check : std::true_type {};

    template<typename Alias, typename AmbiguitySeed>
    struct has_member {
        template<typename C> static char ((&f(decltype(&C::value))))[1];
        template<typename C> static char ((&f(...)))[2];

        //Make sure the member name is consistently spelled the same.
        static_assert(
                (sizeof(f<AmbiguitySeed>(0)) == 1)
                , "Member name specified in AmbiguitySeed is different from member name specified in Alias, or wrong Alias/AmbiguitySeed has been specified."
        );

        static bool const value = sizeof(f<Alias>(0)) == 2;
    };

    //Check for member function with given name AND signature.
#define GENERATE_COMPILE_TIME_FUNC_SIG_CHECK(func_name, func_sig)          \
    template<typename T, typename = std::true_type>                             \
    struct has_member_func_##func_name : std::false_type {};                \
    template<typename T>                                                        \
    struct has_member_func_##func_name<                                     \
        T, std::integral_constant<                                              \
            bool                                                                \
            , npct::utils::compile_time::sig_check<func_sig, &T::func_name>::value  \
        >                                                                       \
    > : std::true_type {};                                                       \

};


#endif //NPCT_UTILS_COMPILETIME_H