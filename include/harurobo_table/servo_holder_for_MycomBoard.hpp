#include <can_utils.hpp>

class servo_for_MycomBoard{
    private : 
    const uint16_t MODE_ID;
    const uint16_t servo_1_ID;
    const uint16_t servo_2_ID;
    const uint16_t servo_3_ID;

    public :
    servo_for_MycomBoard(uint16_t modeID,uint16_t ID1,uint16_t ID2,uint16_t ID3)
    :MODE_ID(modeID),servo_1_ID(ID1),servo_2_ID(ID2),servo_3_ID(ID3)
    {

    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_on()
    {
        return can_utils::generate_frame(this->MODE_ID,static_cast<uint8_t>(0x1));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_off()
    {
        return can_utils::generate_frame(this->MODE_ID,static_cast<uint8_t>(0x0));
    }
    std::unique_ptr<can_plugins2::msg::Frame> update(uint8_t servo_number,bool servo_state){
        switch (servo_number)
        {
        case 1:
            if(servo_state){
                return can_utils::generate_frame(this->servo_1_ID,static_cast<uint8_t>(0x1));
            }
            else{
                return can_utils::generate_frame(this->servo_1_ID,static_cast<uint8_t>(0x0));
            }
        case 2:
            if(servo_state){
                return can_utils::generate_frame(this->servo_2_ID,static_cast<uint8_t>(0x1));
            }
            else{
                return can_utils::generate_frame(this->servo_2_ID,static_cast<uint8_t>(0x0));
            } 
        case 3:
            if(servo_state){
                return can_utils::generate_frame(this->servo_3_ID,static_cast<uint8_t>(0x1));
            }
            else{
                return can_utils::generate_frame(this->servo_3_ID,static_cast<uint8_t>(0x0));
            }                        
        
        default:
            return can_utils::generate_frame(0x919,static_cast<uint8_t>(0x0));
        }
    }

};