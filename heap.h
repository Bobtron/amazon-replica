#ifndef HEAP_H
#define HEAP_H

#include <vector>
#include <functional>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <unordered_map>
#include <iostream>
template <
         typename T,
         typename TComparator = std::equal_to<T>,
         typename PComparator = std::less<double>,
         typename Hasher = std::hash<T> >
class Heap
{
public:
    /// Constructs an m-ary heap. M should be >= 2
    Heap(int m = 2,
         const PComparator& c = PComparator(),
         const Hasher& hash = Hasher(),
         const TComparator& tcomp = TComparator()  );

    /// Destructor as needed
    ~Heap();

    /// Adds an item with the provided priority
    void push(double pri, const T& item);

    /// returns the element at the top of the heap
    ///  max (if max-heap) or min (if min-heap)
    T const & top() const;

    /// Removes the top element
    void pop();

    /// returns true if the heap is empty
    bool empty() const;

    /// decreaseKey reduces the current priority of
    /// item to newpri, moving it up in the heap
    /// as appropriate.
    void decreaseKey(double newpri, const T& item);

private:
    /// Add whatever helper functions you need below

    void heapify(int loc);
    void trickleUp(int loc);
    void swap(int parent, int loc);

    /// delet this shit later
    void printPriority();
    void printInside();

    // These should be all the data members you need.
    std::vector< std::pair<double, T> > store_;
    int m_;
    PComparator c_;
    std::unordered_map<T, size_t, Hasher, TComparator> keyToLocation_;

};

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::Heap(
    int m,
    const PComparator& c,
    const Hasher& hash,
    const TComparator& tcomp ) :

    store_(),
    m_(m),
    c_(c),
    keyToLocation_(100, hash, tcomp)

{

}

// Complete
template <typename T, typename TComparator, typename PComparator, typename Hasher >
Heap<T,TComparator,PComparator,Hasher>::~Heap()
{

}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::push(double priority, const T& item)
{
    // You complete.
    //pushes on both the store_ which holds the heap implementaitnos
    //and also onto the unordered map
    store_.push_back(std::pair<double, T>(priority, item));
    keyToLocation_.insert(std::pair<T, size_t>(item, store_.size() - 1));
    trickleUp(store_.size() - 1);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::decreaseKey(double priority, const T& item)
{
    // You complete
    if(keyToLocation_.find(item) == keyToLocation_.end()){
        return;
    }
    store_[keyToLocation_[item]].first = priority;
    //either one of the two will go thru and fix shit depending
    //on max or min heap
    trickleUp(keyToLocation_[item]);
    heapify(keyToLocation_[item]);
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
T const & Heap<T,TComparator,PComparator,Hasher>::top() const
{
    // Here we use exceptions to handle the case of trying
    // to access the top element of an empty heap
    if(empty()) {
        throw std::logic_error("can't top an empty heap");
    }

    // You complete
    return store_[0].second;
}

/// Removes the top element
template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::pop()
{
    if(empty()) {
        throw std::logic_error("can't pop an empty heap");
    }
    // You complete
    //according to the slides, stick last element in front
    //and remove last element from back
    //then trickle down
    //also removs from unordered map
    keyToLocation_.erase(store_[0].second);
    store_[0] = store_.back();
    store_.pop_back();
    heapify(0);

}

/// returns true if the heap is empty
template <typename T, typename TComparator, typename PComparator, typename Hasher >
bool Heap<T,TComparator,PComparator,Hasher>::empty() const
{
    return store_.empty();
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::heapify(int i) {
    if((m_ * i) + 1 < (int)store_.size()){
        int smallest = (m_ * i) + 1;
        int current = (m_ * i) + 2;
        //loop thru all the children that exist
        //if one should go ahead of parent,
        //swap em and heapify recurse
        while(current < (int)store_.size() and current <= (m_ * i) + m_){
            if(c_(store_[current].first, store_[smallest].first)){
                smallest = current;
            }
            current++;
        }
        if(c_(store_[smallest].first, store_[i].first)){
            swap(i, smallest);
            heapify(smallest);
        }
    }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::trickleUp(int loc) {
    int parent = (loc - 1) / m_;
    while (parent >= 0 && c_(store_[loc].first, store_[parent].first)) {
        swap(parent, loc);
        loc = parent;
        parent = (loc - 1) / m_;
    }
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::swap(int parent, int loc) {
    //when swaps, also swap the location referenced in the map
    keyToLocation_[store_[parent].second] = loc;
    keyToLocation_[store_[loc].second] = parent;
    std::pair<double, T> temp = store_[parent];
    store_[parent] = store_[loc];
    store_[loc] = temp;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::printPriority() {
    for(size_t i = 0; i < store_.size(); i++){
        std::cout << store_[i].first;
    }
    std::cout << std::endl;
}

template <typename T, typename TComparator, typename PComparator, typename Hasher >
void Heap<T,TComparator,PComparator,Hasher>::printInside() {
    for(size_t i = 0; i < store_.size(); i++){
        std::cout << store_[i].second;
    }
    std::cout << std::endl;
}

#endif

