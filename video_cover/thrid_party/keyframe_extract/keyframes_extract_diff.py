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


"""
this key frame extract algorithm is based on interframe difference.
The principle is very simple
First, we load the example1 and compute the interframe difference between each frames
Then, we can choose one of these three methods to extract keyframes, which are
all based on the difference method:

1. use the difference order
    The first few frames with the largest average interframe difference
    are considered to be key frames.
2. use the difference threshold
    The frames which the average interframe difference are large than the
    threshold are considered to be key frames.
3. use local maximum
    The frames which the average interframe difference are local maximum are
    considered to be key frames.
    It should be noted that smoothing the average difference value before
    calculating the local maximum can effectively remove noise to avoid
    repeated extraction of frames of similar scenes.
After a few experiment, the third method has a better key frame extraction effect.
The original code comes from the link below, I optimized the code to reduce
unnecessary memory consumption.
"""


import os
import argparse
import time

import cv2
import numpy as np
import matplotlib.pyplot as plt
from scipy.signal import argrelextrema


parser = argparse.ArgumentParser('Extract mp4 key frame and extract smiley face feature')

parser.add_argument('--video_path', required=False, type=str, default='./video5.mp4',
                    help='Video path of the source file.')
parser.add_argument('--window_length', required=False, type=int, default=50)

args = parser.parse_args()

# Directory to store the processed frames
output_dir = os.path.splitext(args.video_path)[0]

if not os.path.exists(output_dir):
    os.makedirs(output_dir)


def smooth(x, window_len=13, window='blackman'):
    """ smooth the data using a window with requested size.

    This method is based on the convolution of a scaled window with the signal.
    The signal is prepared by introducing reflected copies of the signal
    (with the window size) in both ends so that transient parts are minimized
    in the begining and end part of the output signal.

    Args:
        x: the input signal.
        window_len: the dimension of the smoothing window.
        window: the type of window from 'flat', 'hanning', 'hamming', 'bartlett', 'blackman'.
            flat window will produce a moving average smoothing.
    Returns:
        the smoothed signal

    example:
    import numpy as np
    t = np.linspace(-2,2,0.1)
    x = np.sin(t)+np.random.randn(len(t))*0.1
    y = smooth(x)

    see also:

    numpy.hanning, numpy.hamming, numpy.bartlett, numpy.blackman, numpy.convolve
    scipy.signal.lfilter
    """
    # print(len(x), window_len)

    s = np.r_[2 * x[0] - x[window_len:1:-1],
              x, 2 * x[-1] - x[-1:-window_len:-1]]

    if window == 'flat':  # moving average
        w = np.ones(window_len, 'd')
    else:
        w = getattr(np, window)(window_len)
    y = np.convolve(w / w.sum(), s, mode='same')
    return y[window_len - 1:-window_len + 1]


class Frame:
    """class to hold information about each frame

    """

    def __init__(self, video_id, pic_diff):
        self.id = video_id
        self.different = pic_diff

    def __lt__(self, other):
        if self.id == other.id:
            return self.id < other.id
        return self.id < other.id

    def __gt__(self, other):
        return other.__lt__(self)

    def __eq__(self, other):
        return self.id == other.id and self.id == other.id

    def __ne__(self, other):
        return not self.__eq__(other)


def rel_change(a, b):
    return (b - a) / max(a, b)


def main():
    # load example1 and compute diff between frames
    cap = cv2.VideoCapture(str(args.video_path))
    prev_frame = None
    frame_diffs = []
    frames = []
    success, frame = cap.read()
    i = 0
    while success:
        luv = cv2.cvtColor(frame, cv2.COLOR_BGR2LUV)
        curr_frame = luv
        if curr_frame is not None and prev_frame is not None:
            # logic here
            diff = cv2.absdiff(curr_frame, prev_frame)
            diff_sum = np.sum(diff)
            diff_sum_mean = diff_sum / (diff.shape[0] * diff.shape[1])
            frame_diffs.append(diff_sum_mean)
            frame = Frame(i, diff_sum_mean)
            frames.append(frame)
        prev_frame = curr_frame
        i = i + 1
        success, frame = cap.read()
    cap.release()

    # compute keyframe
    keyframe_id_set = set()

    diff_array = np.array(frame_diffs)
    sm_diff_array = smooth(diff_array, args.window_length)
    frame_indexes = np.asarray(argrelextrema(sm_diff_array, np.greater))[0]
    for i in frame_indexes:
        keyframe_id_set.add(frames[i - 1].id)

    plt.figure(figsize=(40, 20))
    plt.locator_params()
    plt.stem(sm_diff_array, use_line_collection=True)

    # save all keyframes as image
    cap = cv2.VideoCapture(str(args.video_path))
    success, frame = cap.read()
    idx = 0
    while success:
        if idx in keyframe_id_set:
            name = str(idx) + ".png"
            img_save_path = os.path.join(output_dir, name)
            cv2.imwrite(img_save_path, frame)
            keyframe_id_set.remove(idx)
        idx = idx + 1
        success, frame = cap.read()
    cap.release()


if __name__ == "__main__":
    start = time.time()
    main()
    print(f'Done! Times: {time.time() - start:.4f} s.')
