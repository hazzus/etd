#ifndef ETD_HEAP
#define ETD_HEAP

#include <vector>

namespace etd {

template <typename T, typename Compare = std::less<T>, typename Allocator = std::allocator<T>>
struct heap {
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using const_reference = value_type const&;

    heap() = default;

    heap(heap const& other) : storage(other.storage) {}

    heap(heap&& other) : storage(std::move(other.storage)) {}

    heap(std::vector<T> const& vec) : storage(vec) {
        heapify();
    }

    heap(std::vector<T>&& vec) : storage(std::move(vec)) {
        heapify();
    }

    heap& operator=(heap const& other) {
        if (this != &other) {
            heap(other).swap(*this);
        }
        return *this;
    }

    const_reference top() const noexcept {
        return storage.front();
    } // O(1)

    void pop() {
        std::swap(storage.front(), storage.back());
        storage.pop_back();
        sift_down(0);
    } // O(log)

    void push(const_reference value) {
        storage.push_back(value);
        sift_up(storage.size() - 1);
    } // O(log)

    template <typename... Args>
    void emplace(Args&&... args) {
        storage.emplace_back(std::forward<Args...>(args...));
        sift_up(storage.size() - 1);
    } // O(log)

    void swap(heap& other) noexcept {
        std::swap(this->storage, other.storage);
    }

    bool empty() const noexcept {
        return storage.empty();
    }

    size_type size() const noexcept {
        return storage.size();
    }

    template <typename T1, typename C, typename A>
    friend bool operator==(heap<T1, C, A> const& a, heap<T1, C, A> const& b);

  private:
    void sift_down(size_type index) noexcept {
        while (2 * index + 1 < storage.size()) {
            size_type left = 2 * index + 1, right = 2 * index + 2;
            size_type to_swap = left;
            if (right < storage.size() && cmp(storage[right], storage[left])) {
                to_swap = right;
            }
            if (leq(storage[index], storage[to_swap])) {
                break;
            }
            std::swap(storage[index], storage[to_swap]);
            index = to_swap;
        }
    }

    void sift_up(size_type index) noexcept {
        while (index != 0 && cmp(storage[index], storage[(index - 1) / 2])) {
            std::swap(storage[index], storage[(index - 1) / 2]);
            index = (index - 1) / 2;
        }
    }

    void heapify() {
        for (size_type index = storage.size() / 2; index-- > 0;) {
            sift_down(index);
        }
    }

    std::vector<value_type, Allocator> storage;
    static constexpr Compare cmp = Compare();

    static constexpr bool leq(const_reference a, const_reference b) {
        return cmp(a, b) || !cmp(b, a);
    }
};

template <typename T, typename C, typename A>
bool operator==(heap<T, C, A> const& a, heap<T, C, A> const& b) {
    return a.storage == b.storage;
}

} // namespace etd

#endif
