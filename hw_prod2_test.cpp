#include <iostream>
#include <type_traits>
#include <cassert>
#include "hw_prod2.h"

int main() {
    using List1 = TypeList::TypeListType<int, double, char>;

    static_assert(TypeList::Size<int, double, char>() == 3, "Size test failed");
    static_assert(TypeList::SizeList<List1>() == 3, "Size of TypeList test failed");
    static_assert(TypeList::Size<>() == 0, "Size empty test failed");

    static_assert(TypeList::Contains<int, List1>(), "Contains test failed")
    static_assert(!TypeList::Contains<float, List1>(), "Contains test failed")

    static_assert(TypeList::IndexOf<double, List1>() == 1, "IndexOf test failed")
    static_assert(TypeList::IndexOf<char, List1>() == 2, "IndexOf test failed")
    static_assert(TypeList::IndexOf<float, List1>() == (std::size_t)-1, "IndexOf not found test failed")

    using List2 = TypeList::AppendType<List1, float>;
    static_assert(TypeList::SizeList<List2>() == 4, "Append size test failed");
    static_assert(std::is_same_v<TypeList::TypeAtIndex<List2, 3>, float>, "Append type test failed");

    using List3 = TypeList::PrependType<List1, bool>;
    static_assert(TypeList::SizeList<List3>() == 4, "Prepend size test failed");
    static_assert(std::is_same_v<TypeList::TypeAtIndex<List3, 0>, bool>, "Prepend type test failed");

    static_assert(std::is_same_v<TypeList::TypeAtIndex<List1, 0>, int>, "TypeAt test failed");
    static_assert(std::is_same_v<TypeList::TypeAtIndex<List1, 1>, double>, "TypeAt test failed");
    static_assert(std::is_same_v<TypeList::TypeAtIndex<List1, 2>, char>, "TypeAt test failed");

    std::cout << "All tests passed!" << std::endl;

    return 0;
}