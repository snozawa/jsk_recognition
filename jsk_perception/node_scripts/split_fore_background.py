#!/usr/bin/env python
# -*- coding: utf-8 -*-

import cv_bridge
from jsk_recognition_utils.depth import split_fore_background
from jsk_topic_tools import jsk_logwarn
from jsk_topic_tools import ConnectionBasedTransport
import message_filters
import rospy
from sensor_msgs.msg import Image


class SplitForeBackground(ConnectionBasedTransport):

    def __init__(self):
        super(SplitForeBackground, self).__init__()
        self.fg_pub_ = self.advertise('~output/fg', Image, queue_size=10)
        self.bg_pub_ = self.advertise('~output/bg', Image, queue_size=10)

    def subscribe(self):
        self.sub_ = message_filters.Subscriber('~input', Image)
        self.sub_depth_ = message_filters.Subscriber('~input/depth', Image)
        sync = message_filters.TimeSynchronizer(
            [self.sub_, self.sub_depth_], queue_size=10)
        sync.registerCallback(self._apply)

    def unsubscribe(self):
        self.sub_.sub.unregister()
        self.sub_depth_.sub.unregister()

    def _apply(self, img_msg, depth_msg):
        # validation
        if depth_msg.encoding != '16UC1':
            jsk_logwarn('Unsupported depth image encoding: {0}'
                        .format(depth_msg.encoding))
            return
        if not (img_msg.height == depth_msg.height and
                img_msg.width == depth_msg.width):
            return
        # split fg/bg and get each mask
        bridge = cv_bridge.CvBridge()
        depth = bridge.imgmsg_to_cv2(depth_msg)
        fg_mask, bg_mask = split_fore_background(depth)
        # publish cropped
        img = bridge.imgmsg_to_cv2(img_msg, desired_encoding='bgr8')
        fg = img.copy()
        fg[~fg_mask] = 0
        bg = img.copy()
        bg[~bg_mask] = 0
        self.fg_pub_.publish(
            bridge.cv2_to_imgmsg(fg, encoding=img_msg.encoding))
        self.bg_pub_.publish(
            bridge.cv2_to_imgmsg(bg, encoding=img_msg.encoding))


if __name__ == '__main__':
    rospy.init_node('split_fore_background')
    split_fbg = SplitForeBackground()
    rospy.spin()