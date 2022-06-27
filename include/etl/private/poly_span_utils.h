#pragma once

#include <cstddef>
#include <iterator>
#include <type_traits>

namespace span {
namespace detail {

template <bool Condition, typename Result = void>
using enable_if_t = typename std::enable_if<Condition, Result>::type;

template <typename Type>
using HasSize = std::is_integral<decltype(std::declval<Type&>().size())>;

template <typename Type>
using DataType = decltype(std::declval<Type&>().data());

template <typename Type>
using HasData = std::is_pointer<DataType<Type>>;

template <typename From, typename Type>
using HasConvertibleData = std::is_convertible<DataType<From>, Type*>;

template <typename FromType, typename ToType>
using TransferConst = typename std::conditional<std::is_const<FromType>::value,
                                                typename std::add_const<ToType>::type,
                                                ToType>::type;

template <typename Type>
class poly_span_iterator;

template <typename Iter>
struct IsPolySpanItImpl : std::false_type {};

template <typename Type>
struct IsPolySpanItImpl<poly_span_iterator<Type>> : std::true_type {};

template <typename Iter>
using IsPolySpanIterator = IsPolySpanItImpl<Iter>;

template <typename Type>
class poly_span_iterator : public std::iterator<std::random_access_iterator_tag, Type> {
private:
    template <typename Other>
    using EnableIfConversionToConst = enable_if_t<
        std::is_const<Type>::value && std::is_same<Type, const Other>::value>;

public:
    poly_span_iterator() = default;
    poly_span_iterator(const poly_span_iterator&) = default;
    poly_span_iterator& operator=(const poly_span_iterator&) = default;

    poly_span_iterator(Type* ptr, size_t element_size) noexcept
        : m_ptr(ptr), m_element_size(element_size) {}

    template <typename MutableType,
              typename = EnableIfConversionToConst<MutableType>>
    poly_span_iterator(const poly_span_iterator<MutableType>& other) noexcept
        : m_ptr(other.m_ptr), m_element_size(other.m_element_size) {}

    Type* operator->() const noexcept { return m_ptr; }
    Type& operator*() const noexcept { return *m_ptr; }

    friend bool operator==(poly_span_iterator left, poly_span_iterator right) noexcept { return left.m_ptr == right.m_ptr; }
    friend bool operator!=(poly_span_iterator left, poly_span_iterator right) noexcept { return left.m_ptr != right.m_ptr; }
    friend bool operator >(poly_span_iterator left, poly_span_iterator right) noexcept { return left.m_ptr > right.m_ptr; }
    friend bool operator <(poly_span_iterator left, poly_span_iterator right) noexcept { return left.m_ptr < right.m_ptr; }
    friend bool operator>=(poly_span_iterator left, poly_span_iterator right) noexcept { return left.m_ptr >= right.m_ptr; }
    friend bool operator<=(poly_span_iterator left, poly_span_iterator right) noexcept { return left.m_ptr <= right.m_ptr; }

    poly_span_iterator& operator++() noexcept {
        fromCharPtr(toCharPtr() + m_element_size);
        return *this;
    }

    poly_span_iterator& operator--() noexcept {
        fromCharPtr(toCharPtr() - m_element_size);
        return *this;
    }

    poly_span_iterator& operator+=(ptrdiff_t n) noexcept {
        fromCharPtr(toCharPtr() + n * m_element_size);
        return *this;
    }

    friend ptrdiff_t operator-(poly_span_iterator left, poly_span_iterator right) noexcept {
        return (left.toCharPtr() - right.toCharPtr()) / left.m_element_size;
    }

    poly_span_iterator operator+(ptrdiff_t n) const noexcept {
        poly_span_iterator tmp = *this;
        tmp += n;
        return tmp;
    }

    poly_span_iterator operator++(int) noexcept {
        poly_span_iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    poly_span_iterator operator--(int) noexcept {
        poly_span_iterator tmp = *this;
        --(*this);
        return tmp;
    }

    poly_span_iterator operator-(ptrdiff_t n) const noexcept { return *this + (-n); }

    poly_span_iterator& operator-=(ptrdiff_t n) noexcept { return *this += -n; }

    Type& operator[](ptrdiff_t n) const noexcept { return *(*this + n); }

    friend poly_span_iterator operator+(ptrdiff_t n, poly_span_iterator other) noexcept {
        return other + n;
    }

private:
    template <typename>
    friend class poly_span_iterator;

    using VoidPtr = TransferConst<Type, void>*;
    using CharPtr = TransferConst<Type, char>*;

    CharPtr toCharPtr() const noexcept { return static_cast<CharPtr>(static_cast<VoidPtr>(m_ptr)); }
    void fromCharPtr(CharPtr ptr) noexcept { m_ptr = static_cast<Type*>(static_cast<VoidPtr>(ptr)); }

    Type* m_ptr;
    size_t m_element_size;
};

}
}
