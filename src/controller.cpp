#include "controller.h"

//! Calculate the next output value based on the
/*!
  * \param errors pointer to current error value
  * \return errors code, 0 for no error
  */
uint8_t Controller::CalcNextValue( int16_t* errors ){
  for( uint8_t o; o<o_channels; o++ ){
    for( uint8_t i; i<i_channels; i++ ){
      new_output[o] = (int16_t)( (((int32_t)transfer_matrix[o][i]) * errors[i]) >> 16);
    }
  }
  return 0;
};

//! Resets all feedback memory
/*!
  * \param val to write to the stored last values
  *
  * Override the last_output value stored and clear history.
  * It is necessary to initialize to a reasonable output
  */
void Controller::Reset( int16_t* val ){
  for(uint8_t i=0; i<i_channels; i++){
    new_output[i] = val[i];
  }
}

void Controller::SetTransferMatrix( int8_t** tm ){
  for(uint8_t i=0; i<i_channels; i++){
    for(uint8_t o=0; o<o_channels; o++){
      transfer_matrix[o][i] = tm[o][i];
    }
  }
}
