#pragma once

#include "span/detail/poly_span_utils.h"

#include <initializer_list>
#include <stdexcept>

namespace span {

/**
 * C++20 std::span-like view with focus on polymorphic aspect of types.
 * Provides a base class / interface view. Type can be const or non-const.
 * The class behaves like std::span, has O(1) index access and provides RandomAccess iterators.
 *
 * Hints:
 * 1. NEVER assign to elements or apply mutable algorithms to a mutable view to a base class. Object slicing!
 * 2. Pay attention to lifetime of the source buffer. poly_span should mostly be used as an argument of functions.
 * 3. The view has no data() for reason. Pointer to the first byte of the first object is unknown in general.
 */
template <typename Type>
class poly_span {
private:
    template <typename Object>
    using EnableIfConvertibleFromObject = detail::enable_if_t<
        detail::HasSize<Object>::value && detail::HasData<Object>::value &&
        detail::HasConvertibleData<Object, Type>::value>;

    template <typename OtherType>
    using EnableIfConvertibleType = detail::enable_if_t<
        std::is_convertible<OtherType*, Type*>::value>;

    template <typename Iterator>
    using IsContiguousIterator = std::integral_constant<bool,
        std::is_base_of<std::random_access_iterator_tag,
            typename std::iterator_traits<Iterator>::iterator_category>::value &&
        !detail::IsPolySpanIterator<Iterator>::value>;

public:
    using element_type = Type;
    using value_type = typename std::remove_cv<Type>::type;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer = Type*;
    using reference = Type&;
    using const_pointer = const Type*;
    using const_reference = const Type&;
    using iterator = detail::poly_span_iterator<Type>;
    using const_iterator = detail::poly_span_iterator<const Type>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static constexpr size_t npos = ~size_t(0);

    /// Creates empty view.
    poly_span() = default;

    poly_span(const poly_span&) = default;
    poly_span& operator=(const poly_span&) = default;

    /// Construct from a compatible poly_span: convert mutable to const, derived to base type.
    template <typename ConvertibleType>
    poly_span(const poly_span<ConvertibleType>& span) noexcept
        : poly_span(span.m_dataPtr, span.m_size, span.m_element_size) {}


    /// Construct from a range of iterators or pointers. Iterators must Contiguous.
    template <typename ContiguousIterator>
    poly_span(ContiguousIterator begin, ContiguousIterator end) noexcept
        : poly_span(&*begin, end - begin, nullptr) {
        static_assert(IsContiguousIterator<ContiguousIterator>::value, "Wrong iterator");
    }

    /// Construct from a range defined as iterator/pointer to begin and size. Iterators must Contiguous.
    template <typename ContiguousIterator>
    poly_span(ContiguousIterator begin, size_t size) noexcept
        : poly_span(&*begin, size, nullptr) {
        static_assert(IsContiguousIterator<ContiguousIterator>::value, "Wrong iterator");
    }

    /// Construct from a C-style array.
    template <typename ConvertibleType, size_t N>
    poly_span(ConvertibleType(&array)[N]) noexcept
        : poly_span(array, N, nullptr) {}

    /// Construct from initializer_list.
    template <typename ConvertibleType,
              typename = EnableIfConvertibleType<const ConvertibleType>>
    poly_span(std::initializer_list<ConvertibleType> list) noexcept
        : poly_span(list.begin(), list.size(), nullptr) {}

    /// Construct from a buffer objects: std::vector, std::array, std::span or any class with data() and size().
    template <typename BufferLikeObj,
              typename = EnableIfConvertibleFromObject<BufferLikeObj>>
    poly_span(BufferLikeObj& buffer) noexcept
        : poly_span(buffer.data(), buffer.size(), nullptr) {}

    iterator begin() const noexcept { return {m_dataPtr, m_element_size}; }
    iterator end() const noexcept { return {elementAt(m_size), m_element_size}; }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator   cend() const noexcept { return end(); }
    reverse_iterator rbegin() const noexcept { return reverse_iterator{end()}; }
    reverse_iterator   rend() const noexcept { return reverse_iterator{begin()}; }
    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator   crend() const noexcept { return rend(); }

