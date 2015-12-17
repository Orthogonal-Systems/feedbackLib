#ifndef ERROR_H
#define ERROR_H

//! Error computation and statistics handling
/*!
 * Calculates errors and error statistics
 */
class Error {
  private:
    const uint8_t channels;       //!< input channels 
    size_t channel_mask;          //!< enabled channels, bitwise
    uint16_t references[channels];   //!< channel setpoints
    uint16_t last_errors[channels];  //!< stores last error calculation
    uint16_t error_mean[channels];   //!< stores mean errors
    uint16_t error_rms[channels];    //!< stores rms errors
    uint16_t smoothing_factor[channels]; //!< discrete time rc-factor

  public: 
    //! Class constructor
    /*!
     * \param _channels is the number of available input channels
     */
    Error( uint8_t _channels ) : channels(_channels);

    //! return the reference points
    uint16_t* GetReferences(){ return references; };

    //! change the references for each channel
    void   SetReferences( uint16_t *_references );

    //! get the last errors, dont refresh
    /*!
     * \return pointer to last_errors data array
     */
    uint16_t* GetErrors(){ return last_errors; };

    //! calculate new error values from inputs
    /*!
     * \param i_vals holds input values from each channel
     * \return newest error value
     *
     * calculates errors, means, and rms values
     */
    uint16_t* CalculateErrors( uint16_t *i_vals );

    //! get the mean error values
    /*!
     * \return channel error means
     */
    uint16_t* GetErrorMean(){ return error_mean; };

    //! get the rms error values
    /*!
     * \return channel error rms
     */
    uint16_t* GetErrorRMS(){ return error_rms; };

    //! get the number of available input channels
    /*!
     * \return available input channels
     */
    const uint8_t GetChannels(){ return channels };

    //! set the smoothing factors
    /*!
     *  \param sf point to smoothing factor array
     *
     *  sets the smoothing facotr as a raw number, to set as time in ms(us) use:
     *  `SetTimeConst_us()` or `SetTimeConst_ms()`
     */
    void SetSmoothingFactor( uint16_t* sf ){
      smoothing_factor = sf;
    }

    //! get the smoothing factors
    /*!
     *  \param sf smoothing factor
     *
     *  gets the smoothing facotr as a raw number, to get as time in ms(us) use:
     *  `GetTimeConst_us()` or `GetTimeConst_ms()`
     */
    uint16_t* SetSmoothingFactor(){
      return smoothing_factor;
    }

    //! set the smoothing factors
    /*!
     *  \param channel number
     *  \param sf smoothing factor for channel n
     *
     *  sets the smoothing facotr as a raw number, to set as time in ms(us) use:
     *  `SetChTimeConst_us()` or `SetChTimeConst_ms()`
     */
    void SetChSmoothingFactor( uint8_t n, uint16_t sf ){
      if ( n < channels ){
        smoothing_factor[n] = sf;
      }
    }

    //! get the smoothing factors
    /*!
     *  \param sf smoothing factor
     *
     *  gets the smoothing facotr as a raw number, to get as time in ms(us) use:
     *  `GetChTimeConst_us()` or `GetChTimeConst_ms()`
     */
    uint16_t GetChSmoothingFactor( uint8_t n ){
      return smoothing_factor[n];
    }

    //! set the smoothing factor in terms of the time constant in ms
    /*!
     * \param rc is the rc time constant in ms
     * \return computed smoothing factors
     */
    uint16_t* SetTimeConst_ms( uint16_t* rc ){
      for( uint8_t i = 0; i<channels; i++ ){
        smoothing_factor[i] = rc[i]/(0xFFFF - rc[i]);
      }
      return smoothing_factor;
    }

    //! set the smoothing factor in terms of the time constant in ms
    /*!
     * \param rc is the rc time constant in ms
     * \return computed smoothing factors
     */
    uint16_t GetTimeConst_ms(){
      for( uint8_t i = 0; i<channels; i++ ){
        smoothing_factor[i] = rc[i]/(0xFFFF - rc[i]);
      }
      return smoothing_factor;
    }

    //! set the smoothing factor in terms of the time constant in ms
    /*!
     * \param rc is the rc time constant in ms
     * \return computed smoothing factors
     */
    uint16_t SetChTimeConst_ms( uint8_t n, uint16_t rc ){
      SetChSmoothingFector(n, rc/(0xFFFF - rc));
      return smoothing_factor[n];
    }

    //! set the smoothing factor in terms of the time constant in ms
    /*!
     * \param rc is the rc time constant in ms
     * \return computed smoothing factors
     */
    uint16_t GetTimeConst_ms(){
      for( uint8_t i = 0; i<channels; i++ ){
        smoothing_factor[i] = rc[i]/(0xFFFF - rc[i]);
      }
      return smoothing_factor;
    }
}

#endif
