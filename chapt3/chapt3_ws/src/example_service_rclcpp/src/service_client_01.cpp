#include "rclcpp/rclcpp.hpp"
#include "example_interfaces/srv/add_two_ints.hpp"

class ServiceClient01 : public rclcpp::Node {
public:
  // 构造函数,有一个参数为节点名称
  ServiceClient01(std::string name) : Node(name) {
    RCLCPP_INFO(this->get_logger(), "节点已启动：%s.", name.c_str());
    //create client
    // client_ = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints_srv");
    client_ = this->create_client<example_interfaces::srv::AddTwoInts>("add_two_ints_srv");
  }
  // send_request function
  void send_request(int a, int b){
    RCLCPP_INFO(this->get_logger(), "calculate%d+%d",a,b);
    //1. wait for sercice online
    while(!client_->wait_for_service(std::chrono::seconds(1))){

      //rclcpp states when waiting for checking
      if(!rclcpp::ok()){
        RCLCPP_ERROR(this->get_logger(),"等待服务的过程中被打断...");
        return;
      }
      RCLCPP_INFO(this->get_logger(),"等待服务端上线中");
    }
    //2. formulate request
    auto requests=std::make_shared<example_interfaces::srv::AddTwoInts_Request>();
    requests->a = a;
    requests->b = b;

    //3.send asychronous request and wait return. when return call callback function
    client_->async_send_request(requests,std::bind(&ServiceClient01::result_callback_,this,std::placeholders::_1));
  };
private:
  //declear client
  rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedPtr client_;

  void result_callback_(rclcpp::Client<example_interfaces::srv::AddTwoInts>::SharedFuture result_future){
    auto response = result_future.get();//get is function from shared_future API
    RCLCPP_INFO(this->get_logger(),"Calculate result: %ld",response->sum);
  };
};

int main(int argc, char** argv) {
  rclcpp::init(argc, argv);
  /*创建对应节点的共享指针对象*/
  auto node = std::make_shared<ServiceClient01>("service_client_01");
  /* 运行节点，并检测退出信号*/
  //calculate 5+6
  node->send_request(5,6);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}