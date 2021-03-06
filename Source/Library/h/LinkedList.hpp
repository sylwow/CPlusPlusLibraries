#pragma once
#include <iostream>
#include <string>

namespace sd
{
    template <class T> class ListNode
    {
      public:
        using ItemType = T;
        using NodePtr = ListNode<T> *;
        using ConstNodePtr = const ListNode<T> *;

      private:
        T _item;
        NodePtr _left = nullptr;
        NodePtr _right = nullptr;

      public:
        ListNode() = delete;

        template <class... Types> ListNode(Types... args) : _item{args...} {}
        ListNode(const T &i) : _item(i) {}
        ListNode(T &&i) : _item(i) {}

        ~ListNode() = default;

        void setNextNode(NodePtr p)
        {
            if (_left)
            {
                _left->setParent(nullptr);
            }
            _left = p;
            if (_left)
            {
                _left->setParent(this);
            }
        }

        NodePtr getNextNode() { return _left; }

        ConstNodePtr getNextNode() const { return _left; }

        void setParent(NodePtr parent) { _right = parent; }

        NodePtr getParentNode() { return _right; }

        ConstNodePtr getParentNode() const { return _right; }

        T &getItem() { return _item; }

        const T &getItem() const { return _item; }
    };

    template <class T, bool R> // R = Reverse
    class ListIterator
    {
      public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;
        using const_pointer = const value_type*;
        using reference = value_type &;
        using NodePtr =
            std::conditional_t<std::is_const<T>::value, const ListNode<std::remove_cv_t<T>> *, ListNode<T> *>;

      protected:
        NodePtr _ptr = nullptr;

      public:
        ListIterator(NodePtr ptr = nullptr) { _ptr = ptr; }
        ListIterator(const ListIterator<T, R> &rawIterator) = default;
        ~ListIterator() = default;

        ListIterator<T, R> &operator=(const ListIterator<T, R> &rawIterator) = default;
        ListIterator<T, R> &operator=(NodePtr ptr)
        {
            _ptr = ptr;
            return (*this);
        }

        operator bool() const { return _ptr; }

        bool operator==(const ListIterator<T, R> &rawIterator) const { return _ptr == rawIterator._ptr; }
        bool operator!=(const ListIterator<T, R> &rawIterator) const { return _ptr != rawIterator._ptr; }

        ListIterator<T, R> &operator++()
        {
            if constexpr (R)
            {
                _ptr = _ptr->getParentNode();
            }
            else
            {
                _ptr = _ptr->getNextNode();
            }
            return (*this);
        }

        ListIterator<T, R> &operator--()
        {
            if constexpr (R)
            {
                _ptr = _ptr->getNextNode();
            }
            else
            {
                _ptr = _ptr->getParentNode();
            }
            return (*this);
        }

        ListIterator<T, R> operator++(int)
        {
            auto temp(*this);
            ++*this;
            return temp;
        }

        ListIterator<T, R> operator--(int)
        {
            auto temp(*this);
            --*this;
            return temp;
        }

        T &operator*() { return _ptr->getItem(); }
        const T &operator*() const { return _ptr->getItem(); }

        T *operator->() { return &_ptr->getItem(); }
        const T *operator->() const { return &_ptr->getItem(); }
    };

