// -*- mode: C++ -*-
/*********************************************************************
 * Software License Agreement (BSD License)
 *
 *  Copyright (c) 2013, Ryohei Ueda and JSK Lab
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/o2r other materials provided
 *     with the distribution.
 *   * Neither the name of the Willow Garage nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *********************************************************************/

#ifndef JSK_PCL_ROS_ORGANIZED_PLANE_SEGMENTATION_H_
#define JSK_PCL_ROS_ORGANIZED_PLANE_SEGMENTATION_H_

#include <ros/ros.h>
#include <ros/names.h>

#include "jsk_pcl_ros/ClusterPointIndices.h"
#include "sensor_msgs/PointCloud2.h"
#include <pcl_ros/pcl_nodelet.h>
#include <pcl/segmentation/organized_multi_plane_segmentation.h>
#include <dynamic_reconfigure/server.h>
#include "jsk_pcl_ros/OrganizedMultiPlaneSegmentationConfig.h"

namespace jsk_pcl_ros
{
  class OrganizedMultiPlaneSegmentation: public pcl_ros::PCLNodelet
  {
  public:
  protected:
    ros::Publisher pub_;
    ros::Subscriber sub_;
    int min_size_;
    double angular_threshold_;
    double distance_threshold_;
    double max_curvature_;
    typedef jsk_pcl_ros::OrganizedMultiPlaneSegmentationConfig Config;
    boost::shared_ptr <dynamic_reconfigure::Server<Config> > srv_;
    boost::mutex mutex_;
    virtual void segment(const sensor_msgs::PointCloud2::ConstPtr& msg);
    virtual void configCallback (Config &config, uint32_t level);
  private:
    virtual void onInit();
  };
}

#endif
