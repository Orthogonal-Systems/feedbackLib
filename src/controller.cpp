#include "controller.h"
#include "utility.h"

#define P_INDEX 0
#define I_INDEX 1
#define D_INDEX 2

Controller::Controller(){
  int16_t init_output[o_channels];
  for(uint8_t o=0; o<o_channels; o++){
    init_output[o] = 0;
  }
  init(init_output);
}

Controller::Controller( int16_t* init_output ){
  init(init_output);
}

void Controller::init( int16_t* init_output ){
  Reset(init_output);
  for(uint8_t o=0; o<o_channels; o++){
    weights[o][P_INDEX] = 0x7F;
    weights[o][I_INDEX] = 0x00;
    weights[o][D_INDEX] = 0x00;
  }
}

//! Calculate the next output value based on the
/*!
  * \param errors pointer to current error value
  * \return errors code, 0 for no error
  */
uint8_t Controller::CalcNextValue( errors_t errors ){
  
  int32_t temp;
  for( uint8_t o=0; o<o_channels; o++ ){
    temp = ((int32_t)(weights[o][P_INDEX] * errors.error_P[o])) >> 7;
    temp += ((int32_t)(weights[o][I_INDEX] * errors.error_I[o])) >> 7;
    temp += ((int32_t)(weights[o][D_INDEX] * errors.error_D[o])) >> 7;
    output[o] = CheckRange( temp, 
        FEEDBACK_OUTPUT_U_BOUND, 
        FEEDBACK_OUTPUT_L_BOUND 
    );
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
