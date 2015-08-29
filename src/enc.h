
#ifndef __enc_h__
#define __enc_h__

namespace ubot
{

class Enc
{
public:
    Enc(const PinName pin);

    void set(const uint32_t value);

    uint32_t get(void) const;

protected:
    TIM_HandleTypeDef _tim;
    uint8_t _channel;
};

} // namespace ubot

#endif // __enc_h__

