#pragma once
#ifndef DSY_OSCILLATOR_H
#define DSY_OSCILLATOR_H
#include <stdint.h>
#ifdef __cplusplus

namespace daisysp
{
/** Synthesis of several waveforms, including polyBLEP bandlimited waveforms.
*/
class Oscillator
{
  public:
    Oscillator() {}
    ~Oscillator() {}
    /** Choices for output waveforms, POLYBLEP are appropriately labeled. Others are naive forms.
    */
    enum
    {
        WAVE_SIN,
        WAVE_TRI,
        WAVE_SAW,
        WAVE_RAMP,
        WAVE_SQUARE,
        WAVE_POLYBLEP_TRI,
        WAVE_POLYBLEP_SAW,
        WAVE_POLYBLEP_SQUARE,
        WAVE_LAST,
    };


    /** Initializes the Oscillator

        \param sample_rate - sample rate of the audio engine being run, and the frequency that the Process function will be called.

        Defaults:
        - freq_ = 100 Hz
        - amp_ = 0.5
        - waveform_ = sine wave.
    */
    void Init(float sample_rate)
    {
        sr_        = sample_rate;
        sr_recip_  = 1.0f / sample_rate;
        freq_      = 100.0f;
        amp_       = 0.5f;
        phase_     = 0.0f;
        phase_inc_ = CalcPhaseInc(freq_);
        waveform_  = WAVE_SIN;
    }


    /** Changes the frequency of the Oscillator, and recalculates phase increment.
    */
    inline void SetFreq(const float f)
    {
        freq_      = f;
        phase_inc_ = CalcPhaseInc(f);
    }


    /** Sets the amplitude of the waveform.
    */
    inline void SetAmp(const float a) { amp_ = a; }
    /** Sets the waveform to be synthesized by the Process() function.
    */
    inline void SetWaveform(const uint8_t wf)
    {
        waveform_ = wf < WAVE_LAST ? wf : WAVE_SIN;
    }


    /** Processes the waveform to be generated, returning one sample. This should be called once per sample period.
    */
    float Process();


    /** Adds a value 0.0-1.0 (mapped to 0.0-TWO_PI) to the current phase. Useful for PM and "FM" synthesis.
    */
    void PhaseAdd(float _phase) { phase_ += (_phase * float(M_TWOPI)); }
    /** Resets the phase to the input argument. If no argument is present, it will reset phase to 0.0;
    */
    void Reset(float _phase = 0.0f) { phase_ = _phase; }
  private:
    float CalcPhaseInc(float f);
    uint8_t waveform_;
    float   amp_, freq_;
    float   sr_, sr_recip_, phase_, phase_inc_;
    float   last_out_, last_freq_;
};
} // namespace daisysp
#endif
#endif