    /// Returns true if the view has no elements.
    bool empty() const noexcept { return m_size == 0; }

    /// Number of elements in the view.
    size_t size() const noexcept { return m_size; }

    /**
     * @brief sizeof(OriginalType) where OriginalType is type of the source buffer.
     * It's not sizeof(Type) for poly_span<Type>!
     */
    size_t element_size() const noexcept { return m_element_size; }

    /**
     * @brief Checks that other poly_span provides view to the same source buffer.
     * It's not like std::span operator==() which checks for elements equality.
     */
    bool equal_view(const poly_span<const Type>& other) const noexcept {
        return (m_dataPtr == other.m_dataPtr) && (m_size == other.m_size)
                && (m_element_size == other.m_element_size);
    }

    /// Reference to the element with given index. No out of bounds check.
    reference operator[](size_t index) const noexcept { return *elementAt(index); }

    /**
     * @brief Index access with out of bounds checks.
     * @param index position of element in the view.
     * @throw std::out_of_range if index >= size().
     * @return reference to the element.
     */
    reference at(size_t index) const {
        if (index >= m_size) throw std::out_of_range("at() ind >= size()");
        return *elementAt(index);
    }

    /// Reference to the first element. The view must be non-empty.
    reference front() const noexcept { return *m_dataPtr; }

    /// Reference to the last element. The view must be non-empty.
    reference back() const noexcept { return *elementAt(m_size - 1); }

    /**
     * @brief Creates a new view as a subrange of this.
     * @param pos Starting offset for the new view.
     * @param count Number of elements to take into the new view. Default npos is to take all until the end.
     * @throw std::out_of_range if pos > size().
     * @return New sub-view. If count > number of elements before the end it's the same as npos, no error.
     */
    poly_span subspan(size_t pos, size_t count = npos) const {
        if (pos > m_size) throw std::out_of_range("subspan() pos > size()");
        return {elementAt(pos), minImpl(m_size - pos, count), m_element_size};
    }

    /**
     * @brief Creates a new view from exactly N first elements.
     * @param count Exact number of elements to take.
     * @throw std::out_of_range if count > size().
     * @return New sub-view.
     */
    poly_span first(size_t count) const {
        if (count > m_size) throw std::out_of_range("first() count > size()");
        return {m_dataPtr, count, m_element_size};
    }

    /**
     * @brief Creates a new view from exactly N last elements.
     * @param count Exact number of elements to take.
     * @throw std::out_of_range if count > size().
     * @return New sub-view.
     */
    poly_span last(size_t count) const {
        if (count > m_size) throw std::out_of_range("last() count > size()");
        return {elementAt(m_size - count), count, m_element_size};
    }

private:
    poly_span(Type* dataPtr, size_t size, size_t element_size) noexcept
        : m_dataPtr(dataPtr), m_size(size), m_element_size(element_size) {}

    template <typename ConvertibleType>
    poly_span(ConvertibleType* dataPtr, size_t size, std::nullptr_t) noexcept
        : m_dataPtr(dataPtr), m_size(size), m_element_size(sizeof(ConvertibleType)) {}

    template <typename>
    friend class poly_span;

    using VoidPtr = detail::TransferConst<Type, void>*;
    using CharPtr = detail::TransferConst<Type, char>*;

    Type* elementAt(size_t ind) const noexcept {
        const auto base = static_cast<CharPtr>(static_cast<VoidPtr>(m_dataPtr));
        return static_cast<Type*>(static_cast<VoidPtr>(base + ind * m_element_size));
    }

    template <typename Value>
    static Value minImpl(Value a, Value b) noexcept { return (a < b) ? a : b; }

    Type* m_dataPtr = nullptr;
    size_t m_size = 0;
    size_t m_element_size = 0;
};

}
