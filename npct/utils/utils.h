#pragma once
#ifndef NPCT_UTILS_UTILS_H
#define NPCT_UTILS_UTILS_H

#include <string>
#include <vector>
#include <memory>

#ifdef _WIN32
#define NEWLINE "\r\n"
#elif defined macintosh // OS 9
#define NEWLINE "\r"
#else
#define NEWLINE "\n" // Mac OS X uses \n
#endif


#define _STR(x) #x
#define STR(x) _STR(x)



namespace npct::utils {
    std::vector<std::string> split(const std::string& s, const std::string &delimeter);


    template<typename I>
    constexpr uint8_t msb_bit_order(I i, uint8_t order = 0) noexcept {
        static_assert(std::is_integral<I>::value, "typename 'I' must be integral");
        return (uint8_t) (i == 0 ? order - 1 : msb_bit_order(i >> 1, (uint8_t) (order + 1)));
    }

    template <typename I>
    constexpr I cpow(I i, uint8_t power, I r = 1) noexcept {
        static_assert(std::is_integral<I>::value, "typename 'I' must be integral");
        return power == 0 ? r : (power % 2 == 0 ? cpow(i*i, (uint8_t) (power / 2), r) : cpow(i, --power, r*i) );
    }

    template <typename I>
    bool is_pow2 (I i)
    {
        static_assert(std::is_integral<I>::value, "typename 'I' must be integral");
        return ((i != 0) && !(i & (i - 1)));
    }

    /**
     * \see https://stackoverflow.com/questions/18939882/raw-pointer-lookup-for-sets-of-unique-ptrs
     */
    

    template<class T>
    struct pointer_cmp {
        struct helper_t {
            T* ptr;
            helper_t() :ptr(nullptr) {}
            helper_t(helper_t const&) = default;
            helper_t(T* p) :ptr(p) {}
            helper_t(const T* p) :ptr(p) {}
            template<class...Ts>
            helper_t(std::shared_ptr<Ts...> const& sp) : ptr(sp.get()) {}
            template<class U, class...Ts>
            helper_t(std::unique_ptr<U, Ts...> const& up) : ptr(up.get()) {}
            // && optional: enforces rvalue use only

            bool operator < (helper_t o) const { return ptr < o.ptr; }
        };
        typedef std::true_type is_transparent;
        // helper does some magic in order to reduce the number of
        // pairs of types we need to know how to compare: it turns
        // everything into a pointer, and then uses `std::less<T*>`
        // to do the comparison:
        
        // without helper, we'd need 2^n different overloads, where
        // n is the number of types we want to support (so, 8 with
        // raw pointers, unique pointers, and shared pointers).  That
        // seems silly:
        // && helps enforce rvalue use only
        bool operator()(const helper_t && lhs, const helper_t && rhs) const {
            return lhs < rhs;
        }
    };

    std::string generate_alnum_str(size_t len);


    // @see https://stackoverflow.com/questions/36120424/alternatives-of-static-pointer-cast-for-unique-ptr
    template<typename TO, typename FROM>
    std::unique_ptr<TO> static_unique_pointer_cast(std::unique_ptr<FROM>&& old) {
        return std::unique_ptr<TO>{static_cast<TO*>(old.release())};
        //conversion: unique_ptr<FROM>->FROM*->TO*->unique_ptr<TO>
    }

}

#endif //NPCT_UTILS_UTILS_H