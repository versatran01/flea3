#include "flea3/stereo_node.h"

namespace flea3 {

void StereoNode::Acquire() {
  while (is_acquire() && ros::ok()) {
    if (left_ros_.RequestSingle() && right_ros_.RequestSingle()) {
      const auto expose_duration =
          ros::Duration(left_ros_.camera().getExposureTimeSec());
      const auto time = ros::Time::now() + expose_duration;
      left_ros_.PublishCamera(time);
      right_ros_.PublishCamera(time);
    } else {
      ROS_WARN("%s or %s request failed. Reduce frame rate",
               left_ros_.identifier().c_str(), right_ros_.identifier().c_str());
    }
    Sleep();
  }
}

void StereoNode::Setup(Flea3DynConfig& config) {
  left_ros_.Stop();
  right_ros_.Stop();
  left_ros_.set_fps(config.fps);
  right_ros_.set_fps(config.fps);
  auto config_cpy = config;
  left_ros_.camera().Configure(config_cpy);
  right_ros_.camera().Configure(config);
  left_ros_.Start();
  right_ros_.Start();
}

}  // namespace flea3