#ifndef LIST_HPP
#define LIST_HPP
/* List.hpp
 *
 * doubly-linked, double-ended list with Iterator interface
 * EECS 280 List/Editor Project
 */

#include <iostream>
#include <iterator> //std::bidirectional_iterator_tag
#include <cassert>  //assert


template <typename T>
class List {
  //OVERVIEW: a doubly-linked, double-ended list with Iterator interface
public:

//Constructor
List() : first(nullptr), last(nullptr), list_size(0) {}

  //EFFECTS:  returns true if the list is empty
  bool empty() const {
    return first == nullptr;
  }

  //EFFECTS: returns the number of elements in this List
  //HINT:    Traversing a list is really slow. Instead, keep track of the size
  //         with a private member variable. That's how std::list does it.
  int size() const {
    return list_size;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the first element in the list by reference
  T & front() {
    assert(list_size > 0);
    return first->datum;
  }

  //REQUIRES: list is not empty
  //EFFECTS: Returns the last element in the list by reference
  T & back() {
    assert(list_size > 0);
    return last->datum;
  }

  //EFFECTS:  inserts datum into the front of the list
  void push_front(const T &datum) {
    Node* newNode = new Node;
    newNode->datum = datum;
    newNode->prev = nullptr;
    newNode->next = first;

    if (first != nullptr) {
      first->prev = newNode;
    }
    else {
      last = newNode;
    }

    first = newNode;
    list_size++;
  }

  //EFFECTS:  inserts datum into the back of the list
  void push_back(const T &datum) {
    Node* newNode = new Node;
    newNode->datum = datum;
    newNode->prev = last;
    newNode->next = nullptr;

    if (first != nullptr) {
      last->next = newNode;
    }
    else {
      first = newNode;
    }

    last = newNode;
    list_size++;
  }

  //REQUIRES: list is not empty
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS:  removes the item at the front of the list
  void pop_front() {
    assert(list_size > 0);
    Node* temp = first;      // save old first
    first = first->next;     // move first forward

    if (first != nullptr) {
      first->prev = nullptr;
    } else {
      // list is empty
      last = nullptr;
    }

    delete temp;          // delete old node
    list_size--;         //decrement list
  }


  //REQUIRES: list is not empty
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS:  removes the item at the back of the list
  void pop_back() {
    assert(list_size > 0);
    Node* temp = last; 
    last = last->prev; //move last back one

    if (last != nullptr) {
      last->next = nullptr;
    } else {
      // list is empty
      first = nullptr;
    }

    delete temp; //delete node
    list_size--; //decrement size 
  }

  //MODIFIES: invalidates all iterators to the removed elements
  //EFFECTS:  removes all items from the list
  void clear() {
    while (!empty()) {
      pop_front();
    }
  }

  //BIG THREEEE!!!!

  //copy constructor 
  List(const List &other)
  : first(other.first), last(other.last), list_size(other.list_size) { }

  //operator= overload 
  List &operator=(const List &other) {
    first = other.first;
    last = other.last;
    list_size = other.list_size;
    return *this;
  }

  //destructor
  ~List() {
    clear();
  }

  // You should add in a default constructor, destructor, copy constructor,
  // and overloaded assignment operator, if appropriate. If these operations
  // will work correctly without defining these, you should omit them. A user
  // of the class must be able to create, copy, assign, and destroy Lists.

private:
  //a private type
  struct Node {
    Node *next;
    Node *prev;
    T datum;
  };

  //REQUIRES: list is empty
  //EFFECTS:  copies all nodes from other to this
  void copy_all(const List<T> &other) {
    assert(list_size == 0);
    for (Iterator it = other.begin(); it != other.end(); ++it) {
      push_back(*it);
    }
  }

  Node *first;   // points to first Node in list, or nullptr if list is empty
  Node *last;    // points to last Node in list, or nullptr if list is empty

  int list_size; // tracks num of elements

public:
  ////////////////////////////////////////
  class Iterator {
  public:
    //OVERVIEW: Iterator interface to List

    Iterator() : list_ptr(nullptr), node_ptr(nullptr) {}
    // Add a default constructor here. The default constructor must set both
    // pointer members to null pointers.



    // Add custom implementations of the destructor, copy constructor, and
    // overloaded assignment operator, if appropriate. If these operations
    // will work correctly without defining these, you should omit them. A user
    // of the class must be able to copy, assign, and destroy Iterators.



    // Your iterator should implement the following public operators:
    // *, ++ (both prefix and postfix), == and !=.
    // Equality comparisons must satisfy the following rules:
    // - two default-constructed iterators must compare equal
    // - a default-constructed iterator must compare unequal to an
    //   iterator obtained from a list, even if it is the end iterator
    // - two iterators to the same location in the same list must
    //   compare equal
    // - two iterators to different locations in the same list must
    //   compare unequal
    // - comparing iterators obtained from different lists results in
    //   undefined behavior
    //   - Your implementation can handle this however you like, but
    //     don't assume any particular behavior in your test cases --
    //     comparing iterators from different lists essentially
    //     violates the REQURES clause.
    // Note: comparing both the list and node pointers should be
    // sufficient to meet these requirements.



    // Type aliases required to work with STL algorithms. Do not modify these.
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    // This operator will be used to test your code. Do not modify it.
    // REQUIRES: Iterator is decrementable. All iterators associated with a
    //           list are decrementable, including end iterators, except for
    //           begin iterators (those equal to begin() on the list)
    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a reference to this Iterator
    Iterator& operator--() { // prefix -- (e.g. --it)
      assert(list_ptr);
      assert(*this != list_ptr->begin());
      if (node_ptr) {
        node_ptr = node_ptr->prev;
      } else { // decrementing an end Iterator moves it to the last element
        node_ptr = list_ptr->last;
      }
      return *this;
    }

    // This operator will be used to test your code. Do not modify it.
    // REQUIRES: Iterator is decrementable. All iterators associated with a
    //           list are decrementable, including end iterators, except for
    //           begin iterators (those equal to begin() on the list)
    // EFFECTS:  moves this Iterator to point to the previous element
    //           and returns a copy of the original Iterator
    Iterator operator--(int /*dummy*/) { // postfix -- (e.g. it--)
      Iterator copy = *this;
      operator--();
      return copy;
    }

    // REQUIRES: this is a dereferenceable iterator
    // EFFECTS: Increments this iterator to point to the next
    // element and returns the iterator
    Iterator &operator++() {
      assert(node_ptr);
      node_ptr = node_ptr->next;
      return *this;
    }

    // REQUIRES: this is a dereferenceable iterator
    // EFFECTS: Increments this iterator to point to the next
    // element and returns a copy of the original iterator.
    Iterator operator++(int /*postfix*/) {
      Iterator copy = *this; // N.B.: Copy by value
      operator++();
      return copy;
    }

    // REQUIRES: this is a dereferenceable iterator
    // EFFECTS: Returns the element this iterator points to.
    T & operator*() const {
      assert(node_ptr);
      return node_ptr->datum;
    }

    // EFFECTS: Returns whether this and rhs are pointing to
    // the same place.
    // NOTE: The result is only meaningful if both are
    // pointing into the same underlying container.
    bool operator==(Iterator rhs) const {
      return list_ptr == rhs.list_ptr && node_ptr == rhs.node_ptr;
    }

    // EFFECTS: Returns whether this and rhs are not pointing to
    // the same place.
    bool operator!=(Iterator rhs) const {
      return !(*this == rhs);
    }

    // REQUIRES: Iterator is dereferenceable
    // EFFECTS: returns the underlying element by pointer
    // NOTE: T must be a class type to use the syntax it->. If T has a
    //       member variable f, then it->f accesses f on the
    //       underlying T element.
    T* operator->() const {
      return &operator*();
    }

  private:
    const List *list_ptr; //pointer to the List associated with this Iterator
    Node *node_ptr; //current Iterator position is a List node
    // add any additional necessary member variables here


    // add any friend declarations here


    // construct an Iterator at a specific position in the given List
    Iterator(const List *lp, Node *np);

    friend class List;

  };//List::Iterator
  ////////////////////////////////////////

  // return an Iterator pointing to the first element
  Iterator begin() const {
    return Iterator(this, first);
  }

  // return an Iterator pointing to "past the end"
  Iterator end() const {
    return Iterator(this, nullptr);
  }

  //REQUIRES: i is a valid, dereferenceable iterator associated with this list
  //MODIFIES: invalidates all iterators to the removed element
  //EFFECTS: Removes a single element from the list container.
  //         Returns An iterator pointing to the element that followed the
  //         element erased by the function call
  Iterator erase(Iterator i) {
    Node* target = i.node_ptr;
    assert(target != nullptr);

    Node* next_node = target->next;
    
    if (target == first && target == last) {     //if there is only one node
      first = last = nullptr;
    }

    else if (target == first) {      //if removing first node
      first = target->next;
      first->prev = nullptr;
    }
   
    else if (target == last) {        //if removing last node
      last = target->prev;
      last->next = nullptr;
    }
  
    else {     //removing random middle node
      target->prev->next = target->next;
      target->next->prev = target->prev;
    }

    delete target;
    --list_size;

    return Iterator(this, next_node);
  }

  //REQUIRES: i is a valid iterator associated with this list
  //EFFECTS: Inserts datum before the element at the specified position.
  //         Returns an iterator to the the newly inserted element.
  Iterator insert(Iterator i, const T &datum) {
    Node* current = i.node_ptr;

    // allocate new node
    Node* new_node = new Node;
    new_node->datum = datum;

    
    if (current == first) { //inserting at front
      new_node->prev = nullptr;
      new_node->next = first;

      if (first != nullptr) {
        first->prev = new_node;
      }

      first = new_node;

      if (last == nullptr) { // list was empty
        last = new_node;
      }
    }

    if (current == nullptr) { // inserting at end
      push_back(datum);
      return Iterator(this, last);
    }
    
    else { //insert in middle
      Node* prev = current->prev;

      new_node->next = current;
      new_node->prev = prev;

      prev->next = new_node;
      current->prev = new_node;
    }

    ++list_size;
    return Iterator(this, new_node);
  }


};//List


template <typename T>
List<T>::Iterator::Iterator(const List *lp, Node *np)
  : list_ptr(lp), node_ptr(np) {}

////////////////////////////////////////////////////////////////////////////////
// Add your member function implementations below or in the class above
// (your choice). Do not change the public interface of List, although you
// may add the Big Three if needed.  Do add the public member functions for
// Iterator.


#endif // Do not remove this. Write all your code above this line.
