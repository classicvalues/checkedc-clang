// RUN: rm -rf %t*
// RUN: 3c -base-dir=%S -alltypes -addcr %s -- | FileCheck -match-full-lines -check-prefixes="CHECK_ALL","CHECK" %s
// RUN: 3c -base-dir=%S -addcr %s -- | FileCheck -match-full-lines -check-prefixes="CHECK_NOALL","CHECK" %s
// RUN: 3c -base-dir=%S -addcr %s -- | %clang -c -fcheckedc-extension -x c -o /dev/null -
// RUN: 3c -base-dir=%S -alltypes -output-dir=%t.checked %s --
// RUN: 3c -base-dir=%t.checked -alltypes %t.checked/b2_calleeunsafe.c -- | diff %t.checked/b2_calleeunsafe.c -
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int *sus(int *x, int *y) {
  //CHECK_NOALL: int *sus(int *x : itype(_Ptr<int>), _Ptr<int> y) : itype(_Ptr<int>) {
  //CHECK_ALL: int *sus(_Array_ptr<int> x, _Ptr<int> y) : itype(_Ptr<int>) {
  int *z = malloc(sizeof(int));
  //CHECK: int *z = malloc<int>(sizeof(int));
  *z = 1;
  x++;
  *x = 2;
  z++;
  return z;
}

int *foo() {
  //CHECK_NOALL: _Ptr<int> foo(void) _Checked {
  //CHECK_ALL: _Ptr<int> foo(void) {
  int sx = 3, sy = 4;
  int *x = &sx;
  //CHECK_NOALL: _Ptr<int> x = &sx;
  //CHECK_ALL: int *x = &sx;
  int *y = &sy;
  //CHECK: _Ptr<int> y = &sy;
  int *z = sus(x, y);
  //CHECK_NOALL: _Ptr<int> z = sus(x, y);
  //CHECK_ALL: _Ptr<int> z = sus(_Assume_bounds_cast<_Array_ptr<int>>(x, byte_count(0)), y);
  *z = *z + 1;
  return z;
}

int *bar() {
  //CHECK_NOALL: _Ptr<int> bar(void) _Checked {
  //CHECK_ALL: _Ptr<int> bar(void) {
  int sx = 3, sy = 4;
  int *x = &sx;
  //CHECK_NOALL: _Ptr<int> x = &sx;
  //CHECK_ALL: int *x = &sx;
  int *y = &sy;
  //CHECK: _Ptr<int> y = &sy;
  int *z = (sus(x, y));
  //CHECK_NOALL: _Ptr<int> z = (sus(x, y));
  //CHECK_ALL: _Ptr<int> z = (sus(_Assume_bounds_cast<_Array_ptr<int>>(x, byte_count(0)), y));
  return z;
}
