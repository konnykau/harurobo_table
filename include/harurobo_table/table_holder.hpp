#include <can_utils.hpp>
#include "send_data.hpp"

enum class holder_state{close,open};
class solv_sender{
    private:
    holder_state solv_state;
    const uint16_t BID;


    public:
    solv_sender(int SOLV_ID)
    :solv_state(holder_state::close),BID(SOLV_ID)
    {

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
        if(this->solv_state == holder_state::open){
            solv_state = holder_state::close;
            return close();            
        }
        else{
            solv_state = holder_state::open;
            return open();            
        }
        
    }
    std::unique_ptr<can_plugins2::msg::Frame> close()
    {
        return can_utils::shirasu_target(this->BID + 1,static_cast<uint8_t>(0x01));
    }
    std::unique_ptr<can_plugins2::msg::Frame> open()
    {
        return can_utils::shirasu_target(this->BID + 1,static_cast<uint8_t>(0x00));
    }
    

};