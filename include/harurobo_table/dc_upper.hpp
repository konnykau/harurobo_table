#include <can_utils.hpp>
#include "harurobo_table/send_data.hpp"
// constexpr float POS_TARGET = static_cast<float>(1.0);
constexpr float VEL_TARGET = static_cast<float>(1.0);

enum class upper_state{down,up};

class DC_upper{
    private:
    const uint16_t CAN_ID;//CAN ID
    upper_state Upper_state;
    const float POS_TARGET;

    public:
    DC_upper(int CAN_ID)//velocityモードで使う用
    :CAN_ID(CAN_ID),Upper_state(upper_state::down),POS_TARGET(0)
    {}//初期化
    DC_upper(int CAN_ID,float TARGET)//positionモードで使う用
    :CAN_ID(CAN_ID),Upper_state(upper_state::down),POS_TARGET(TARGET)
    {}//初期化
    std::unique_ptr<can_plugins2::msg::Frame> update()
    {
        if(Upper_state == upper_state::down){
            Upper_state = upper_state::up;
            return this->rise_by_pos();
        }
        else{
            Upper_state = upper_state::down;
            return this->fall_by_pos();
        }
        
    }
    std::unique_ptr<can_plugins2::msg::Frame> rise_by_pos()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,POS_TARGET);
    }
    std::unique_ptr<can_plugins2::msg::Frame> fall_by_pos()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,0.0f);
    }
    std::unique_ptr<can_plugins2::msg::Frame> rise_by_vel()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,VEL_TARGET);
    }
    std::unique_ptr<can_plugins2::msg::Frame> fall_by_vel()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,-VEL_TARGET);
    }
    std::unique_ptr<can_plugins2::msg::Frame> stop_vel()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,0.0f);
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_vel()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x5));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_pos()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x6));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_dis()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x0));
    }

};

class DC_upper_pos{
    private:
    const uint16_t CAN_ID;//CAN ID
    upper_state Upper_state;
    const float POS_TARGET;

    public:
    DC_upper_pos(int CAN_ID,float TARGET)//positionモードで使う用
    :CAN_ID(CAN_ID),Upper_state(upper_state::down),POS_TARGET(TARGET)
    {}//初期化
    std::unique_ptr<can_plugins2::msg::Frame> update()
    {
        if(Upper_state == upper_state::down){
            Upper_state = upper_state::up;
            return this->rise_by_pos();
        }
        else{
            Upper_state = upper_state::down;
            return this->fall_by_pos();
        }
        
    }
    std::unique_ptr<can_plugins2::msg::Frame> rise_by_pos()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,POS_TARGET);
    }
    std::unique_ptr<can_plugins2::msg::Frame> fall_by_pos()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,0.0f);
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_pos()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x6));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_dis()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x0));
    }
};

class DC_upper_vel{
    private:
    const uint16_t CAN_ID;//CAN ID
    upper_state Upper_state;
    const float POS_TARGET;

    public:
    DC_upper_vel(int CAN_ID)//velocityモードで使う用
    :CAN_ID(CAN_ID),Upper_state(upper_state::down),POS_TARGET(0)
    {}//初期化
    std::unique_ptr<can_plugins2::msg::Frame> rise_by_vel()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,VEL_TARGET);
    }
    std::unique_ptr<can_plugins2::msg::Frame> fall_by_vel()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,-VEL_TARGET);
    }
    std::unique_ptr<can_plugins2::msg::Frame> stop_vel()
    {
        return can_utils::shirasu_target(this->CAN_ID + 1,0.0f);
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_vel()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x5));
    }
    std::unique_ptr<can_plugins2::msg::Frame> mode_dis()
    {
        return can_utils::generate_frame(this->CAN_ID,static_cast<uint8_t>(0x0));
    }
};