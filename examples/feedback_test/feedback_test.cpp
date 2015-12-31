#include <Arduino.h>
#include "feedback.h"
#include "error.h"
#include "feedback_conf.h"

const char init_str[] = "Initializing : ";
const char suc_str[] = "Success";
const char err_str[] = "Failure, code: 0x";

const int16_t *data;  // pointer to data from device

Feedback fb;
uint8_t trigPin = 3;

uint16_t cnt=0;

int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setupTrigger(){
  pinMode( trigPin, INPUT );
}

// poll for change on pin
// cycle cant be too fast, or it will get out sync
// TODO: make more robust
void waitForTrigger(){
  uint8_t initial = digitalRead( trigPin );
  Serial.printf( "waiting for %s transition\n", (initial ? "high" : "low") );
  delay(100); // remove
  while(1){
    //if( digitalRead( trigPin ) != initial ){
      break;
    //}
  }
}

void setup (){
  fb = Feedback();
  setupTrigger();

  Serial.begin(115200);

  // IO device initialization
  Serial.print("\n\n");
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
    refs[i] = 655;
  }
  fb.err.SetReferences( refs );

  // setup error matrix
  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    fb.err.SetErrorMatrixEntry(i,i,-32);
  }
}

void loop(){
  //uint8_t ch = 3;

  waitForTrigger();
  uint8_t err = fb.Measure();
  if (err){
    Serial.printf("\nmeasure error recieved: 0x%x\n\n", err);
  } else {

    data = fb.io.GetLastInputs();
    Serial.printf("iteration: 0x%04X\n", cnt);

    Serial.println("inputs:");
    for(uint8_t i=0; i<I_CHANNELS; i++){
    //for(uint8_t i=ch; i<ch+1; i++){
      Serial.printf("ch[%d]: %d\n", i, data[i]);
    }
    //Serial.println("");
      
    const int16_t *p_errors = fb.err.GetPErrors();
    const int16_t *i_errors = fb.err.GetIErrors();
    const int16_t *d_errors = fb.err.GetDErrors();

    Serial.println("errors:");
    for(uint8_t i=0; i<I_CHANNELS; i++){
    //for(uint8_t i=ch; i<ch+1; i++){
      Serial.printf("ch[%d]: P: %05d, I: %05d, D: %05d\n", i, p_errors[i], i_errors[i], d_errors[i]);
    }
    //Serial.println("");

    waitForTrigger();
    fb.Update();
    data = fb.ctrl.GetNextValue();
    Serial.println("outs:");
    for(uint8_t i=0; i<O_CHANNELS; i++){
  //for(uint8_t i=ch; i<ch+1; i++){
    Serial.printf("ch[%d]: %d\n", i, data[i]);
    }
    Serial.println("\n");

    cnt++;
  }
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
