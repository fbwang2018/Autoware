#ifndef NODE_STATUS_PUBLISHER_H_INCLUDED
#define NODE_STATUS_PUBLISHER_H_INCLUDED

//headers in ROS
#include <ros/ros.h>

//headers in Autoware
#include <autoware_health_checker/constants.h>
#include <autoware_health_checker/diag_buffer.h>
#include <autoware_system_msgs/NodeStatus.h>

//headers in STL
#include <map>
#include <memory>
#include <functional>

class NodeStatusPublisher
{
public:
    NodeStatusPublisher(ros::NodeHandle nh,ros::NodeHandle pnh);
    ~NodeStatusPublisher();
    void CHECK_MIN_VALUE(std::string key,double value,double warn_value,double error_value,double fatal_value);
    void CHECK_MAX_VALUE(std::string key,double value,double warn_value,double error_value,double fatal_value);
    // std::pair<double,double> first value is min value and second value is max value
    void CHECK_RANGE(std::string key,double value,std::pair<double,double> warn_value,std::pair<double,double> error_value,std::pair<double,double> fatal_value);
    template<class T>
    void CHECK_VALUE(std::string key,T value,std::function<uint8_t(T value)> check_func)
    {
        addNewBuffer(key);
        uint8_t check_result = check_func(value);
        autoware_system_msgs::DiagnosticStatus new_status;
        new_status.type = autoware_system_msgs::DiagnosticStatus::OUT_OF_RANGE;
        new_status.level = check_result;
        diag_buffers_[key]->addDiag(new_status);
    }
    void CHECK_RATE(std::string key,double warn_rate,double error_rate,double fatal_rate);
private:
    ros::NodeHandle nh_;
    ros::NodeHandle pnh_;
    std::map<std::string,std::shared_ptr<DiagBuffer> > diag_buffers_;
    ros::Publisher status_pub_;
    bool keyExist(std::string key);
    void addNewBuffer(std::string key);
};

#endif  //NODE_STATUS_PUBLISHER_H_INCLUDED