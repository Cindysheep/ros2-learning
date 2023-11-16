#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class TopicPublisher01 : public rclcpp::Node{

    public:
    //build function, one argument is node name'
    TopicPublisher01(std::string name) : Node(name){
        RCLCPP_INFO(this->get_logger(), "大家好，我是%s.", name.c_str());
        //create publisher
        command_publisher_ = this->create_publisher<std_msgs::msg::String>("command",10);
        //create timer
        timer_ = this->create_wall_timer(std::chrono::milliseconds(500), std::bind(&TopicPublisher01::timer_callback, this));

    }
    private:
        void timer_callback(){
            //create messege
            std_msgs::msg::String messege;
            messege.data = "forward";
            //print logs
            RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", messege.data.c_str());
            //publish msg
            command_publisher_->publish(messege);
        }
        //declear timer pointer
        rclcpp::TimerBase::SharedPtr timer_;
        //declear timer publisher pointer
        rclcpp::Publisher<std_msgs::msg::String>::SharedPtr command_publisher_;
        
};

int main(int argc, char **argv){

    rclcpp::init(argc, argv);
    /*create a new node*/
    auto node = std::make_shared<TopicPublisher01>("topic_publisher_01");
    /*run node and detecte exit signal*/
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;

}