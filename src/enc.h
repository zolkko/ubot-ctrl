
#ifndef __enc_h__
#define __enc_h__

#include <stdint.h>
#include <stdbool.h>
#include <atomic>

namespace ubot
{

class Enc
{
public:
    Enc(const PinName pin);

    Enc(const Enc& enc)
        : _tim(enc._tim),
          _channel(enc._channel),
          _cc_it(enc._cc_it),
          _cc_flag(enc._cc_flag),
          _of_flag(enc._of_flag),
          _ovf(enc._ovf),
          _values_index(enc._values_index)
    {
        memcpy(_values, enc._values, sizeof(_values));
        _speed.store(enc._speed.load());
    }

    void enable_it(void);

    void disable_it(void);
   
    bool handle_it(void);

    int16_t get_speed(void) const {
        return _speed.load();
    }

private:
    bool is_update(void);

    void clear_update(void);

    uint32_t get_cc(void);
   
    bool is_cc(void);

    void clear_cc(void);

    bool is_of(void);

    void clear_of(void);

    TIM_HandleTypeDef _tim;
    uint32_t _channel;
    uint32_t _cc_it;
    uint32_t _cc_flag;
    uint32_t _of_flag;

    uint8_t  _ovf;
    uint8_t  _values_index;
    uint32_t _values[2];

    std::atomic_short _speed;
};

} // namespace ubot

#endif // __enc_h__

