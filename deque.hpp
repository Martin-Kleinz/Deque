#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <cmath>

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
    iterator insert(iterator pos, const T &val)
    {
      // if (pos.list != this || pos.current == nullptr || pos.current == head || pos.current == tail)
      //   throw sjtu::invalid_iterator();
      node *newnode = new node(val);
      node *oldnode = pos.current;
      newnode->prev = oldnode->prev;
      newnode->next = oldnode;
      oldnode->prev->next = newnode;
      oldnode->prev = newnode;
      ++size;
      iterator it;
      it.current = newnode;
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
  class CircularArray
  {
  public:
    T **arr = nullptr;
    int tail = 0, capa = 100;
    CircularArray()
    {
      arr = new T *[capa];
    }
    CircularArray(size_t c) : capa(c)
    {
      arr = new T *[capa];
    }
    ~CircularArray()
    {
      for (int i = 0; i != tail; i++)
        delete arr[i];
      delete[] arr;
    }
    bool full()
    {
      return tail == capa;
    }
    bool empty()
    {
      return tail == 0;
    }
    T *operator[](size_t n)
    {
      return arr[n];
    }
  };

  template <class T>
  class deque
  {
  private:
    double_list<CircularArray<T> *> *dq;
    int capa = 256, size_ = 0;

  public:
    class const_iterator;
    class iterator
    {
    private:
      /**
       * add data members.
       * just add whatever you want.
       */
      size_t id;
      double_list<CircularArray<T> *> *deq;
      typename double_list<CircularArray<T> *>::iterator it;
      T *p_;
      friend class deque<T>;

    public:
      /**
       * return a new iterator which points to the n-next element.
       * if there are not enough elements, the behaviour is undefined.
       * same for operator-.
       */
      iterator() {}
      iterator(size_t n, double_list<CircularArray<T> *> *d, typename double_list<CircularArray<T> *>::iterator od, T *ptr) : id(n), deq(d), it(od), p_(ptr) {}
      iterator operator+(const int &n) const
      {
        iterator result = *this;
        int rest = n;
        while (rest > 0)
        {
          auto cur = *(result.it);
          if (cur->tail - result.id > rest)
          {
            result.id += rest;
            result.p_ = (*result.it)->arr[result.id];
            return result;
          }
          rest -= cur->tail - result.id;
          result.it++;
          if (result.it == deq->end())
            throw sjtu::index_out_of_bound();
          else
            result.id = 0, result.p_ = (*result.it)->arr[result.id];
        }
        return result;
      }
      iterator operator-(const int &n) const
      {
        iterator result = *this;
        int rest = n;
        while (rest)
        {
          auto cur = *(result.it);
          if (result.id >= rest)
          {
            result.id -= rest;
            result.p_ = (*result.it)->arr[result.id];
            return result;
          }
          rest -= result.id + 1;
          if (result.it == deq->begin())
            throw sjtu::index_out_of_bound();
          result.it--;
          cur = *(result.it);
          result.id = cur->tail - 1;
          result.p_ = (*result.it)->arr[result.id];
        }
        return result;
      }

      /**
       * return the distance between two iterators.
       * if they point to different vectors, throw
       * invaild_iterator.
       */
      int operator-(const iterator &rhs) const
      {
        if (this->deq != rhs.deq)
          throw sjtu::invalid_iterator();
        iterator cur = *this;
        int diff1 = 0;
        while (true)
        {
          auto cirarr = *(this->it);
          diff1 += cirarr->tail - cur.id;
          ++cur.it;
          if (cur.it == deq->end())
            break;
          cur.id = 0;
        }
        cur = rhs;
        int diff2 = 0;
        while (true)
        {
          auto cirarr = *(this->it);
          diff2 += cirarr->tail - cur.id;
          ++cur.it;
          if (cur.it == deq->end())
            break;
          cur.id = 0;
        }
        return diff2 - diff1;
      }
      iterator &operator+=(const int &n)
      {
        *this = *this + n;
        return *this;
      }
      iterator &operator-=(const int &n)
      {
        *this = *this - n;
        return *this;
      }

      /**
       * iter++
       */
      iterator operator++(int)
      {
        iterator result = *this;
        *this = *this + 1;
        return result;
      }
      /**
       * ++iter
       */
      iterator &operator++()
      {
        if (id < (*it)->tail - 1)
        {
          ++id;
          p_ = (*it)->arr[id];
        }
        else
        {
          ++it;
          if (it == deq->end())
          {
            it--;
            id = (*it)->tail;
            p_ = nullptr;
          }
          else
          {
            id = 0;
            p_ = (*it)->arr[id];
          }
        }
        return *this;
      }
      /**
       * iter--
       */
      iterator operator--(int)
      {
        iterator result = *this;
        *this = *this - 1;
        return result;
      }
      /**
       * --iter
       */
      iterator &operator--()
      {
        *this = *this - 1;
        return *this;
      }

      /**
       * *it
       */
      T &operator*() const
      {
        if (!p_)
          throw sjtu::runtime_error();
        return *p_;
      }
      /**
       * it->field
       */
      T *operator->() const noexcept
      {
        return p_;
      }

      /**
       * check whether two iterators are the same (pointing to the same
       * memory).
       */
      bool operator==(const iterator &rhs) const
      {
        return this->it == rhs.it && this->id == rhs.id;
      }
      bool operator==(const const_iterator &rhs) const
      {
        return this->it == rhs.it && this->id == rhs.id;
      }
      /**
       * some other operator for iterators.
       */
      bool operator!=(const iterator &rhs) const
      {
        return this->it != rhs.it || this->id != rhs.id;
      }
      bool operator!=(const const_iterator &rhs) const
      {
        return this->it != rhs.it || this->id != rhs.id;
      }
    };

    class const_iterator
    {
      /**
       * it should has similar member method as iterator.
       * you can copy them, but with care!
       * and it should be able to be constructed from an iterator.
       */
    private:
      /**
       * add data members.
       * just add whatever you want.
       */
      size_t id;
      double_list<CircularArray<T> *> *deq;
      typename double_list<CircularArray<T> *>::iterator it;
      T *p_;
      friend class deque<T>;

    public:
      /**
       * return a new iterator which points to the n-next element.
       * if there are not enough elements, the behaviour is undefined.
       * same for operator-.
       */
      const_iterator() {}
      const_iterator(iterator ite) : id(ite.id), deq(ite.deq), it(ite.it), p_(ite.p_) {}
      const_iterator(int n, double_list<CircularArray<T> *> *d, typename double_list<CircularArray<T> *>::iterator od, T *ptr) : id(n), deq(d), it(od), p_(ptr) {}
      const_iterator operator+(const int &n) const
      {
        const_iterator result = *this;
        int rest = n;
        while (rest)
        {
          auto cur = *(result.it);
          if (cur->tail - result.id > rest)
          {
            result.id += rest;
            result.p_ = (*result.it)->arr[result.id];
            return result;
          }
          rest -= cur->tail - result.id;
          result.it++;
          if (result.it == deq->end())
            throw sjtu::index_out_of_bound();
          else
            result.id = 0, result.p_ = (*result.it)->arr[result.id];
        }
        return result;
      }
      const_iterator operator-(const int &n) const
      {
        const_iterator result = *this;
        int rest = n;
        while (rest)
        {
          auto cur = *(result.it);
          if (result.id >= rest)
          {
            result.id -= rest;
            result.p_ = (*result.it)->arr[result.id];
            return result;
          }
          rest -= result.id + 1;
          if (result.it == deq->begin())
            throw sjtu::index_out_of_bound();
          result.it--;
          cur = *(result.it);
          result.id = cur->tail - 1;
          result.p_ = (*result.it)->arr[result.id];
        }
        return result;
      }

      /**
       * return the distance between two iterators.
       * if they point to different vectors, throw
       * invaild_iterator.
       */
      int operator-(const iterator &rhs) const
      {
        if (this->deq != rhs.deq)
          throw sjtu::invalid_iterator();
        iterator cur = *this;
        int diff1 = 0;
        while (true)
        {
          auto cirarr = *(this->it);
          diff1 += cirarr->tail - cur.id;
          ++cur.it;
          if (cur.it == deq->end())
            break;
          cur.id = 0;
        }
        cur = rhs;
        int diff2 = 0;
        while (true)
        {
          auto cirarr = *(this->it);
          diff2 += cirarr->tail - cur.id;
          ++cur.it;
          if (cur.it == deq->end())
            break;
          cur.id = 0;
        }
        return diff2 - diff1;
      }
      const_iterator &operator+=(const int &n)
      {
        *this = *this + n;
        return *this;
      }
      const_iterator &operator-=(const int &n)
      {
        *this = *this - n;
        return *this;
      }

      /**
       * iter++
       */
      const_iterator operator++(int)
      {
        iterator result = *this;
        *this = *this + 1;
        return result;
      }
      /**
       * ++iter
       */
      const_iterator &operator++()
      {
        if (id < (*it)->tail - 1)
        {
          ++id;
          p_ = (*it)->arr[id];
        }
        else
        {
          ++it;
          if (it == deq->end())
          {
            it--;
            id = (*it)->tail;
            p_ = nullptr;
          }
          else
          {
            id = 0;
            p_ = (*it)->arr[id];
          }
        }
        return *this;
      }
      /**
       * iter--
       */
      const_iterator operator--(int)
      {
        iterator result = *this;
        *this = *this - 1;
        return result;
      }
      /**
       * --iter
       */
      const_iterator &operator--()
      {
        *this = *this - 1;
        return *this;
      }

      /**
       * *it
       */
      T &operator*() const
      {
        if (!p_)
          throw sjtu::runtime_error();
        return *p_;
      }
      /**
       * it->field
       */
      T *operator->() const noexcept
      {
        return p_;
      }

      /**
       * check whether two iterators are the same (pointing to the same
       * memory).
       */
      bool operator==(const iterator &rhs) const
      {
        return this->it == rhs.it && this->id == rhs.id;
      }
      bool operator==(const const_iterator &rhs) const
      {
        return this->it == rhs.it && this->id == rhs.id;
      }
      /**
       * some other operator for iterators.
       */
      bool operator!=(const iterator &rhs) const
      {
        return this->it != rhs.it || this->id != rhs.id;
      }
      bool operator!=(const const_iterator &rhs) const
      {
        return this->it != rhs.it || this->id != rhs.id;
      }
    };

    /**
     * constructors.
     */
    deque()
    {
      dq = new double_list<CircularArray<T> *>;
      CircularArray<T> *first = new CircularArray<T>(capa);
      dq->insert_tail(first);
    }
    deque(const deque &other)
    {
      dq = new double_list<CircularArray<T> *>;
      capa = other.capa;
      for (auto it = other.dq->begin(); it != other.dq->end(); ++it)
      {
        CircularArray<T> *current = *it;
        CircularArray<T> *newArray = new CircularArray<T>(current->capa);
        for (int i = 0; i < current->tail; ++i)
        {
          newArray->arr[newArray->tail++] = new T(*(current->arr[i]));
        }
        dq->insert_tail(newArray);
      }
      size_ = other.size_;
    }

    /**
     * deconstructor.
     */
    ~deque()
    {
      delete dq;
      dq = nullptr;
    }

    /**
     * assignment operator.
     */
    deque &operator=(const deque &other)
    {
      if (this == &other)
        return *this;
      delete dq;
      size_ = other.size_;
      dq = new double_list<CircularArray<T> *>;
      capa = other.capa;
      for (auto it = other.dq->begin(); it != other.dq->end(); ++it)
      {
        CircularArray<T> *current = *it;
        CircularArray<T> *newArray = new CircularArray<T>(current->capa);
        for (int i = 0; i < current->tail; ++i)
        {
          newArray->arr[newArray->tail++] = new T(*(current->arr[i]));
        }
        dq->insert_tail(newArray);
      }
      return *this;
    }

    /**
     * access a specified element with bound checking.
     * throw index_out_of_bound if out of bound.
     */
    T &at(const size_t &pos)
    {
      if (pos >= size_)
        throw sjtu::index_out_of_bound();
      return *(begin() + pos);
    }
    const T &at(const size_t &pos) const
    {
      if (pos >= size_)
        throw sjtu::index_out_of_bound();
      return *(cbegin() + pos);
    }
    T &operator[](const size_t &pos)
    {
      if (pos >= size_)
        throw sjtu::index_out_of_bound();
      return *(begin() + pos);
    }
    const T &operator[](const size_t &pos) const
    {
      if (pos >= size_)
        throw sjtu::index_out_of_bound();
      return *(cbegin() + pos);
    }

    /**
     * access the first element.
     * throw container_is_empty when the container is empty.
     */
    const T &front() const
    {
      if (size_ == 0)
        throw sjtu::container_is_empty();
      return *cbegin();
    }
    /**
     * access the last element.
     * throw container_is_empty when the container is empty.
     */
    const T &back() const
    {
      if (size_ == 0)
        throw sjtu::container_is_empty();
      return *(cend() - 1);
    }

    /**
     * return an iterator to the beginning.
     */
    iterator begin()
    {
      auto first_block = dq->begin();
      return iterator(0, dq, first_block, (*first_block)->arr[0]);
    }
    const_iterator cbegin() const
    {
      auto first_block = dq->begin();
      return const_iterator(0, dq, first_block, (*first_block)->arr[0]);
    }

    /**
     * return an iterator to the end.
     */
    iterator end()
    {
      auto last_block = --dq->end();
      return iterator((*last_block)->tail, dq, last_block, nullptr);
    }
    const_iterator cend() const
    {
      auto last_block = --dq->end();
      return iterator((*last_block)->tail, dq, last_block, nullptr);
    }

    /**
     * check whether the container is empty.
     */
    bool empty() const
    {
      return size_ == 0;
    }

    /**
     * return the number of elements.
     */
    size_t size() const
    {
      return size_;
    }

    /**
     * clear all contents.
     */
    void clear()
    {
      while (!empty())
        pop_back();
    }

    /**
     * insert value before pos.
     * return an iterator pointing to the inserted value.
     * throw if the iterator is invalid or it points to a wrong place.
     */
    iterator insert(iterator pos, const T &value)
    {
      if ((*pos.it)->full())
      {
        int c = std::max((*pos.it)->capa, static_cast<int>(std::sqrt(size_)) + 1);
        iterator iter = pos;
        CircularArray<T> *newarr = new CircularArray<T>(c);
        int i = (*pos.it)->tail / 2;
        iter.id = i;
        iter.p_ = (*iter.it)->arr[iter.id];
        while (i < (*pos.it)->tail - 1)
        {
          newarr->arr[newarr->tail++] = iter.p_;
          i++;
          iter++;
        }
        newarr->arr[newarr->tail++] = iter.p_;
        (*pos.it)->tail = (*pos.it)->tail / 2;
        (*pos.it)->capa = c;
        iter.it++;
        dq->insert(iter.it, newarr);
        if (pos.id >= (*pos.it)->tail)
        {
          pos.id -= (*pos.it)->tail;
          pos.it++;
        }
      }
      if (pos.deq != dq)
        throw sjtu::runtime_error();
      int sum = (*pos.it)->tail - pos.id;
      (*pos.it)->tail++;
      int j = pos.id + sum - 1;
      int k = pos.id;
      for (; j >= k; j--)
        (*pos.it)->arr[j + 1] = (*pos.it)->arr[j];
      (*pos.it)->arr[pos.id] = new T(value);
      size_++;
      pos.p_ = (*pos.it)->arr[pos.id];
      return pos;
    }

    /**
     * remove the element at pos.
     * return an iterator pointing to the following element. if pos points to
     * the last element, return end(). throw if the container is empty,
     * the iterator is invalid, or it points to a wrong place.
     */

    iterator erase(iterator pos)
    {
      int c = std::max(capa, static_cast<int>(std::sqrt(size_)));
      if (pos.it != dq->begin())
      {
        auto prev_it = pos.it;
        --prev_it;
        if ((*pos.it)->tail + (*prev_it)->tail < c)
        {
          CircularArray<T> *newarr = new CircularArray<T>(c);
          int ind = pos.id;
          ind += (*prev_it)->tail;
          for (int i = 0; i < (*prev_it)->tail; ++i)
          {
            newarr->arr[newarr->tail++] = (*prev_it)->arr[i];
          }
          for (int i = 0; i < (*pos.it)->tail; ++i)
          {
            newarr->arr[newarr->tail++] = (*pos.it)->arr[i];
          }
          delete *prev_it;
          dq->erase(prev_it);
          delete *pos.it;
          dq->erase(pos.it);
          pos.it = dq->insert(pos.it, newarr);
          pos.id = ind;
          pos.p_ = (*pos.it)->arr[pos.id];
        }
      }
      if (empty())
        throw sjtu::container_is_empty();
      if (pos.deq != this->dq || pos == end())
        throw sjtu::invalid_iterator();
      int index = pos.id;
      auto cur = (*pos.it);
      delete cur->arr[index];
      for (int i = index + 1; i < cur->tail; i++)
        cur->arr[i] = cur->arr[i + 1];
      cur->tail--;
      size_--;
      if (cur->tail == 0)
      {
        delete *pos.it;
        pos.it = dq->erase(pos.it);
        pos.id = 0;
        if (pos.it != dq->end())
        {
          pos.p_ = (*pos.it)->arr[0];
        }
        else
        {
          pos.p_ = nullptr;
        }
      }
      else
        pos.p_ = cur->arr[index];
      return pos;
    }

    /**
     * add an element to the end.
     */
    void push_back(const T &value)
    {
      insert(end(), value);
    }

    /**
     * remove the last element.
     * throw when the container is empty.
     */
    void pop_back()
    {
      if (empty())
        throw sjtu::container_is_empty();
      erase(end() - 1);
    }

    /**
     * insert an element to the beginning.
     */
    void push_front(const T &value)
    {
      insert(begin(), value);
    }

    /**
     * remove the first element.
     * throw when the container is empty.
     */
    void pop_front()
    {
      if (empty())
        throw sjtu::container_is_empty();
      erase(begin());
    }
  };

} // namespace sjtu

#endif
