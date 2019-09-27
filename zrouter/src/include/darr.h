/*-------------------------------------------------------------
////	AUTHOR: SANJEEV MAHAJAN 
---------------------------------------------------------------*/
#include "assert.h"
#include <stdlib.h>
#ifndef _DARR_H
#define _DARR_H
template <class T>
class Darr {
  public:
    Darr(int n = 0);
    ~Darr(void);
    void insert(T el);
    void remove(int i);
    T pop();
    T get(int i);
    void set(int i, T el);
    int n();
    void dsort(int cmp(const void *a, const void *b));
    void reset(int n = 0);
  private:
    T *_ar;
    int _n;
    int _tot;
};

template <class T>
Darr<T>::Darr(int n) {
  _n = 0;
  _tot = n;
  if(n == 0) {
    _ar = NULL; 
    return;
  }
  _ar = new T [_tot];
}
template <class T>
void Darr<T>::insert(T el) {
  if(_n<_tot) {
    _ar[_n++] = el;
    return;
  }
  _tot = 2*_tot+1;
  T *ara = new T [_tot];
  int i;
  for(i = 0; i<_n; i++)
    ara[i] = _ar[i];
  ara[_n++] = el;
  if(_ar)
    delete [] _ar;
  _ar = ara;
  return;
}
template <class T>
void Darr<T>::remove(int i) {
  assert(i<_n);
  assert(i>=0);
  int j;
  for(j = i+1; j<_n; j++)
    _ar[j-1] = _ar[j];
  _n--;
}
template <class T>
T Darr<T>::pop() {
  assert(_n>0);
  _n--;
  return _ar[_n];
} 
  
template <class T>
T Darr<T>::get(int i) {
  assert(i<_n);
  assert(i>=0);
  return _ar[i];
}
template <class T>
void Darr<T>::set(int i, T el) {
  assert(i<_n);
  assert(i>=0);
  _ar[i] = el;
}
template <class T>
void Darr<T>::reset(int n) {
  if(_n>n)
    _n = n;
  assert(_n<=_tot);
}
template <class T>
int Darr<T>::n(void) {
  return _n;
}
template <class T>
Darr<T>::~Darr(void) {
  if(_ar)
    delete [] _ar; 
}
template <class T>
void Darr<T>::dsort(int cmp(const void *a, const void *b)) {
  qsort(_ar, _n, sizeof(T), cmp);
} 
#endif
