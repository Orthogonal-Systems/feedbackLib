#include <Arduino.h>
#include "feedback.h"
#include "feedback_conf.h"

const char init_str[] = "Initializing : ";
const char suc_str[] = "Success";
const char err_str[] = "Failure, code: 0x";

Feedback fb;

void setup (){
  fb = Feedback();

  Serial.begin(9600);

  // IO device initialization
  Serial.print( init_str );
  Serial.print( "IO Device :" );
  uint8_t ret = fb.io.Init();
  while ( ret ){
    Serial.print( err_str );
    Serial.println(ret, HEX); // error code
    delay(1000);
    ret = fb.io.Init();
  }
  Serial.println( suc_str );

  // set points
  int16_t refs[I_CHANNELS];
  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    refs[i] = i<<8;
  }
  fb.err.SetReferences( refs );

  // setup controller
  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    fb.ctrl.SetTransferMatrixEntry(i,i,0x10);
  }

  int16_t* retrefs = fb.err.GetReferences();
  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    Serial.printf("ref[%d] = %d\n", i, retrefs[i]);
  }

  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    for( uint8_t o=0; o<O_CHANNELS; o++ ){
      Serial.printf("tm[%d][%d] = %d\n", o, i, fb.ctrl.GetTransferMatrixEntry(o,i));
    }
  }
}

void loop(){
  uint8_t ch = 1;
  uint8_t err = fb.Measure();
  if (err){
    Serial.printf("\nmeasure error recieved: 0x%x\n\n", err);
  }

  int16_t* ins = fb.io.GetLastInputs();
  Serial.println("inputs:");
  for(uint8_t i=0; i<I_CHANNELS; i++){
  //for(uint8_t i=ch; i<ch+1; i++){
    Serial.printf("ch[%d]: %d\n", i, ins[i]);
  }
  Serial.println("");
    
    
  int16_t* errors = fb.err.GetErrors();
  Serial.println("errors:");
  for(uint8_t i=0; i<I_CHANNELS; i++){
  //for(uint8_t i=ch; i<ch+1; i++){
    Serial.printf("ch[%d]: %d\n", i, errors[i]);
  }
  Serial.println("");

  fb.Update();
  errors = fb.ctrl.GetNextValue();
  Serial.println("outs:");
  for(uint8_t i=0; i<O_CHANNELS; i++){
  //for(uint8_t i=ch; i<ch+1; i++){
    Serial.printf("ch[%d]: %d\n", i, errors[i]);
  }
  Serial.println("\n\n\n");

  delay(5000);
}

// normal arduino main function
int main(void){
  init();

  setup();

  for(;;){
    loop();
    if (serialEventRun) serialEventRun();
  }
  return 0;
}
