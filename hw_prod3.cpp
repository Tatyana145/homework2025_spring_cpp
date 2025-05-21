#include <iostream>
#include <type_traits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>
#include <array>
#include <typeinfo>
#include <typeindex>
#include "hw_prod2.h"

template <typename... Types>
class TypeMap {
private:
    using KeyList = TypeList::TypeListType<Types...>;
    using ValueList = std::vector<void*>;
    std::array<size_t, sizeof...(Types)> indexMap;
    ValueList values;
    std::vector<std::type_index> keyTypes;


public:
     TypeMap() : keyTypes{std::type_index(typeid(Types))...}, indexMap{} {}

    // Добавление элемента
    template <typename Key>
    void AddValue(const Key& value) {
        if (TypeList::Contains<Key, KeyList>()) {
            std::size_t index = TypeList::IndexOf<Key, KeyList>();
            if (indexMap[index] != 0) {
                delete static_cast<Key*>(values[indexMap[index]-1]);
                values[indexMap[index]-1] = new Key(value);
            } else {
                values.push_back(new Key(value));
                indexMap[index] = values.size();
            }

        } else {
            // Обработка ошибки: тип не входит в TypeList
            throw std::invalid_argument("Type not in TypeList");
        }
    }

    // Получение значения
    template <typename Key>
    Key& GetValue() const {
        if (TypeList::Contains<Key, KeyList>()) {
            std::size_t index = TypeList::IndexOf<Key, KeyList>();
            if (indexMap[index] != 0) {
                 return *static_cast<Key*>(values[indexMap[index]-1]);
            } else {
                 throw std::out_of_range("Value not found for this type");
            }
        } else {
            // Обработка ошибки: тип не входит в TypeList
            throw std::invalid_argument("Type not in TypeList");
        }
    }

    // Проверка наличия элемента
    template <typename Key>
    bool Contains() const {
        if (TypeList::Contains<Key, KeyList>()) {
            std::size_t index = TypeList::IndexOf<Key, KeyList>();
            return indexMap[index] != 0;
        }
        return false;
    }

    // Удаление элемента
    template <typename Key>
    void RemoveValue() {
        if (TypeList::Contains<Key, KeyList>()) {
            std::size_t index = TypeList::IndexOf<Key, KeyList>();
            if (indexMap[index] != 0) {
                 delete static_cast<Key*>(values[indexMap[index]-1]);

                //сдвигаем все элементы за удаленным
                for (size_t i=indexMap[index]-1; i < values.size()-1; ++i) {
                    values[i] = values[i+1];
                }
                values.pop_back();
                indexMap[index] = 0;

                 // Проходим по всем типам и корректируем их значения в indexMap
                 for (size_t i = 0; i < sizeof...(Types); ++i) {
                     if (indexMap[i] > indexMap[index]) {
                        --indexMap[i];
                     }
                }
            }
        }
    }

    // Для тестирования: получение размера values
    size_t getValueSize() const {
        return values.size();
    }

    // Деструктор для очистки памяти
    ~TypeMap() {
        for (void* value : values) {
            if (value != nullptr) {
                delete static_cast<char*>(value);
            }
        }
    }
};

struct DataA {
    std::string value;
};

struct DataB {
    int value;
};

int main() {
    TypeMap<int, DataA, double, DataB> myTypeMap;

    myTypeMap.AddValue<int>(42);
    myTypeMap.AddValue<double>(3.14);
    myTypeMap.AddValue<DataA>({"Hello, TypeMap!"});
    myTypeMap.AddValue<DataB>({10});

    std::cout << "Value for int: " << myTypeMap.GetValue<int>() << std::endl;
    std::cout << "Value for double: " << myTypeMap.GetValue<double>() << std::endl;
    std::cout << "Value for DataA: " << myTypeMap.GetValue<DataA>().value << std::endl;
    std::cout << "Value for DataB: " << myTypeMap.GetValue<DataB>().value << std::endl;

    std::cout << "Contains int? " << (myTypeMap.Contains<int>() ? "Yes" : "No") << std::endl;

    myTypeMap.RemoveValue<double>();

    std::cout << "Size of values after removal: " << myTypeMap.getValueSize() << std::endl;

    try {
        std::cout << "Value for double after removal: " << myTypeMap.GetValue<double>() << std::endl;
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << std::endl;
    }

    return 0;
}