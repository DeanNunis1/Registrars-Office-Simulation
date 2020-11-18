#ifndef REGISTAR_REGISTRAR_H
#define REGISTAR_REGISTRAR_H

#include <iostream>

/**
 * List Interface
 * @tparam T
 */
template <typename T>
class ListInterface {
public:
    virtual void push_front(T const &data) = 0;
    virtual void push_back(T const &data) = 0;
    virtual void pop_front() = 0;
    virtual void pop_back() = 0;
    virtual T& front() = 0;
    virtual T& back() = 0;
    virtual T const& front() const = 0;
    virtual T const& back() const = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
    virtual ~ListInterface() {

    }
};


/**
 * Doubly Linked List
 * @tparam T
 */
template <typename T>
class List : ListInterface<T> {
private:

    /**
     * Doubly Linked List Node
     */
    class Node {
    public:
        Node *prev;
        Node *next;
        T data;

        /**
         * Constructor
         * @param data
         */
        Node(T const& data)
            : prev(this), next(this), data(data)
        {}
    };

    /**
     * Inserts node between lhs and lhs->next
     * @param lhs Pointer to Node
     * @param node Pointer to Node
     */
    void insert_after(Node *lhs, Node *node) {
        lhs->next->prev = node;
        node->next = lhs->next;
        lhs->next = node;
        node->prev = lhs;
    }

    /**
     * Unlinks / removes the node from the linked list chain i.e.,
     * removes link between node and node->prev, node and node->next and
     * makes a new link between node->prev and node->next
     * @param node Pointer to Node
     */
    void unlink_node(Node *node) {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

    Node *head;
    size_t length;

public:


    /**
     * Constructor
     */
    List()
        : head(NULL), length(0)
    {}


    /**
     * Copy Constructor.
     * @param rhs
     */
    List(List const& rhs)
        : head(NULL), length(0) {

        // Call copy assignment
        *this = rhs;
    }

    /**
     * Copy Assignment.
     * @param rhs
     * @return Reference to the list.
     */
    List& operator=(List const& rhs) {

        // Self Assignment
        if (this == &rhs)
            return *this;

        // Remove existing elements
        while (length) {
            pop_back();
        }

        // Copy elements from rhs
        Node *curr = rhs.head;
        for (size_t i = 0; i < rhs.length; ++i) {
            push_back(curr->data);
            curr = curr->next;
        }

        return *this;
    }

    /**
     * Destructor
     */
    ~List() {

        while (length) {
            pop_back();
        }
    }

    /**
     * Insert data at the beginning of the list.
     * @param data Data to insert.
     */
    void push_front(T const &data) {
        Node *node = new Node(data);
        if (!length) {
            head = node;
        } else {
            insert_after(head->prev, node);
            head = node;
        }
        ++length;
    }

    /**
     * Insert data at the end of the list.
     * @param data Data to insert.
     */
    void push_back(T const &data) {
        Node *node = new Node(data);
        if (!length) {
            head = node;
        } else {
            insert_after(head->prev, node);
        }
        ++length;
    }

    /**
     * Removes first item from the list.
     */
    void pop_front() {
        Node *new_head = head->next;
        unlink_node(head);
        --length;
        delete head;
        if (length) {
            head = new_head;
        }
        else {
            head = NULL;
        }
    }

    /**
     * Removes the last item from the list.
     */
     void pop_back() {
        /* Previous of head is pointing to last item of the list */
        Node *to_remove = head->prev;
        unlink_node(to_remove);
        --length;
        delete to_remove;
        if (!length) {
            head = NULL;
        }
    }

    /**
     * Return the first element of the list.
     * @return Reference to first the element of the list.
     */
    T& front() {
        return head->data;
    }

    /**
     * Return the last element of the list.
     * @return Reference to the last element of the list.
     */
    T& back() {
        /* Previous of head is pointing to the last element of the list */
        return head->prev->data;
    }

    /**
     * Return the first element of the list.
     * @return Const reference to the first element of the list.
     */
    T const & front() const {
        return head->data;
    }

    /**
    * Return the last element of the list.
    * @return Reference to the last element of the list.
    */
    T const & back() const {
        /* Previous of head is pointing to the last element of the list */
        return head->prev->data;
    }

    /**
     * Return the length of the list
     * @return The size of the list.
     */
    size_t size() const {
        return length;
    }

    /**
     * Check if the list is empty.
     * @return true if the length is 0 otherwise false.
     */
    bool empty() const {
        return size() == 0;
    }

    /**
     * Return the index'th element of the list
     * @param index
     * @return Reference to the element.
     */
    T& operator[](size_t index) {

        Node *curr = head;
        while (index--) {
            curr = curr->next;
        }
        return curr->data;
    }

};


/**
 * Queue Class
 * @tparam T
 */
template <typename T>
class Queue {

private:
    List <T> list;

public:


    /**
     * Adds data to the end of the queue.
     * @param data
     */
    void enqueue(T const& data) {
        list.push_back(data);
    }

    /**
     * Removes an element from the front of the queue.
     */
    void dequeue() {
        list.pop_front();
    }

    /**
     * Returns the first element of the queue
     * @return Reference to the first element of the queue
     */
    T& front() {
        return list.front();
    }

    /**
    * Returns the first element of the queue
    * @return Const reference to the first element of the queue
    */
    T const& front() const {
        return list.front();
    }

    /**
     * Returns the length of the queue.
     * @return Length of the queue.
     */
    size_t size() const {
        return list.size();
    }

    /**
    * Check if the queue is empty.
    * @return true if the length is 0 otherwise false.
    */
    bool empty() const {
        return list.empty();
    }
};


/**
 * RegistrarWindow Class
 * Represents a Registrar office window.
 */
class RegistrarWindow {
public:
    int idle;           // Amount of time this window was idle for
    int occupied_till;  // Time till which window is occupied or -1 if not occupied
    RegistrarWindow()
        : idle(0), occupied_till(-1)
    {};
};


/**
 * Student Class
 * Represents a student.
 */
class Student {
public:
    int arrive;     // Arrival time of the student
    int amount;     // Amount of time this student needs at a window
    Student(int arrive, int amount)
            : arrive(arrive), amount(amount)
    {}
};


/**
 * Registar Class
 */
class Registar {

private:

    int num_windows;
    Queue <Student> queue;

public:

    /**
    * Constructor
    * @param filename Name of the file containing the input data.
    */
    Registar(const char *filename);


    /**
     * Perform the simulation.
     */
    void simulate();

};

#endif //REGISTAR_REGISTRAR_H
