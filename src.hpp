#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>

struct Permutation {
    int* mapping;
    size_t size;

    // Construct identity permutation of given size
    Permutation(size_t size) : mapping(nullptr), size(size) {
        mapping = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) mapping[i] = static_cast<int>(i);
    }

    // Construct from mapping array
    Permutation(const int* m, size_t size) : mapping(nullptr), size(size) {
        mapping = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) mapping[i] = m[i];
    }

    // Copy constructor
    Permutation(const Permutation& other) : mapping(nullptr), size(other.size) {
        mapping = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) mapping[i] = other.mapping[i];
    }

    // Move constructor
    Permutation(Permutation&& other) noexcept : mapping(other.mapping), size(other.size) {
        other.mapping = nullptr;
        other.size = 0;
    }

    // Copy assignment
    Permutation& operator=(const Permutation& other) {
        if (this == &other) return *this;
        delete[] mapping;
        size = other.size;
        mapping = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) mapping[i] = other.mapping[i];
        return *this;
    }

    // Move assignment
    Permutation& operator=(Permutation&& other) noexcept {
        if (this == &other) return *this;
        delete[] mapping;
        mapping = other.mapping;
        size = other.size;
        other.mapping = nullptr;
        other.size = 0;
        return *this;
    }

    ~Permutation() {
        delete[] mapping;
        mapping = nullptr;
        size = 0;
    }

    void apply(int* permutation) const {
        if (!size) return;
        int* tmp = new int[size];
        for (size_t i = 0; i < size; ++i) tmp[i] = permutation[i];
        for (size_t i = 0; i < size; ++i) permutation[i] = tmp[mapping[i]];
        delete[] tmp;
    }

    // Composition: (this * other)(i) = this(other(i))
    Permutation operator*(const Permutation& other) const {
        Permutation res(size);
        for (size_t i = 0; i < size; ++i) res.mapping[i] = mapping[other.mapping[i]];
        return res;
    }

    Permutation inverse() const {
        Permutation inv(size);
        for (size_t i = 0; i < size; ++i) inv.mapping[mapping[i]] = static_cast<int>(i);
        return inv;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        os << "[";
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) os << " ";
        }
        os << "]";
        return os;
    }
};

struct Transposition {
    int a, b;

    Transposition() : a(0), b(0) {}

    Transposition(int a, int b) : a(a), b(b) {}

    void apply(int* permutation, size_t size) const {
        if (a == b) return;
        if (static_cast<size_t>(a) < size && static_cast<size_t>(b) < size) {
            int t = permutation[a];
            permutation[a] = permutation[b];
            permutation[b] = t;
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        os << "{" << t.a << " " << t.b << "}";
        return os;
    }

    Permutation toPermutation(size_t size) const {
        Permutation p(size);
        if (a != b && static_cast<size_t>(a) < size && static_cast<size_t>(b) < size) {
            p.mapping[a] = b;
            p.mapping[b] = a;
        }
        return p;
    }
};

struct Cycle {
    int* elements;
    size_t size;

    Cycle(const int* elems, size_t size) : elements(nullptr), size(size) {
        elements = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) elements[i] = elems[i];
    }

    // Copy constructor
    Cycle(const Cycle& other) : elements(nullptr), size(other.size) {
        elements = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) elements[i] = other.elements[i];
    }

    // Move constructor
    Cycle(Cycle&& other) noexcept : elements(other.elements), size(other.size) {
        other.elements = nullptr;
        other.size = 0;
    }

    // Copy assignment
    Cycle& operator=(const Cycle& other) {
        if (this == &other) return *this;
        delete[] elements;
        size = other.size;
        elements = size ? new int[size] : nullptr;
        for (size_t i = 0; i < size; ++i) elements[i] = other.elements[i];
        return *this;
    }

    // Move assignment
    Cycle& operator=(Cycle&& other) noexcept {
        if (this == &other) return *this;
        delete[] elements;
        elements = other.elements;
        size = other.size;
        other.elements = nullptr;
        other.size = 0;
        return *this;
    }

    ~Cycle() {
        delete[] elements;
        elements = nullptr;
        size = 0;
    }

    void apply(int* permutation, size_t n) const {
        if (!size) return;
        int* tmp = new int[n];
        for (size_t i = 0; i < n; ++i) tmp[i] = permutation[i];
        for (size_t j = 0; j < size; ++j) {
            int to = elements[j];
            int from = elements[(j + 1) % size];
            if (static_cast<size_t>(to) < n && static_cast<size_t>(from) < n) {
                permutation[to] = tmp[from];
            }
        }
        delete[] tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        os << "{";
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) os << " ";
        }
        os << "}";
        return os;
    }

    Permutation toPermutation(size_t n) const {
        Permutation p(n);
        for (size_t j = 0; j < size; ++j) {
            int to = elements[j];
            int from = elements[(j + 1) % size];
            if (static_cast<size_t>(to) < n && static_cast<size_t>(from) < n) {
                p.mapping[to] = from;
            }
        }
        return p;
    }
};


#endif
