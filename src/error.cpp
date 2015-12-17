#include "error.h"

Error::Error( uint8_t _channels, uint16_t *refs ) : channels(_channels){
  references = refs;
}

//! change the references for each channel
void   SetReferences( uint16_t *_references ){
  references = _references;
}

//! calculate new error values from inputs
/*!
  * \param i_vals holds input values from each channel
  * \return newest error value
  *
  * calculates errors, means, and rms values
  */
uint16_t* CalculateErrors( uint16_t *i_vals ){
  for( uint8_t i; i<channels; i++ ){
    last_errors[i] = i_vals[i] - references[i];
    error_mean[i] = (int16_t)( ( ((int32_t)last_errors[i])*smoothing_factor[i] ) >> 16);
    error_rms[i]=0; // TODO calculate rms error, needs more storage?
  }
  return last_errors;
};
