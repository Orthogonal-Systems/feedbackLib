#include "error.h"
#include "utility.h"
#include "feedback_conf.h"

Error::Error(){
  //SetReferences( refs );
  for( uint8_t e=0; e<e_chs; e++ ){
    errors.error_I[e] = 0;
    errors.error_P[e] = 0;
    errors.error_D[e] = 0;
    for( uint8_t i=0; i<i_chs; i++ ){
      error_matrix[e][i] = 0;
    }
  }
  for( uint8_t i=0; i<i_chs; i++ ){
    error_in[i] = 0;
    error_in_mean[i] = 0;
    error_in_var[i] = 0;
    smoothing_factor[i] = 0;
  }
}

//! change the references for each channel
void Error::SetReferences( const int16_t *_references ){
  for( uint8_t i=0; i<i_chs; i++){
    references[i] = _references[i];
  }
}

//! calculate new error values from inputs
/*!
  * \param i_vals holds input values from each channel
  * \param deltaT holds time in us since last error reading (for calculating freq)
  * \return newest proportional error values
  *
  * calculates PID errors, means, and variance values
  */
const errors_t & Error::CalculateErrors ( const int16_t *i_vals, const uint16_t deltaT_ms ){
  for( uint8_t i=0; i<i_chs; i++ ){
    error_in[i] = i_vals[i] - references[i];
    int32_t temp = ( ((int32_t)error_in_mean[i])*(1-smoothing_factor[i]) ) \
      + (((int32_t)error_in[i])*smoothing_factor[i] ) ;
    error_in_mean[i] = (int16_t)( temp >> 16);
    error_in_var[i]=0; // TODO calculate rms error, needs more storage?
  }

  int16_t new_errs[e_chs];
  for( uint8_t e=0; e<e_chs; e++ ){
    for( uint8_t i=0; i<i_chs; i++ ){
      new_errs[e] += (int16_t)( ((int32_t)(error_matrix[e][i] * error_in[i])) >> 8 );
    }
    // calculate derivative errors
    int32_t new_err = (((int32_t)(new_errs[e] - errors.error_P[e]))/deltaT_ms);
    errors.error_D[e] = CheckRange( new_err, FEEDBACK_ERROR_BOUND );
    // calculate proportional errors
    errors.error_P[e] = CheckRange( new_errs[e], FEEDBACK_ERROR_BOUND );;
    // calculate integral errors
    new_err = errors.error_I[e] + (errors.error_P[e]*deltaT_ms);
    errors.error_I[e] = CheckRange( new_err, FEEDBACK_ERROR_BOUND );
  }
  return errors;
}

void Error::SetErrorMatrix( const int8_t** em ){
  for(uint8_t i=0; i<i_chs; i++){
    for(uint8_t o=0; o<e_chs; o++){
      error_matrix[o][i] = em[o][i];
    }
  }
}
