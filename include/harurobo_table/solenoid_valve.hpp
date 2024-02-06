#include <can_utils.hpp>
#include "send_data.hpp"

enum class holder_state{unpowered,powered};
class solv_sender{
    private:
    holder_state solv_state[8];
    const uint16_t BID;


    public:
    solv_sender(int SOLV_ID)
    :BID(SOLV_ID)
    {
        for(int i = 0;i<8;i++){
            solv_state[i] = holder_state::unpowered;
        }
    }

    std::unique_ptr<can_plugins2::msg::Frame> mode_on()
    {
        return can_utils::generate_frame(this->BID,static_cast<uint8_t>(0x1));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_off()
    {
        return can_utils::generate_frame(this->BID,static_cast<uint8_t>(0x0));
    }
    std::unique_ptr<can_plugins2::msg::Frame> update()
    {
        uint8_t sending_data = 0;
        for(int i = 0;i < 8;i++){
            if(solv_state[i] == holder_state::powered){
                sending_data = sending_data + power(2,i);
            }
        }
        return can_utils::generate_frame(this->BID + 1,sending_data);
        
    }
    void unpower(int number)
    {
        solv_state[number] = holder_state::unpowered;
    }
    void power_on(int number)
    {
        solv_state[number] = holder_state::powered;
    }
    // std::unique_ptr<can_plugins2::msg::Frame> open()
    // {
    //     return can_utils::shirasu_target(this->BID + 1,static_cast<uint8_t>(0x00));
    // }
    static int power(int x,unsigned int n){
        if(n == 0)return 1;
        else return x*power(x,n-1);
    }

};