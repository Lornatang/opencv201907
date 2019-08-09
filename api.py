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

"""Deep learning computer vision"""

import base64
import os
import time

from django.http import JsonResponse
from rest_framework.views import APIView
import urllib.request

# rgb image to sketch image
raw_sketch     =    "/home/wwwroot/my_resume/static/images/dlcv/raw_sketch.png"
new_sketch     =    "/home/wwwroot/my_resume/static/images/dlcv/static/sketch.png"

# video cover
raw_video_name =    "/home/wwwroot/my_resume/static/images/dlcv/video.mp4"
raw_video_dir  =    "/home/wwwroot/my_resume/static/images/dlcv/video"
smile_dir      =    "/home/wwwroot/my_resume/static/images/dlcv/static"
smile_path     =    "/home/wwwroot/my_resume/static/images/dlcv/static/smile.png"

# ocr
raw_ocr        =    "/home/wwwroot/my_resume/static/images/dlcv/raw_ocr.png"
correct_ocr    =    "/home/wwwroot/my_resume/static/images/dlcv/correct.png"
face_path      =    "/home/wwwroot/my_resume/static/images/dlcv/face.png"
text_path      =    "/home/wwwroot/my_resume/static/images/dlcv/text.txt"

class ImageToSketch(APIView):
    """ Photo style changes to sketch style.
    """
    @staticmethod
    def post(request):
        """ Get the image based on the base64 encoding or url address
            and do the pencil style conversion

        Args:
            request: Post request in url.
            - image_code: 64-bit encoding of images.
            - url:        The URL of the image.

        Returns:
            Base64 bit encoding of the image.

        Notes:
            Later versions will not return an image's address,
            but instead a base64-bit encoded address
        """
        ret = {"status_code": 10000, "message": None, "image": None}
        # Get the url for the image
        url = request.POST.get("url")
        flag = 0
        if url:
            # download image
            image = urllib.request.urlopen(url)
            with open(raw_sketch, "wb") as v:
                v.write(image.read())
            os.system(f"its")
            time.sleep(2)
        else:
            ret["status_code"] = 10001
            ret["message"] = "参数获取异常"
            ret["image"] = None
            return JsonResponse(ret)
        if flag == 0:
            if os.path.exists(new_sketch):
                with open(new_sketch, 'rb') as f:
                    image = base64.b64encode(f.read()).decode()
                    image = {'name': "sketch.png", "data": image}
                    ret["status_code"] = 20000
                    ret["message"] = 'success'
                    ret["image"] = image
                    return JsonResponse(ret)
            else:
                ret["status_code"] = 10002
                ret["message"] = "保存文件出错"
                ret["image"] = None
                return JsonResponse(ret)
        else:
            ret["status_code"] = 10003
            ret["message"] = "API接口调用出错"
            ret["image"] = None
            return JsonResponse(ret)


class VideoCover(APIView):
    """ Extract video beautiful cover.
    """
    @staticmethod
    def post(request):
        """ Get the image based on the base64 encoding or url address
            and do the pencil style conversion

        Args:
            request: Post request in url.
            - image_code: 64-bit encoding of images.
            - url:        The URL of the image.

        Returns:
            Base64 bit encoding of the image.

        Notes:
            Later versions will not return an image's address,
            but instead a base64-bit encoded address
        """
        ret = {"status_code": 10000, "message": None, "image": None}
        # Get the url for the image
        url = request.POST.get("url")
        try:
            if url:
                video = urllib.request.urlopen(url)
                with open(raw_video_name, "wb") as v:
                    v.write(video.read())
                # download image
                os.system(f"vc")
            else:
                ret["status_code"] = 10001
                ret["message"] = "参数获取异常"
                ret["image"] = None
                return JsonResponse(ret)
        except Exception as e:
            print(f"Error: get image error. message: {e}")
            ret["status_code"] = 10002
            ret["message"] = "视频获取异常"
            ret["image"] = None
            return JsonResponse(ret)

        try:
            with open(smile_path, 'rb') as f:
                image = base64.b64encode(f.read()).decode()
                image = {'name': "smile.png", "data": image}
                ret["status_code"] = 20000
                ret["message"] = 'success'
                ret["image"] = image
        except Exception as e:
            print(f"Error: image coding error. message: {e}")
            ret["status_code"] = 10003
            ret["message"] = "图片编码异常"
            ret["image"] = None
            return JsonResponse(ret)
        return JsonResponse(ret)


