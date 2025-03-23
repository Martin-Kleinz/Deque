#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

namespace sjtu
{

  template <class T>
  class double_list
  {
  public:
    struct node
    {
      T *value;
      node *prev;
      node *next;
      node(const T &val) : prev(nullptr), next(nullptr)
      {
        value = new T(val);
      }
      node() : prev(nullptr), next(nullptr), value(nullptr) {}
      ~node()
      {
        if (value)
          delete value;
      }
    };
    node *head;
    node *tail;
    size_t size;
    double_list()
    {
      head = new node();
      tail = new node();
      head->next = tail;
      tail->prev = head;
      size = 0;
    }
    double_list(const double_list<T> &other)
    {
      head = new node();
      tail = new node();
      head->next = tail;
      tail->prev = head;
      size = 0;
      for (iterator it = other.begin(); it != other.end(); ++it)
        insert_tail(*it);
    }
    ~double_list()
    {
      while (head)
      {
        node *cur = head;
        head = head->next;
        delete cur;
      }
    }

    class iterator
    {
    public:
      node *current;
      double_list<T> *list;
      iterator() : current(nullptr), list(nullptr) {}
      iterator(const iterator &t) : current(t.current), list(t.list) {}
      ~iterator() {}
      iterator operator++(int)
      {
        if (current == list->tail)
        {
          throw sjtu::index_out_of_bound();
        }
        iterator it(*this);
        current = current->next;
        return it;
      }
      iterator &operator++()
      {
        if (current == list->tail)
        {
          throw sjtu::index_out_of_bound();
        }
        current = current->next;
        return *this;
      }
      iterator operator--(int)
      {
        if (current->prev == list->head)
        {
          throw sjtu::index_out_of_bound();
        }
        iterator it(*this);
        current = current->prev;
        return it;
      }
      iterator &operator--()
      {
        if (current->prev == list->head)
        {
          throw sjtu::index_out_of_bound();
        }
        current = current->prev;
        return *this;
      }
      T &operator*() const
      {
        if (!current)
        {
          std::cerr << "Iterator current is null!" << std::endl;
          throw sjtu::index_out_of_bound();
        }
        if (current == list->head || current == list->tail)
        {
          std::cerr << "Iterator points to head or tail!" << std::endl;
          throw sjtu::index_out_of_bound();
        }
        return *(current->value);
      }
      T *operator->() const noexcept
      {
        return current->value;
      }
      bool operator==(const iterator &rhs) const
      {
        return current == rhs.current;
      }
      bool operator!=(const iterator &rhs) const
      {
        return current != rhs.current;
      }
    };
    iterator begin() const
    {
      iterator it;
      it.current = head->next;
      it.list = const_cast<double_list<T> *>(this);
      return it;
    }
    iterator end() const
    {
      iterator it;
      it.current = tail;
      it.list = const_cast<double_list<T> *>(this);
      return it;
    }
    iterator last()
    {
      iterator it;
      it.current = tail->prev;
      it.list = const_cast<double_list<T> *>(this);
      return it;
    }
    iterator erase(iterator pos)
    {
      if (pos == end())
        return pos;
      node *todel = pos.current;
      iterator nextit;
      nextit.current = todel->next;
      nextit.list = pos.list;
      todel->prev->next = todel->next;
      todel->next->prev = todel->prev;
      delete todel;
      size--;
      return nextit;
    }
    void insert_head(const T &val)
    {
      node *newnode = new node(val);
      newnode->next = head->next;
      newnode->prev = head;
      head->next->prev = newnode;
      head->next = newnode;
      size++;
    }
    void insert_tail(const T &val)
    {
      node *newnode = new node(val);
      newnode->prev = tail->prev;
      newnode->next = tail;
      tail->prev->next = newnode;
      tail->prev = newnode;
      size++;
    }
    void delete_head()
    {
      if (size == 0)
        return;
      node *todele = head->next;
      head->next = todele->next;
      todele->next->prev = head;
      delete todele;
      size--;
    }
    void delete_tail()
    {
      if (size == 0)
        return;
      node *todele = tail->prev;
      tail->prev = todele->prev;
      todele->prev->next = tail;
      delete todele;
      size--;
    }
    bool empty() const
    {
      return size == 0;
    }
  };

