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


import os
import time

import cv2


def video_to_image(video_path, output_dir, fps):
    """ The program runs the main function interface.

    Args:
        video_path: Handle the absolute position of example1.
        output_dir: Video intercepts the saved directory location.
        fps: Set how many images example1 plays per second.
    """

    # Create a directory if the file directory does not exist.
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)

    camera = cv2.VideoCapture(video_path)

    for i in range(0, 6000):
        res, image = camera.read()

        if not res:
            break

        if i % fps == 0:
            cv2.imwrite(output_dir + str(i) + '.png', image)
            # print(output_dir + str(i) + '.png')

    camera.release()


if __name__ == '__main__':
    start = time.time()
    video_to_image('./video5.mp4', './video5/', 17)
    print(f'Done! Times: {time.time() - start:.4f} s.')