    template <class T> class List
    {
      private:
        using NodePtr = ListNode<T> *;
        using ConstNodePtr = const ListNode<T> *;

        NodePtr _head = nullptr;
        NodePtr _tail = nullptr;
        size_t _size = 0;

      public:
        using Iterator = ListIterator<T, false>;
        using ConstIterator = ListIterator<const T, false>;

        using ReverseIterator = ListIterator<T, true>;
        using ConstReverseIterator = ListIterator<const T, true>;

        // Constructors
        List() = default;

        List(size_t count, const T &value = T())
        {
            for (int i = 0; i < count; ++i)
            {
                pushBack(value);
            }
        }

        template <class InputIt> List(InputIt first, InputIt last)
        {
            for (InputIt it = first; it != last; ++it)
            {
                pushBack((*it));
            }
        }

        List(const List<T> &other)
        {
            auto end = other.end();
            for (auto it = other.begin(); it != end; ++it)
            {
                pushBack(*it);
            }
        }

        List(List<T> &&other)
        {
            _head = other._head;
            _tail = other._tail;
            _size = other._size;
            other._head = nullptr;
            other._tail = nullptr;
            other._size = 0;
        }

        List(std::initializer_list<T> init)
        {
            auto end = init.end();
            for (auto it = init.begin(); it != end; ++it)
            {
                pushBack(*it);
            }
        }

        ~List() { clear(); }

        // Assign
        List<T> &operator=(const List<T> &other)
        {
            clear();
            auto end = other.end();
            for (auto it = other.begin(); it != end; ++it)
            {
                pushBack(*it);
            }
            return *this;
        }

        List<T> &operator=(List<T> &&other)
        {
            clear();
            _head = other._head;
            _tail = other._tail;
            _size = other._size;
            other._head = nullptr;
            other._tail = nullptr;
            other._size = 0;
            return *this;
        }

        List<T> &operator=(std::initializer_list<T> ilist)
        {
            clear();
            auto end = ilist.end();
            for (auto it = ilist.begin(); it != end; ++it)
            {
                pushBack(*it);
            }
            return *this;
        }

        // Element access
        T &at(size_t index)
        {
            auto ptr = getNode(index);
            return ptr->getItem();
        }

        const T &at(size_t index) const
        {
            auto ptr = getConstNode(index);
            return ptr->getItem();
        }

        T &operator[](size_t index) { return at(index); }

        const T &operator[](size_t index) const { return at(index); }

        T &front()
        {
            assertEmpty();
            return _head->getItem();
        }

        const T &front() const
        {
            assertEmpty();
            return _head->getItem();
        }

        T &back()
        {
            assertEmpty();
            return _tail->getItem();
        }

        const T &back() const
        {
            assertEmpty();
            return _tail->getItem();
        }

        // Modifiers
        void pushBack(const T &item) { insertNode(size(), makeNode(item)); }

        void pushBack(T &&item) { insertNode(size(), makeNode(std::move(item))); }

        void pushFront(const T &item) { insertNode(0, makeNode(item)); }

        void pushFront(T &&item) { insertNode(0, makeNode(std::move(item))); }

        template <class... Types> void emplaceBack(Types... args) { insertNode(size(), makeNodeWithItem(args...)); }

        template <class... Types> void emplaceFront(Types... args) { insertNode(0, makeNodeWithItem(args...)); }

        template <class... Types> void emplace(size_t index, Types... args)
        {
            insertNode(index, makeNodeWithItem(args...));
        }

        void insert(size_t index, const T &item) { insertNode(index, makeNode(item)); }

        void insert(size_t index, T &&item) { insertNode(index, makeNode(std::move(item))); }

        void remove(size_t index) { removeNode(index); }

        void popFront()
        {
            assertEmpty();
            removeNode(0);
        }

        void popBack()
        {
            assertEmpty();
            removeNode(size() - 1);
        }

        void swap(List<T> &other)
        {
            auto tmp{std::move(*this)};
            *this = std::move(other);
            other = std::move(tmp);
        }

        void clear() { removeAllNodes(); }

        // Capacity
        size_t size() const { return _size; }

        bool empty() const { return size() == 0; }

        // Iterators
        Iterator begin() { return Iterator{_head}; }
        Iterator end() { return Iterator{}; }

        ConstIterator begin() const { return ConstIterator{const_cast<NodePtr>(_head)}; }
        ConstIterator end() const { return ConstIterator{}; }

        ConstIterator cBegin() const { return ConstIterator{const_cast<NodePtr>(_head)}; }
        ConstIterator cEnd() const { return ConstIterator{}; }

        ReverseIterator rBegin() { return ReverseIterator{_tail}; }
        ReverseIterator rEnd() { return ReverseIterator{}; }

        ConstReverseIterator rBegin() const { return ConstReverseIterator{const_cast<NodePtr>(_tail)}; }
        ConstReverseIterator rEnd() const { return ConstReverseIterator{}; }

        ConstReverseIterator crBegin() const { return ConstReverseIterator{const_cast<NodePtr>(_tail)}; }
        ConstReverseIterator crEnd() const { return ConstReverseIterator{}; }

      private:
        NodePtr getNode(size_t index) { return const_cast<NodePtr>(getConstNode(index)); }

        ConstNodePtr getConstNode(size_t index) const
        {
            assertIndex(index);
            NodePtr ptr = nullptr;
            if (index <= size() / 2)
            {
                ptr = _head;
                while (index--)
                {
                    ptr = ptr->getNextNode();
                    assertPointner(ptr);
                }
            }
            else
            {
                ptr = _tail;
                index = size() - index - 1;
                while (index--)
                {
                    ptr = ptr->getParentNode();
                    assertPointner(ptr);
                }
            }
            return ptr;
        }

        void insertNode(size_t index, NodePtr node)
        {
            if (index + 1 > size())
            {
                index = size(); // push back
            }
            if (index == 0) // push front
            {
                auto tmp = _head;
                setHead(node);
                _head->setNextNode(tmp);
                if (size() == 0)
                {
                    setTail(_head);
                }
            }
            else if (index == size()) // push back
            {
                _tail->setNextNode(node);
                setTail(node);
            }
            else
            {
                auto toMove = getNode(index);
                auto previous = toMove->getParentNode();
                previous->setNextNode(node);
                node->setNextNode(toMove);
            }
            ++_size;
        }

        void removeNode(size_t index)
        {
            assertIndex(index);
            if (index == 0) // pop front
            {
                auto tmp = _head->getNextNode();
                deleteNode(_head);
                setHead(tmp);
                if (size() == 1)
                {
                    setTail(tmp);
                }
            }
            else if (index == size() - 1) // pop back
            {
                auto tmp = _tail->getParentNode();
                deleteNode(_tail);
                setTail(tmp);
            }
            else
            {
                auto next = getNode(index + 1);
                auto toRemove = next->getParentNode();
                auto previous = toRemove->getParentNode();
                previous->setNextNode(next);
                deleteNode(toRemove);
            }
            --_size;
        }

        void removeAllNodes()
        {
            auto index = size();
            auto ptr = _head;
            while (index--)
            {
                assertPointner(ptr);
                auto tmp = ptr->getNextNode();
                deleteNode(ptr);
                ptr = tmp;
            }
            _head = nullptr;
            _tail = nullptr;
            _size = 0;
        }

        void assertIndex(size_t index) const
        {
            if (index + 1 > size())
            {
                throw std::out_of_range(
                    std::string("Index: ") + std::to_string(index) +
                    " exceeded allowed boundaries, current list size is: " + std::to_string(size()));
            }
        }

        void assertEmpty() const
        {
            if (empty())
            {
                throw std::runtime_error("List is empty");
            }
        }

        void assertPointner(NodePtr ptr) const
        {
            if (!ptr)
            {
                throw std::runtime_error("Empty pointner");
            }
        }

        void setHead(NodePtr ptr)
        {
            _head = ptr;
            if (_head)
            {
                _head->setParent(nullptr);
            }
        }

        void setTail(NodePtr ptr)
        {
            _tail = ptr;
            if (_tail)
            {
                _tail->setNextNode(nullptr);
            }
        }

        NodePtr makeNode(const T &item) { return new ListNode<T>(item); }

        NodePtr makeNode(T &&item) { return new ListNode<T>(std::move(item)); }

        template <class... Types> NodePtr makeNodeWithItem(Types... args) { return new ListNode<T>(args...); }

        void deleteNode(NodePtr ptr) { delete ptr; }
    };

    template <class T> bool operator==(const List<T> &lhs, const List<T> &rhs)
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T> bool operator!=(const List<T> &lhs, const List<T> &rhs) { return !(lhs == rhs); }

    template <class T> bool operator<(const List<T> &lhs, const List<T> &rhs)
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template <class T> bool operator<=(const List<T> &lhs, const List<T> &rhs) { return lhs < rhs || lhs == rhs; }

    template <class T> bool operator>(const List<T> &lhs, const List<T> &rhs)
    {
        return std::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
    }

    template <class T> bool operator>=(const List<T> &lhs, const List<T> &rhs) { return lhs > rhs || lhs == rhs; }

    void linkedMain();

} // namespace sd