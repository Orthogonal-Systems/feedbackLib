#include "error.h"

Error::Error(){
  //SetReferences( refs );
}

//! change the references for each channel
void Error::SetReferences( int16_t *_references ){
  for(uint8_t i=0; i<chs; i++){
    references[i] = _references[i];
  }
}

//! calculate new error values from inputs
/*!
  * \param i_vals holds input values from each channel
  * \return newest error value
  *
  * calculates errors, means, and rms values
  */
int16_t* Error::CalculateErrors( int16_t *i_vals ){
  for( uint8_t i=0; i<chs; i++ ){
    last_errors[i] = i_vals[i] - references[i];
    error_mean[i] = (int16_t)( ( ((int32_t)last_errors[i])*smoothing_factor[i] ) >> 16);
    error_rms[i]=0; // TODO calculate rms error, needs more storage?
  }
  return last_errors;
};
