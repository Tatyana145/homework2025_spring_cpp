#include <type_traits>
#include <cstddef>

namespace TypeListDetails {

    template <typename T, typename U>
    constexpr bool is_same_v = std::is_same<T, U>::value;

    template<std::size_t... Ints>
    struct index_sequence {
        using type = index_sequence<Ints...>;
    };

    template<std::size_t N, std::size_t... Ints>
    struct make_index_sequence_impl : make_index_sequence_impl<N - 1, N - 1, Ints...> {};

    template<std::size_t... Ints>
    struct make_index_sequence_impl<0, Ints...> {
        using type = index_sequence<Ints...>;
    };

    template<std::size_t N>
    using make_index_sequence = typename make_index_sequence_impl<N>::type;


    template<typename... Types>
    struct TypeList {};

    template<typename List, std::size_t Index>
    struct TypeAt;

    template<typename... Types>
    constexpr std::size_t Size() {
        return sizeof...(Types);
    }

    template<typename Type, typename... Types>
    constexpr bool Contains() {
        return (is_same_v<Type, Types> || ...);
    }

    template<typename Type, typename... Types, std::size_t... Indices>
    constexpr std::size_t IndexOfHelper(TypeList<Types...>, index_sequence<Indices...>) {
        constexpr std::size_t indices[] = { (is_same_v<Type, Types> ? Indices : (std::size_t)-1)... };

        for (size_t i = 0; i < sizeof...(Types); ++i) {
            if (indices[i] != static_cast<size_t>(-1))
                return indices[i];
        }

        return static_cast<size_t>(-1);
    }

    template<typename Type, typename... Types>
    constexpr std::size_t IndexOf() {
        return IndexOfHelper<Type>(TypeList<Types...>{}, make_index_sequence<sizeof...(Types)>{});
    }

    template<typename List, typename NewType>
    struct Append;

    template<typename... Types, typename NewType>
    struct Append<TypeList<Types...>, NewType> {
        using type = TypeList<Types..., NewType>;
    };

    // Добавление типа в начало списка
    template<typename List, typename NewType>
    struct Prepend;

    template<typename... Types, typename NewType>
    struct Prepend<TypeList<Types...>, NewType> {
        using type = TypeList<NewType, Types...>;
    };

    // Частичная специализация для TypeAt
    template<typename First, typename... Rest, std::size_t Index>
    struct TypeAt<TypeList<First, Rest...>, Index> {
    private:
        // Вспомогательный тип, чтобы компилятор выдавал более понятные ошибки
        template <typename...>
        struct CompileError;

    public:
        using type = typename TypeAt<TypeList<Rest...>, Index - 1>::type;
    };

    template<typename First, typename... Rest>
    struct TypeAt<TypeList<First, Rest...>, 0> {
        using type = First;
    };
} // namespace TypeListDetails

namespace TypeList {
    template<typename... Types>
    using TypeListType = TypeListDetails::TypeList<Types...>;

    // Вспомогательная структура для передачи типов в TypeListDetails::Size
    template <typename List>
    struct SizeHelper;

    template <typename... Types>
    struct SizeHelper<TypeListType<Types...>> {
        static constexpr std::size_t value = TypeListDetails::Size<Types...>();
    };

    template<typename List>
    constexpr std::size_t Size() {
        return SizeHelper<List>::value;
    }

    // Вспомогательная структура для передачи типов в TypeListDetails::Contains
    template <typename Type, typename List>
    struct ContainsHelper;

    template <typename Type, typename... Types>
    struct ContainsHelper<Type, TypeListType<Types...>> {
        static constexpr bool value = TypeListDetails::Contains<Type, Types...>();
    };

    template<typename Type, typename List>
    constexpr bool Contains() {
        return ContainsHelper<Type, List>::value;
    }

    // Вспомогательная структура для передачи типов в TypeListDetails::IndexOf
    template <typename Type, typename List>
    struct IndexOfHelper;

    template <typename Type, typename... Types>
    struct IndexOfHelper<Type, TypeListType<Types...>> {
        static constexpr std::size_t value = TypeListDetails::IndexOf<Type, Types...>();
    };

    template<typename Type, typename List>
    constexpr std::size_t IndexOf() {
        return IndexOfHelper<Type, List>::value;
    }

    template<typename List, typename NewType>
    using Append = typename TypeListDetails::Append<List, NewType>::type;

    template<typename List, typename NewType>
    using Prepend = typename TypeListDetails::Prepend<List, NewType>::type;

    template<typename List, std::size_t Index>
    using TypeAt = typename TypeListDetails::TypeAt<List, Index>::type;
} // namespace TypeList