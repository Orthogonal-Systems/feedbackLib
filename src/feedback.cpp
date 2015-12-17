#include "feedback.h"

Feedback:Feedback( i_channels, o_channels ){
  // initialize member classes with const fields
  io = IO( i_channels, o_channels );
  err = Error( i_channels );
  ctrl = Controller( i_channels, o_channels );
}

void Feedback::Init(){
  return io.Init(); // setup I/O device, >0 if error
}

uint8_t Feedback::Measure(){
  uint8_t err = io.ReadInputs();
  // generate output value to be pushed when update is called
  if( err > 0 ){
    return err;
  }
  ctrl.CalcNextValue( err.GetErrors( io.GetLastInputs() ) );
  // push corrected outputs to system, update occurs on trigger
  io.SetOutputs( ctrl.GetNextValue() );
  return 0;
}

void Feedback::Update(){
  io.UpdateDAC();
}
