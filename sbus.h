#ifndef SBUS_INPUT
#define SBUS_INPUT

#include "Arduino.h"

enum sbusProtocolStates {
    SBUS_DECODING_STATE_IDLE,
    SBUS_DECODING_STATE_IN_PROGRESS
};

class SbusInput
{
  public:
  	SbusInput(HardwareSerial &serial);
    void start(void);
    void restart(void);
    void loop(void);
    bool isReceiving(void);
    void recoverStuckFrames(void);
    void (* setRcChannelCallback)(uint8_t channel, int value, int offset);
    bool isFailsafe(void);
  private:
  	HardwareSerial &_serial;
    uint32_t _frameDecodingStartedAt = 0;
    uint32_t _frameDecodingEndedAt = 0 ;
    uint8_t _protocolState = SBUS_DECODING_STATE_IDLE;
    uint32_t _lastByteArrivedAt = 0;
	  void sbusRead(void);
    void sbusToChannels(byte buffer[]);
    uint8_t flagByte;
};

void sbusPreparePacket(uint8_t packet[], bool isSignalLoss, bool isFailsafe, int (* rcChannelGetCallback)(uint8_t));

#endif

