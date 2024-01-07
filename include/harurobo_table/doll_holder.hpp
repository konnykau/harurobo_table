#include <can_utils.hpp>
constexpr uint16_t OPEN_CCR = 45000;
constexpr uint16_t CLOSE_CCR = 15000;
enum class servo_state{close,open};
enum class servo_member{left,center,right};

class servo_holder{
    private:
    const uint16_t CAN_ID;
    uint16_t CCRs[4];
    servo_state SERVO_STATE[4];
    // left = 0 center = 1 right = 2  3は使わない

    public:
    servo_holder(uint16_t CAN_ID)
    :CAN_ID(CAN_ID)
    {
    for(int i = 0;i < 4;i++){
        CCRs[i] = 0;
        SERVO_STATE[i] = servo_state::close;
    }
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_on()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x1));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_off()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x0));
    }
    void update(servo_member updating_subject){
        switch(updating_subject){
            case servo_member::left :
                if(SERVO_STATE[0] == servo_state::close){
                    SERVO_STATE[0] = servo_state::open;
                    CCRs[0] = OPEN_CCR;
                }
                else{
                    SERVO_STATE[0] = servo_state::close;
                    CCRs[0] = CLOSE_CCR; 
                }
            break;
            case servo_member::center :
                if(SERVO_STATE[1] == servo_state::close){
                    SERVO_STATE[1] = servo_state::open;
                    CCRs[1] = OPEN_CCR;
                }
                else{
                    SERVO_STATE[1] = servo_state::close;
                    CCRs[1] = CLOSE_CCR; 
                }

            break;
            case servo_member::right :
                if(SERVO_STATE[2] == servo_state::close){
                    SERVO_STATE[2] = servo_state::open;
                    CCRs[2] = OPEN_CCR;
                }
                else{
                    SERVO_STATE[2] = servo_state::close;
                    CCRs[2] = CLOSE_CCR; 
                }

            break;


        }
    }
    std::unique_ptr<can_plugins2::msg::Frame> send_servo_state()
    {
        uint64_t returner = 0;
        for(int i = 0;i < 4;i++){
            returner = 65536*returner;
            returner = returner + CCRs[i];
        }
        return can_utils::generate_frame(CAN_ID + 1,returner);

        
    }


};