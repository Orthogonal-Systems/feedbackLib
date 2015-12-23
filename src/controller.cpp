#include "controller.h"

Controller::Controller(){
  for(uint8_t i=0; i<o_channels; i++){
    output[i] = 0;
  }
  for(uint8_t i=0; i<i_channels; i++){
    for(uint8_t o=0; o<o_channels; o++){
      transfer_matrix[o][i] = 0;
    }
  }
}

Controller::Controller( int16_t* init_output ){
  Reset(init_output);
  for(uint8_t i=0; i<i_channels; i++){
    for(uint8_t o=0; o<o_channels; o++){
      transfer_matrix[o][i] = 0;
    }
  }
}

//! Calculate the next output value based on the
/*!
  * \param errors pointer to current error value
  * \return errors code, 0 for no error
  */
uint8_t Controller::CalcNextValue( int16_t* errors ){
  
  for( uint8_t o=0; o<o_channels; o++ ){
    for( uint8_t i=0; i<i_channels; i++ ){
      output[o] += (int16_t)( ((int32_t)(transfer_matrix[o][i] * errors[i])) >> 8);
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
  for(uint8_t i=0; i<o_channels; i++){
    output[i] = val[i];
  }
}

void Controller::SetTransferMatrix( int8_t** tm ){
  for(uint8_t i=0; i<i_channels; i++){
    for(uint8_t o=0; o<o_channels; o++){
      transfer_matrix[o][i] = tm[o][i];
    }
  }
}
