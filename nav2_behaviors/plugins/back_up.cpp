// Copyright (c) 2022 Joshua Wallace
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "nav2_behaviors/plugins/back_up.hpp"

namespace nav2_behaviors
{

ResultStatus BackUp::onRun(const std::shared_ptr<const BackUpAction::Goal> command)
{
  if (command->target.y != 0.0 || command->target.z != 0.0) {
    std::string error_msg = "Backing up in Y and Z not supported, will only move in X.";
    RCLCPP_INFO(logger_, error_msg.c_str());
    return ResultStatus{Status::FAILED, BackUpActionResult::INVALID_INPUT, error_msg};
  }

  // Silently ensure that both the speed and direction are negative.
  command_x_ = -std::fabs(command->target.x);
  command_speed_ = -std::fabs(command->speed);
  command_time_allowance_ = command->time_allowance;
  command_disable_collision_checks_ = command->disable_collision_checks;

  end_time_ = this->clock_->now() + command_time_allowance_;

  if (!nav2_util::getCurrentPose(
      initial_pose_, *tf_, local_frame_, robot_base_frame_,
      transform_tolerance_))
  {
    std::string error_msg = "Initial robot pose is not available.";
    RCLCPP_ERROR(logger_, error_msg.c_str());
    return ResultStatus{Status::FAILED, BackUpActionResult::TF_ERROR, error_msg};
  }

  return ResultStatus{Status::SUCCEEDED, BackUpActionResult::NONE, ""};
}

}  // namespace nav2_behaviors

#include "pluginlib/class_list_macros.hpp"
PLUGINLIB_EXPORT_CLASS(nav2_behaviors::BackUp, nav2_core::Behavior)
