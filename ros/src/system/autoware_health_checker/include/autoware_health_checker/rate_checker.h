#ifndef RATE_CHECKER_H_INCLUDED
#define RATE_CHECKER_H_INCLUDED

//headers in ROS
#include <ros/ros.h>

//headers in STL
#include <vector>
#include <mutex>

//headers in Boost
#include <boost/optional.hpp>

//headers in Autoware
#include <autoware_health_checker/constants.h>

class RateChecker
{
    public:
        RateChecker(double buffer_length,double warn_rate,double error_rate,double fatal_rate,std::string description);
        ~RateChecker();
        void check();
        uint8_t getErrorLevel();
    private:
        boost::optional<double> getRate();
        ros::Time start_time_;
        void update();
        std::vector<ros::Time> data_;
        const double buffer_length_;
        const double warn_rate_;
        const double error_rate_;
        const double fatal_rate_;
        std::mutex mtx_;
};
#endif //RATE_CHECKER_H_INCLUDED