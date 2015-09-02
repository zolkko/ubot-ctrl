
#ifndef __enc_h__
#define __enc_h__

namespace ubot
{

class Enc
{
public:
    Enc(const PinName pin);

    void enable_it(void);

    void disable_it(void);
   
    void handle_it(void);

    uint32_t get_speed(void) const {
        return _speed;
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

    uint8_t  _values_index;
    uint32_t _values[2];

    uint32_t _speed;
};

} // namespace ubot

#endif // __enc_h__

