
#define EXISTS
#define DONT_EXISTS
#ifdef EXISTS
#undef DONT_EXISTS
#endif
#ifndef DONT_EXISTS
#define ABC 0xDEAD
#endif

int a = ABC;

