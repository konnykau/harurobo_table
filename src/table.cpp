#include <sensor_msgs/msg/joy.hpp>
#include <functional>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <can_utils.hpp>
#include "harurobo_table/solenoid_valve.hpp"
#include "harurobo_table/servo_holder.hpp"
#include "harurobo_table/dc_upper.hpp"
#include "harurobo_table/send_data.hpp"
#include "harurobo_table/servo_holder_for_MycomBoard.hpp"

using std::placeholders::_1;

constexpr float UPPER_VEL_TARGET =1.0;

class holder_and_upper_for_koinobori: public rclcpp::Node
{
public:
  holder_and_upper_for_koinobori()
  : Node("harurobo_koinobori_holder"),table_holder(0x100),table_upper(0x200,UPPER_VEL_TARGET),doll_holder(0x20F,0x201,0x202,0x203),doll_upper(0x660,UPPER_VEL_TARGET)
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 10, std::bind(&holder_and_upper_for_koinobori::topic_callback, this, _1));//joy == コントローラーの入力をsubscription
    can_pub_ = this->create_publisher<can_plugins2::msg::Frame>("can_tx", 10);//canに対してpublish
    // PID_pub_ = this->create_publisher<std_msgs::msg::String>("PID_topic", 10);
  }
  

private:
  void topic_callback(const sensor_msgs::msg::Joy & msg)//この関数が随時実行されるらしい
  {
    if(msg.buttons[7]){//startボタン
      can_pub_->publish(std::move(table_holder.mode_on()));
      can_pub_->publish(std::move(table_upper.mode_vel()));
      can_pub_->publish(std::move(doll_holder.mode_on()));
      can_pub_->publish(std::move(doll_upper.mode_vel()));
    }//mode onにする
    if(msg.buttons[6]){//backボタン
      can_pub_->publish(std::move(table_holder.mode_off()));
      can_pub_->publish(std::move(table_upper.mode_dis()));
      can_pub_->publish(std::move(doll_holder.mode_off()));
      can_pub_->publish(std::move(doll_upper.mode_dis()));
    }//mode offにする

    if(msg.buttons[0]&&msg.buttons[5]){//台把持
      table_holder.power_on(0);
    }
    else if(msg.buttons[0]&&msg.buttons[4]){
      table_holder.unpower(0);
    }
    can_pub_->publish(std::move(table_holder.update()));

    can_pub_->publish(table_upper.update(msg.axes[7] == 1,msg.axes[7] == -1));
    //台昇降

    if(msg.buttons[2]&&msg.buttons[5]){//人形左翼開方
      can_pub_->publish(std::move(doll_holder.update(1,true)));
    }
    else if(msg.buttons[2]&&msg.buttons[4]){//人形左翼閉鎖
      can_pub_->publish(std::move(doll_holder.update(1,false)));
    }
    if(msg.buttons[3]&&msg.buttons[5]){//人形中欧開方
      can_pub_->publish(std::move(doll_holder.update(2,true)));
    }
    else if(msg.buttons[3]&&msg.buttons[4]){//人形中欧閉鎖
      can_pub_->publish(std::move(doll_holder.update(2,false)));
    }
    if(msg.buttons[1]&&msg.buttons[5]){//人形右翼開方
      can_pub_->publish(std::move(doll_holder.update(3,true)));
    }
    else if(msg.buttons[1]&&msg.buttons[4]){//人形右翼閉鎖
      can_pub_->publish(std::move(doll_holder.update(3,false)));
    }
    // can_pub_->publish(std::move(doll_holder.send_servo_state()));
    //人形把持

    can_pub_->publish(doll_upper.update(msg.axes[4] == 1,msg.axes[4] == -1));
    //人形昇降

    // if(msg.axes[6] == 1){//人形昇降の将校
    //   can_pub_->publish(std::move(table_holder.update(1)));
    // }
    

  }
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
  rclcpp::Publisher<can_plugins2::msg::Frame>::SharedPtr can_pub_;
  // rclcpp::Publisher<std_msgs::msg::String>::SharedPtr PID_pub_;
  solv_sender table_holder;
  DC_upper_vel table_upper;
  servo_for_MycomBoard doll_holder;
  DC_upper_vel doll_upper;
  
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<holder_and_upper_for_koinobori>());
  rclcpp::shutdown();
  return 0;
}