  template <class T>
  class deque
  {
  public:
    class const_iterator;
    class iterator
    {
    private:
      /**
       * add data members.
       * just add whatever you want.
       */
    public:
      /**
       * return a new iterator which points to the n-next element.
       * if there are not enough elements, the behaviour is undefined.
       * same for operator-.
       */
      iterator operator+(const int &n) const {}
      iterator operator-(const int &n) const {}

      /**
       * return the distance between two iterators.
       * if they point to different vectors, throw
       * invaild_iterator.
       */
      int operator-(const iterator &rhs) const {}
      iterator &operator+=(const int &n) {}
      iterator &operator-=(const int &n) {}

      /**
       * iter++
       */
      iterator operator++(int) {}
      /**
       * ++iter
       */
      iterator &operator++() {}
      /**
       * iter--
       */
      iterator operator--(int) {}
      /**
       * --iter
       */
      iterator &operator--() {}

      /**
       * *it
       */
      T &operator*() const {}
      /**
       * it->field
       */
      T *operator->() const noexcept {}

      /**
       * check whether two iterators are the same (pointing to the same
       * memory).
       */
      bool operator==(const iterator &rhs) const {}
      bool operator==(const const_iterator &rhs) const {}
      /**
       * some other operator for iterators.
       */
      bool operator!=(const iterator &rhs) const {}
      bool operator!=(const const_iterator &rhs) const {}
    };

    class const_iterator
    {
      /**
       * it should has similar member method as iterator.
       * you can copy them, but with care!
       * and it should be able to be constructed from an iterator.
       */
    };

    /**
     * constructors.
     */
    deque() {}
    deque(const deque &other) {}

    /**
     * deconstructor.
     */
    ~deque() {}

    /**
     * assignment operator.
     */
    deque &operator=(const deque &other) {}

    /**
     * access a specified element with bound checking.
     * throw index_out_of_bound if out of bound.
     */
    T &at(const size_t &pos) {}
    const T &at(const size_t &pos) const {}
    T &operator[](const size_t &pos) {}
    const T &operator[](const size_t &pos) const {}

    /**
     * access the first element.
     * throw container_is_empty when the container is empty.
     */
    const T &front() const {}
    /**
     * access the last element.
     * throw container_is_empty when the container is empty.
     */
    const T &back() const {}

    /**
     * return an iterator to the beginning.
     */
    iterator begin() {}
    const_iterator cbegin() const {}

    /**
     * return an iterator to the end.
     */
    iterator end() {}
    const_iterator cend() const {}

    /**
     * check whether the container is empty.
     */
    bool empty() const {}

    /**
     * return the number of elements.
     */
    size_t size() const {}

    /**
     * clear all contents.
     */
    void clear() {}

    /**
     * insert value before pos.
     * return an iterator pointing to the inserted value.
     * throw if the iterator is invalid or it points to a wrong place.
     */
    iterator insert(iterator pos, const T &value) {}

    /**
     * remove the element at pos.
     * return an iterator pointing to the following element. if pos points to
     * the last element, return end(). throw if the container is empty,
     * the iterator is invalid, or it points to a wrong place.
     */
    iterator erase(iterator pos) {}

    /**
     * add an element to the end.
     */
    void push_back(const T &value) {}

    /**
     * remove the last element.
     * throw when the container is empty.
     */
    void pop_back() {}

    /**
     * insert an element to the beginning.
     */
    void push_front(const T &value) {}

    /**
     * remove the first element.
     * throw when the container is empty.
     */
    void pop_front() {}
  };

} // namespace sjtu

#endif
