#pragma once
//Note(by Alan Duan): Define some micro to generate set/get function code.
//                    Only can be used in pxr_base cpp file.

#define PXR_DEF_SET_AND_GET(C, N, T, V)\
  T C::Get##N##_() const { return V;}\
  void C::Set##N##_(T val) {V = val;}

#define PXR_DEF_SET_AND_GET_REF(C, N, T, V)\
  T C::Get##N##_() const { return V;}\
  void C::Set##N##_(const T& val) {V = val;}
#define PXR_DEF_RESET(C) void C::Reset() {*this = C();}

#define PXR_DEF_MEM_EQUAL(N) N = other.N;