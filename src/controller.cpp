#include "controller.h"

//! Calculate the next output value based on the
/*!
  * \param errors pointer to current error value
  * \return errors code, 0 for no error
  */
uint8_t CalcNextValue( itype *errors ){
  for( uint8_t o; o<o_channels; o_channels++ ){
    for( uint8_t i; i<i_channels; i_channels++ ){
      (((int32_t)transfer_matrix[o][i]) * errors[i]
    }
  }
};

//! Get the next value to push to the output
/*!
  * \return next value to push to the output
  */
otype* GetNextValue( itype *errors ){ return new_output; };

//! Resets all feedback memory
/*!
  * \param val to write to the stored last values
  *
  * Override the last_output value stored and clear history.
  * It is necessary to initialize to a reasonable output
  */
void Reset( otype *val );
//void Flush(); // is this necessary?

//! Return the number of available input channels
/*!
  * \return number of input channels availble on the device
  */
const uint8_t GetIChannels(){ return i_channels };

//! Return the number of available output channels
/*!
  * \return number of output channels availble on the device
  */
const uint8_t GetOChannels(){ return o_channels };
