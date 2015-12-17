#ifndef FEEDBACK_H
#define FEEDBACK_H

//! Functions to make sure errors are not diverging
class WatchDog {
  public:
    IsOK(){ return true; };
}

//! Top-level feedback class
class Feedback {
  private:

  public:
    IO          io;
    Error       err;
    Controller  ctrl;
    WatchDog    wd;

    //! constructor
    Feedback( uint8_t i_channels, uint8_t o_channels );

    //! Initialize the member classes
    void Init();
    
    //! Measure the system and compute next output value
    void Measure();

    //! Push output values
    void Update();
}

#endif
