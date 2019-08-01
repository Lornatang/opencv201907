# Copyright 2019 DayHR Authors. All Rights Reserved.
# Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

import cv2
import os


def main(filename, fps, frame_size, img_start, img_end):
    """ The program runs the main function interface.

    Args:
        filename: Video file name.
        fps: Set how many images example1 plays per second..
        frame_size: Size of frame in example1.
        img_start: The image is transformed into the starting target of the example1 image.
        img_end: The image is transformed into the ending target of the example1 image
    """
    fourcc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')
    video_writer = cv2.VideoWriter(filename, fourcc, fps, frame_size)

    for i in range(img_start, img_end):
        # check img exist
        if os.path.exists('./example1/' + str(i) + '.png'):
            img = cv2.imread(filename + str(i) + '.png')
            video_writer.write(img)

            print(str(i) + '.png' + ' completion!')

    video_writer.release()


if __name__ == '__main__':
    main('./result.avi', 25, (640, 362), 0, 6000)
