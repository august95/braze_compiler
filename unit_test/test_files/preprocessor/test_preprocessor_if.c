#define ABC 0x1
#define DEF 0x10
#if ABC + 0x11
  #if 0
  #if 1
    #define GHI 0xabcd 
  #endif
  #endif   

  #if DEF + ABC + 0x100
    #define GHI 0xdead
  #endif
  #ifndef DEF
    #define GHI 1234 
  #endif
#endif

int a = GHI;

