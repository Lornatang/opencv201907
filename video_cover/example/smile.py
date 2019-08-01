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
import urllib.request
import shutil

import cv2


def check_exists():
    # Create a directory if the file directory does not exist.
    if not os.path.exists(images_dir):
        os.makedirs(images_dir)


def download_url(video_url):
    video_name = 'video.mp4'
    file = urllib.request.urlopen(video_url)
    with open(video_name, "wb") as video:
        video.write(file.read())
    return video_name


def detector():
    # Face detector
    face_path = '../data/face_improved.xml'
    face_detector = cv2.CascadeClassifier(face_path)

    # Smile detector
    smile_path = '../data/smile.xml'
    smile_detector = cv2.CascadeClassifier(smile_path)

    return face_detector, smile_detector


def video_to_image():
    """ Divide video file into several consecutive frames.

    Returns:
        Multiple images in consecutive frames.
    """
    camera = cv2.VideoCapture(video_path)

    for i in range(1, 99999):
        success, image = camera.read()

        if not success:
            break

        if i % 12 == 0:
            cv2.imwrite(images_dir + '/' + str(i) + '.png', image)

    camera.release()


def image_to_video():
    """ Combine multiple consecutive images into a single video file.

    Returns:
        A visual video file.
    """
    fourcc = cv2.VideoWriter_fourcc('M', 'J', 'P', 'G')
    video_writer = cv2.VideoWriter(video_path, fourcc, 25, (640, 362))

    for i in range(0, 99999):
        # check img exist
        if os.path.exists(images_dir + str(i) + '.png'):
            image = cv2.imread(video_path + str(i) + '.png')
            video_writer.write(image)

    video_writer.release()


def detector_smile():
    """ Perform smiley face detection on the images under the key frame.

    Returns:
        If multiple smiley faces are detected, save the best one.
        If there is no smiley face, the 120th frame image is saved by default.
    """
    # In case no smiley faces are detected.
    flag = True
    # load detector
    face_detector, smile_detector = detector()

    # Choose a smile if the machine doesn't recognize it.
    alternative_smile_path = images_dir + '/' + '120.png'
    alternative_smile = cv2.imread(alternative_smile_path)

    for file in os.listdir(images_dir):
        # Determine the user's maximum smile
        smile_degree_min = 0
        # file abs path
        file_path = os.path.join(images_dir, file)
        img = cv2.imread(file_path)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # First, it detects the face and returns a rectangle that holds the
        faces = face_detector.detectMultiScale(
            gray,
            scaleFactor=1.1,
            minNeighbors=8,
            minSize=(55, 55),
            flags=cv2.CASCADE_SCALE_IMAGE
        )

        if faces is not None:
            # Draw each face and extract the area of the face
            for (x, y, w, h) in faces:
                # cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
                face_image = gray[y:y + h, x:x + w]
                # roi_color = img[y:y + h, x:x + w]

                # Smiley face detection
                smile = smile_detector.detectMultiScale(
                    face_image,
                    scaleFactor=1.16,
                    minNeighbors=35,
                    minSize=(25, 25),
                    flags=cv2.CASCADE_SCALE_IMAGE
                )

                # Frame the corners of the mouth and label the smiling face.
                # for (x2, y2, w2, h2) in smile:
                #     cv2.rectangle(roi_color, (x2, y2), (x2 + w2, y2 + h2),
                #                  (255, 0, 0), 2)
                #     cv2.putText(img, 'Smile', (x, y - 7), 3,1.2, (0, 255, 0),
                #                 2, cv2.LINE_AA)

                if smile is not None:
                    for i in smile:
                        # Gets the height that identifies the smiley face box.
                        smile_degree_max = i[3]
                        if smile_degree_max > smile_degree_min:
                            cv2.imwrite('smile.png', img)
                            smile_degree_min = smile_degree_max
                            flag = False
    if flag:
        cv2.imwrite('smile.png', alternative_smile)


def remove_temp_file():
    os.remove(video_path)
    shutil.rmtree(os.path.splitext(video_path)[0])


def main():
    print(f'source video path: `{video_path}`.')
    print(f'target images dir: `{images_dir}`.')
    start = time.time()
    # step 1: Detects which folders are needed to run the program.
    check_exists()
    # step 2: Is video to randomly extract multiple key covers.
    video_to_image()
    # step 3: Identify the best smiley faces in the keyframe file directory.
    detector_smile()
    remove_temp_file()
    print(f'Done!\nTimes: {time.time() - start:.4f} s.')


if __name__ == '__main__':
    url = 'https://vd3.bdstatic.com/mda-iidj6snc83gn3jza/sc/mda-iidj6snc83gn3jza.mp4?auth_key=1563333305-0-0-20ad1eaa6cd1846f6ef9f56f9b2d7f45&bcevod_channel=searchbox_feed&pd=bjh&abtest=all'
    # download url file
    video_path = download_url(url)
    images_dir = './video'
    # output_dir = './smile'

    main()
