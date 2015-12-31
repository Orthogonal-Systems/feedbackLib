#include "feedback.h"
#include "feedback_conf.h"
#include "error.h"
#include "io.h"
#include "controller.h"

Feedback::Feedback(){
  init(0); // 2**0 = 1 averages
}

Feedback::Feedback( uint8_t avgs ){
  init(avgs); // 2**avgs = # of averages
}

void Feedback::init( const uint8_t avgs ){
  SetAverages(avgs);
  // initialize member classes with const fields
  io = IO();
  err = Error();
  int16_t inits[o_channels]; 
  for(uint8_t i=0; i<o_channels; i++){
    inits[i]=0x0800;
  }
  ctrl = Controller(inits);
  //Update(); // push initial outputs to DAC
}

uint8_t Feedback::IOInit(){
  // TODO: turn off synchronous dac updates in init by default
  return io.Init(); // setup I/O device, >0 if error
}

uint8_t Feedback::Measure( ){
  return Measure( averages );
}

uint8_t Feedback::Measure( const uint8_t n ){
  uint8_t error = io.ReadInputs( n );
  // generate output value to be pushed when update is called
  if( error > 0 ){
    return error;
  }
  const int16_t *ins = io.GetLastInputs();
  uint16_t deltaT_ms = (uint16_t)(io.GetDeltaT_us()/1000);
  const errors_t errs = err.CalculateErrors( ins, deltaT_ms );
  ctrl.CalcNextValue( errs );
  // push corrected outputs to system, update occurs on trigger
  io.SetOutputs( ctrl.GetNextValue() );
  return 0;
}

void Feedback::Update(){
  io.UpdateDAC();
}

uint8_t Feedback::SetAverages( const uint8_t n ){
  if( n > FEEDBACK_MAX_AVGS ){
    averages = FEEDBACK_MAX_AVGS;
  } else {
    averages = n;
  }
  return averages;
}