class OCR(APIView):
    """ Photo style changes to sketch style.
    """
    @staticmethod
    def post(request):
        """ Get the image based on the base64 encoding or url address
            and do the pencil style conversion

        Args:
            request: Post request in url.
            - image_code: 64-bit encoding of images.
            - url:        The URL of the image.

        Returns:
            Base64 bit encoding of the image.

        Notes:
            Later versions will not return an image's address,
            but instead a base64-bit encoded address
        """
        ret = {"status_code": 10000,
               "message": None,
               "raw_img": None,
               "correct_img": None,
               "face": None,
               "text": None}
        # Get the url for the image
        file = request.FILES.get("file")

        # with open(raw_ocr, "wb") as f:
        #     for chunk in file.chunks(chunk_size=1024):
        #         f.write(chunk)

        if file:
            with open(raw_ocr, "wb") as f:
                for chunk in file.chunks(chunk_size=1024):
                    f.write(chunk)
            # download image
            os.system(f"ocr")
            time.sleep(2)

            if os.path.exists(correct_ocr):
                with open(raw_ocr, 'rb') as r:
                    raw_image = base64.b64encode(r.read()).decode()
                    raw_image = {'name': "raw_ocr.png", "data": raw_image}
                with open(correct_ocr, 'rb') as c:
                    correct_image = base64.b64encode(c.read()).decode()
                    correct_image = {'name': "correct.png", "data": correct_image}
                with open(face_path, 'rb') as f:
                    face = base64.b64encode(f.read()).decode()
                    face = {'name': "face.png", "data": face}
                ret = {"status_code": 20000,
                       "message": "提取成功！",
                       "raw_img": raw_image,
                       "new_img": correct_image,
                       "face": face,
                       "text": text_path}
                return JsonResponse(ret)
            else:
                ret = {"status_code": 10002,
                       "message": "图片保存异常",
                       "raw_img": None,
                       "correct_img": None,
                       "face": None,
                       "text": None}
                return JsonResponse(ret)
        # elif file:
        #     destination = open(raw_ocr, 'wb+')
        #     for chunk in file.chunks():
        #         destination.write(chunk)
        #     destination.close()
        #     os.system(f"ocr --file {file}")
        #     time.sleep(2)
        #
        #     if os.path.exists(correct_ocr):
        #         with open(raw_ocr, 'rb') as r:
        #             raw_image = base64.b64encode(r.read()).decode()
        #             raw_image = {'name': "raw_ocr.png", "data": raw_image}
        #         with open(correct_ocr, 'rb') as c:
        #             correct_image = base64.b64encode(c.read()).decode()
        #             correct_image = {'name': "correct.png", "data": correct_image}
        #         with open(face_path, 'rb') as f:
        #             face = base64.b64encode(f.read()).decode()
        #             face = {'name': "face.png", "data": face}
        #         ret = {"status_code": 20000,
        #                "message": "提取成功！",
        #                "raw_img": raw_image,
        #                "new_img": correct_image,
        #                "face": face,
        #                "text": text_path}
        #         return JsonResponse(ret)
        #     else:
        #         ret = {"status_code": 10002,
        #                "message": "图片保存异常",
        #                "raw_img": None,
        #                "correct_img": None,
        #                "face": None,
        #                "text": None}
        #         return JsonResponse(ret)

        else:
            ret = {"status_code": 10001,
                   "message": "参数获取异常",
                   "raw_img": None,
                   "correct_img": None,
                   "face": None,
                   "text": None}
            return JsonResponse(ret)
