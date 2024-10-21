#pragma once

#include <iterator>
#include <cstddef>
#include <stdexcept>

template <typename T>
struct Iterator
{
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;
    using value_type        = T;
    using pointer           = T*;
    using reference         = T&;

    Iterator(pointer ptr) : m_ptr(ptr) {}

    reference operator*() const { return *m_ptr; }
    pointer operator->() { return m_ptr; }

    Iterator& operator++() { ++m_ptr; return *this; }

    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

private:

    pointer m_ptr;
};

template <typename T>
class Vector {
public:
    Vector() : buf(new T[1]), buf_size(1), size(0) { }

    ~Vector() {
        delete[] buf;
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (&other == this) {
            return *this;
        }
        size = other.size;
        buf_size = other.buf_size;
        Realloc(buf_size);
        for (size_t i = 0; i < size; ++i) {
            buf[i] = other[i];
        }
        return *this;
    }

    Vector<T>& operator=(Vector<T>&& other) noexcept {
        if (&other == this) {
            return *this;
        }

        size = other.size;
        buf_size = other.buf_size;
        delete [] buf;
        buf = other.buf;

        other.buf_size = 1;
        other.size = 0;
        other.buf = nullptr;
        other.Realloc(1);

        return *this;
    }

    // Capacity
    [[nodiscard]] bool Empty() const { return size == 0; }

    [[nodiscard]] size_t Capacity() const { return buf_size; }

    [[nodiscard]] size_t Size() const { return size; }

    [[nodiscard]] size_t MaxSize() const { return SIZE_MAX; }

    void Reserve(size_t new_buf_size) {
        if (new_buf_size < size) {
            throw std::runtime_error("New capacity should be more than current size");
        }
        buf_size = new_buf_size;
        Realloc(new_buf_size);
    }

    void ShrinkToFit() {
        buf_size = size;
        Realloc(size);
    }

    // Modifiers
    void Clear() { size = 0; }

    void PushBack(const T& value) {
        if (size == buf_size) {
            buf_size *= 2;
            Realloc(buf_size);
        }
        buf[size++] = value;
    }

    void PushBack(T&& value) {
        if (size == buf_size) {
            buf_size *= 2;
            Realloc(buf_size);
        }
        buf[size] = T();
        buf[size++] = std::move(value);
    }

    template<typename ...Args>
    void EmplaceBack(Args&& ...args) {
        PushBack(T(std::move(args)...));
    }

    void PopBack() {
        if (size == 0) {
            throw std::runtime_error("No elements are left");
        }
        --size;
    }

    void Resize(size_t count) {
        Resize(count, T());
    }

    void Resize(size_t count, const T& value) {
        if (count > buf_size) {
            buf_size = count * 2;
            Realloc(buf_size);
        }
        if (count > size) {
            for (size_t i = size; i < count; ++i) {
                buf[i] = value;
            }
        }
        size = count;
    }

    void Swap(Vector<T>& other) {
        T* tmp_buf = other.buf;
        other.buf = buf;
        buf = tmp_buf;

        size_t tmp_buf_size = other.buf_size;
        other.buf_size = buf_size;
        buf_size = tmp_buf_size;

        size_t tmp_size = other.size;
        other.size = size;
        size = tmp_size;
    }

    // Element access
    T& operator[](size_t pos) {
        return buf[pos];
    }

    const T& operator[](size_t pos) const {
        return buf[pos];
    }

    T& At(size_t pos) {
        if (pos >= size) {
            throw std::runtime_error("Out of bounds");
        }
        return buf[pos];
    }

    const T& At(size_t pos) const {
        if (pos >= size) {
            throw std::runtime_error("Out of bounds");
        }
        return buf[pos];
    }

    T* data() {
        return buf;
    }

    const T* data() const {
        return buf;
    }

    T& front() {
        return At(0);
    }

    const T& front() const {
        return At(0);
    }

    T& back() {
        return At(size - 1);
    }

    const T& back() const {
        return At(size - 1);
    }

    // Iterators
    Iterator<T> begin() { return Iterator(&buf[0]); }
    Iterator<T> end()   { return Iterator(&buf[size]); }

private:
    void Realloc(size_t new_buf_size) {
        auto new_buf = new T[new_buf_size];
        if (new_buf == nullptr) {
            throw std::runtime_error("Could not create buffer for vector");
        }

        for (size_t idx = 0; idx < size; ++idx) {
            new_buf[idx] = buf[idx];
        }
        delete[] buf;
        buf = new_buf;
    }

    T* buf;
    size_t buf_size;
    size_t size;
};
