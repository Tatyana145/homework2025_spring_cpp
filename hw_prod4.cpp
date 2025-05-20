#include <iostream>
#include <cassert>

// MixIn для операторов сравнения
template <typename T>
class less_than_comparable {
public:
    bool operator>(T const& other) const {
        return other < static_cast<T const&>(*this);
    }

    bool operator<=(T const& other) const {
        return !(static_cast<T const&>(other) < static_cast<T const&>(*this));
    }

    bool operator>=(T const& other) const {
        return ! (static_cast<T const&>(*this) < other);
    }

    bool operator==(T const& other) const {
        return !(static_cast<T const&>(*this) < other) && !(other < static_cast<T const&>(*this));
    }

    bool operator!=(T const& other) const {
        return (static_cast<T const&>(*this) < other) || (other < static_cast<T const&>(*this));
    }
};

// MixIn для подсчета экземпляров
template <typename T>
class counter {
public:
    counter() {
        ++count_;
    }

    counter(counter const&) {
        ++count_;
    }

    ~counter() {
        --count_;
    }

    static size_t count() {
        return count_;
    }

private:
    static size_t count_;
};

template <typename T>
size_t counter<T>::count_ = 0;

// Пример использования
class Number : public less_than_comparable<Number>, public counter<Number> {
public:
    Number(int value) : m_value{value} {}

    int value() const {
        return m_value;
    }

    bool operator<(Number const& other) const {
        return m_value < other.m_value;
    }

private:
    int m_value;
};

int main() {
    Number one{1};
    Number two{2};
    Number three{3};
    Number four{4};
    assert(one >= one);
    assert(three <= four);
    assert(two == two);
    assert(three > two);
    assert(one < two);
    std::cout << "Count: " << counter<Number>::count() << std::endl;
    return 0;
}