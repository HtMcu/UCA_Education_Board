#include "Si4432.h"

Si4432::Si4432(Module* mod) : Si443x(mod) {

}

int16_t Si4432::begin(float freq, float br, float freqDev, float rxBw, int8_t power) {
  // execute common part
  int16_t state = Si443x::begin(br, freqDev, rxBw);
  RADIOLIB_ASSERT(state);

  // configure publicly accessible settings
  state = setFrequency(freq);
  RADIOLIB_ASSERT(state);

  state = setOutputPower(power);
  RADIOLIB_ASSERT(state);

  return(state);
}

int16_t Si4432::setFrequency(float freq) {
  RADIOLIB_CHECK_RANGE(freq, 240.0, 930.0, ERR_INVALID_FREQUENCY);

  // set frequency
  return(Si443x::setFrequencyRaw(freq));
}

int16_t Si4432::setOutputPower(int8_t power) {
  RADIOLIB_CHECK_RANGE(power, -1, 20, ERR_INVALID_OUTPUT_POWER);

  // set output power
  return(_mod->SPIsetRegValue(SI443X_REG_TX_POWER, (uint8_t)((power + 1) / 3), 2, 0));
}
