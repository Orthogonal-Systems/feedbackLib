#include "io.h"
#include "Arduino.h"    // micros
#include "amc7812err.h" // error codes
#include "amc7812.h" // error codes

//! Initiaizes driver and device to prepare for feedback.
/*!
  * \return status code, 0 for success
  */
// TODO: add setting/options
uint8_t IO::Init(){
  switch(amc7812.begin()){
    case 0:   // nominial responses
      status = IO_STATUS_OK;
      break;
    case AMC7812_WRITE_ERR:   // confirmation didn't match expected
      status = IO_STATUS_NC;  // non-fatal error
      return status;
    default:
      status = IO_STATUS_FATAL;
  }

  // passed initial setup read channel masks from driver
  i_ch_mask = amc7812.GetADCStatus();
  o_ch_mask = amc7812.GetDACStatus();

  // set mode to triggered
  SetTriggeredADCMode();
  SetTriggeredDACMode();
  return 0;
}

//! Perform a read operation, updating i_vals data array
/*!
  * \return device error code, 0 is for no error
  *
  * Read enabled input channels from device, data array is stored retrieve with
  * `GetLastInputs()`.
  * Value of non-enabled channels is not defined at this level, check the driver.
  */
int8_t IO::ReadInputs(){
  uint8_t err = amc7812.ReadADCs();
  if( err > 0 ){
    return err;
  }
  // set timestamp for last read value
  uint32_t read_time_us = micros();
  deltaT_us = read_time_us - last_call_us;
  last_call_us = read_time_us;
  // retrieve values
  const uint16_t* adc_vals = amc7812.GetADCReadings();
  for( uint8_t i=0; i<i_channels; i++){
    i_vals[i] = (int16_t)adc_vals[i];
  }
  return 0;
}

void IO::SetOutputs( const int16_t* setpoints ){
  // TODO: convert from signed to unsigned?
  for( uint8_t i=0; i<o_channels; i++ ){
    amc7812.WriteDAC( i, (uint16_t)setpoints[i] );
  }
}

//! Set the bitwise input channel enabled mask and update device
/*!
  * Enabled channels are denoted by a 1 in the corresponding bit.
  */
void IO::SetEnabledIChannels( const uint16_t mask){
  for( uint8_t i=0; i<i_channels; i++ ){
    if( (mask>>i)&(0x01) ){
      amc7812.EnableADC(i);
    }
  }
  i_ch_mask = mask;
}

//! Set the bitwise output channel enabled mask and update device
/*!
  * Enabled channels are denoted by a 1 in the corresponding bit.
  */
void IO::SetEnabledOChannels( const uint16_t mask){
  for( uint8_t i=0; i<o_channels; i++ ){
    if( (mask>>i)&(0x01) ){
      amc7812.EnableDAC(i);
    }
  }
  o_ch_mask = mask;
}

//! Set the ADC update mode to triggered
/*!
  *  The adc values will only be pushed to the outputs when triggered
  */
void IO::SetTriggeredADCMode(){
  amc7812.SetTriggeredADCMode();
}

//! Set the ADC update mode to continuous
/*!
  *  The adc values will be pushed to the outputs as soon as the register is
  *  updated
  */
void IO::SetContinuousADCMode(){
  amc7812.SetContinuousADCMode();
}

//! Set the DAC update mode to triggered
/*!
  *  The dac values will only be pushed to the outputs when triggered
  */
void IO::SetTriggeredDACMode(){
  amc7812.SetTriggeredDACMode();
}

//! Set the DAC update mode to continuous
/*!
  *  The dac values will be pushed to the outputs as soon as the register is
  *  updated
  */
void IO::SetContinuousDACMode(){
  amc7812.SetContinuousDACMode();
}
