#include "feedback.h"
#include "error.h"
#include "io.h"
#include "controller.h"

Feedback::Feedback(){
  // initialize member classes with const fields
  io = IO();
  err = Error();
  int16_t inits[o_channels]; 
  for(uint8_t i=0; i<o_channels; i++){
    inits[i]=0;
  }
  ctrl = Controller(inits);
}

uint8_t Feedback::Init(){
  // TODO: turn off synchronous dac updates in init by default
  return io.Init(); // setup I/O device, >0 if error
}

uint8_t Feedback::Measure(){
  uint8_t error = io.ReadInputs();
  // generate output value to be pushed when update is called
  if( error > 0 ){
    return error;
  }
  int16_t* ins = io.GetLastInputs();
  int16_t* errs = err.CalculateErrors( ins );
  ctrl.CalcNextValue( errs );
  // push corrected outputs to system, update occurs on trigger
  io.SetOutputs( ctrl.GetNextValue() );
  return 0;
}

void Feedback::Update(){
  io.UpdateDAC();
}
