#ifndef MSORT_H
#define MSORT_H

#include <vector>
#include <cmath>
#include <iostream>

template <class T, class Comparator>
void merge(std::vector<T>& myArray, int start, int middle, int stop, Comparator comp){
    //for debugging
//    std::cout << "merge start: " << start << " stop: " << stop << std::endl;

    std::vector<T> temp;
    int first = start;
    int second = middle;
    while(first < middle and second < stop){
        if(comp(myArray[first], myArray[second])){
            temp.push_back(myArray[first++]);
        }else{
            temp.push_back(myArray[second++]);
        }
    }
    while(first < middle){
        temp.push_back(myArray[first++]);
    }
    while(second < stop){
        temp.push_back(myArray[second++]);
    }

    //debugging
//    for(int i = 0; i < temp.size(); i++){
//        std::cout << temp[i] << " " << i << std::endl;
//    }

    for(int i = start; i < stop; i++){
        myArray[i] = temp[i - start];
    }
}

/**
 *
 * @tparam T
 * @tparam Comparator
 * @param myArray
 * @param start
 * @param stop
 * is the index after the end
 * @param comp
 */
template <class T, class Comparator>
void mergeRecurse(std::vector<T>& myArray, int start, int stop, Comparator comp){
    //for debugging
//    std::cout << "start: " << start << " stop: " << stop << std::endl;
    if(stop - start > 1){
        int middle = floor((start + stop) / 2);
        //not inclusive of middle
        mergeRecurse(myArray, start, middle, comp);
        //not inclusive of stop
        mergeRecurse(myArray, middle, stop, comp);
        //merge from start to stop-1
        merge(myArray, start, middle, stop, comp);
    }
}

template <class T, class Comparator>
void mergeSort (std::vector<T>& myArray, Comparator comp){
    mergeRecurse(myArray, 0, myArray.size(), comp);
}

#endif //MSORT_H
