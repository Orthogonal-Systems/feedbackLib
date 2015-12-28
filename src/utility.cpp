#include "utility.h"
#include <stdint.h>

int16_t CheckRange( int32_t in, int16_t u_bound, int16_t l_bound ){
  if( in > u_bound ){
    return u_bound;
  }
  if( in < l_bound ){
    return l_bound;
  }
  return (int16_t)in;
}

int16_t CheckRange( int32_t in, int16_t bound ){
  return CheckRange( in, bound, -bound );
}
