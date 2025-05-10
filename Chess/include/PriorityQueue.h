// Generic comparator structure
template <typename T>
struct DefaultComparator {
    int operator()(const T& a, const T& b) const {
        if (a < b) return -1;
        if (b < a) return 1;
        return 0;
    }
};

// PriorityQueue class template
template <typename T, typename Comparator = DefaultComparator<T>>
class PriorityQueue {
private:
    std::list<T> m_queue;
    size_t m_maxSize;
    Comparator m_comparator;

public:
    // Constructor with optional max size parameter (0 means unlimited)
    PriorityQueue(size_t maxSize = 0, const Comparator& comparator = Comparator())
        : m_maxSize(maxSize), m_comparator(comparator) {
    }

    // Push element to the queue in O(n) complexity
    void push(const T& element);

    // Poll the highest priority element in O(1) complexity
    T poll();

    // Check if queue is empty
    bool isEmpty() const { return m_queue.empty(); }

    // Get current size of queue
    size_t size() const { return m_queue.size(); }

    // Get a const reference to the underlying list (for iteration)
    const std::list<T>& getList() const { return m_queue; }
};


template <typename T, typename Comparator>
void PriorityQueue<T, Comparator>::push(const T& element) {
    // If max size is enforced and already reached
    if (m_maxSize > 0 && m_queue.size() == m_maxSize) {
        // The queue is sorted, so the last element has the lowest priority
        auto lowestIt = std::prev(m_queue.end());

        // If the new element has lower or equal priority than the lowest one, skip insertion
        if (m_comparator(element, *lowestIt) <= 0) {
            return;
        }

        // Else, remove the lowest element to make space
        m_queue.erase(lowestIt);
    }

    // Find correct insertion point to keep queue sorted by priority
    auto it = m_queue.begin();
    while (it != m_queue.end() && m_comparator(*it, element) >= 0) {
        ++it;
    }

    // Insert element in the correct position
    m_queue.insert(it, element);
}


template <typename T, typename Comparator>
T PriorityQueue<T, Comparator>::poll() {
    if (isEmpty()) {
       // throw EmptyQueueException(); TODO
    }

    // The front element is always the highest priority one
    T result = m_queue.front();
    m_queue.pop_front();
    return result;
}