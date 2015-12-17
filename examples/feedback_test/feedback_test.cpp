#include <Arduino.h>

#define I_CHANNELS 12 // 16 available, but start with nxn transfer matrix
#define O_CHANNELS 12

const init_str[] = "Initializing : ";
const suc_str[] = "Success";
const err_str[] = "Failure, code: 0x";

Feedback fb;

void setup ()
{
  fb = Feedback( I_CHANNELS, O_CHANNELS );

  Serial.begin(9600);

  // IO device initialization
  Serial.print( init_str[] );
  Serial.print( "IO Device :" );
  uint8_t ret = fb.io.Init();
  while ( ret ){
    Serial.print( err_str );
    Serial.println(ret, HEX); // error code
    delay(1000);
    ret = fb.io.begin();
  }
  Serial.println( suc_str );

  // set points
  uint16_t refs[I_CHANNELS];
  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    refs[i] = i<<4;
  }
  fb.err.SetReferences( refs );

  // setup controller
  for( uint8_t i=0; i<I_CHANNELS; i++ ){
    fb.ctrl.SetTransferMatrixEntry(i,i,1);
  }
}

void loop(){
  fb.Measure();
  fb.Update();
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
