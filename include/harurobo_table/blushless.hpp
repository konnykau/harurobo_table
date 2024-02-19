#include <can_utils.hpp>
#include "harurobo_table/send_data.hpp"

class robomaster_DC{
    const uint16_t BID;
    const uint16_t ctr_id;
    robomaster_DC(const int ID)
    :BID(0x200),ctr_id(ID)
    {

    }
    std::unique_ptr<can_plugins2::msg::Frame> current_control(int current_target){
        return can_utils::generate_frame(BID,current_target);
    }
};