#include <sensor_msgs/msg/joy.hpp>
#include <functional>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include <can_utils.hpp>
#include "harurobo_table/table_holder.hpp"
#include "harurobo_table/doll_holder.hpp"
#include "harurobo_table/table_upper.hpp"
#include "harurobo_table/send_data.hpp"

using std::placeholders::_1;

class holder_and_upper_for_koinobori: public rclcpp::Node
{
public:
  holder_and_upper_for_koinobori()
  : Node("harurobo_koinobori_holder"),table_holder(0x100),table_upper(0x640),doll_holder(0x300),doll_upper(0x660)
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::Joy>(
      "joy", 10, std::bind(&holder_and_upper_for_koinobori::topic_callback, this, _1));//joy == コントローラーの入力をsubscription
    can_pub_ = this->create_publisher<can_plugins2::msg::Frame>("can_tx", 10);//canに対してpublish
  }
  

private:
  void topic_callback(const sensor_msgs::msg::Joy & msg)//この関数が随時実行されるらしい
  {
    if(msg.buttons[9]){//startボタン
      can_pub_->publish(std::move(table_holder.mode_on()));
      can_pub_->publish(std::move(table_upper.mode_pos()));
      can_pub_->publish(std::move(doll_holder.mode_on()));
      can_pub_->publish(std::move(doll_upper.mode_pos()));
    }//mode onにする
    if(msg.buttons[8]){//backボタン
      can_pub_->publish(std::move(table_holder.mode_off()));
      can_pub_->publish(std::move(table_upper.mode_dis()));
      can_pub_->publish(std::move(doll_holder.mode_off()));
      can_pub_->publish(std::move(doll_upper.mode_dis()));
    }//mode offにする
    if(msg.buttons[5]){//台把持
      can_pub_->publish(std::move(table_holder.update(0)));
    }
    if(msg.buttons[4]){//台昇降上げる
      can_pub_->publish(std::move(table_upper.rise_by_vel()));
    }
    else if(msg.buttons[4]){//台昇降下げる
      can_pub_->publish(std::move(table_upper.fall_by_vel()));
    }
    else{//止める
      can_pub_->publish(std::move(table_upper.stop_vel()));
    }

    if(msg.buttons[1]){//人形左翼
      doll_holder.update(servo_member::left);
    }
    if(msg.buttons[1]){//人形中欧
      doll_holder.update(servo_member::center);
    }
    if(msg.buttons[1]){//人形右翼
      doll_holder.update(servo_member::right);
    }
    can_pub_->publish(std::move(doll_holder.send_servo_state()));

    if(msg.buttons[1]){//人形昇降上げる
      can_pub_->publish(std::move(doll_upper.rise_by_vel()));
    }
    else if(msg.buttons[1]){//人形昇降下げる
      can_pub_->publish(std::move(doll_upper.fall_by_vel()));
    }
    else{//止める
      can_pub_->publish(std::move(doll_upper.stop_vel()));
    }
    if(msg.buttons[1]){//人形昇降の将校
      can_pub_->publish(std::move(table_holder.update(1)));
    }
    

  }
  rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr subscription_;
  rclcpp::Publisher<can_plugins2::msg::Frame>::SharedPtr can_pub_;
  solv_sender table_holder;
  DC_upper table_upper;
  servo_holder doll_holder;
  DC_upper doll_upper;
  
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<holder_and_upper_for_koinobori>());
  rclcpp::shutdown();
  return 0;
}
