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
from django.shortcuts import render
from django.http import JsonResponse
from rest_framework.views import APIView
import urllib.request
import time


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

    def get(self, request):
        raw_name = "raw.png"
        img_name = "sketch.png"
        return render(request, "rgb_to_sketch.html", {"message": "success", "img_path": img_name, "raw_img": raw_name})
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
        new_image = "static/sketch.png"
        try:
            if url:
                # # print(url)
                # # download image
                # os.system(f"its {url}")
                # time.sleep(2)
                # download image
                image = urllib.request.urlopen(url)
                with open(raw_sketch, "wb") as v:
                    v.write(image.read())
                os.system(f"its")
                time.sleep(1)
            else:
                ret["status_code"] = 10001
                ret["message"] = "参数获取异常"
                ret["image"] = None
                # return JsonResponse(ret)
                return render(request, "rgb_to_sketch.html", {"message": "参数获取异常"})
        except Exception as e:
            ret["status_code"] = 10002
            ret["message"] = "图片获取异常"
            ret["image"] = None
            return render(request, "rgb_to_sketch.html", {"message": "图片获取异常"})

        try:
            if os.path.exists(new_sketch):
                with open(new_sketch, 'rb') as f:
                    image = base64.b64encode(f.read()).decode()
                    image = {'name': "sketch.png", "data": image}
                    ret["status_code"] = 20000
                    ret["message"] = 'success'
                    ret["image"] = image
        except Exception as e:
            print("Error: image coding error. message: ",e)
            ret["status_code"] = 10003
            ret["message"] = "图片编码异常"
            ret["image"] = None
            return render(request, "rgb_to_sketch.html", {"message": "图片编码异常"})
        raw_name ="raw_sketch.png"
        img_name="sketch.png"
        return render(request, "rgb_to_sketch.html", {"message": "success", "img_path": img_name, "raw_img": raw_name})


class VideoCover(APIView):
    """ Extract video beautiful cover.
    """
    def get(self,request):
        img_name = "smile.png"
        return render(request, "get_cover.html", {"msg":"success", 'cover': img_name})
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
        # smile_path = "/tmp/dlcv/static/smile.png"
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
                return render(request, "get_cover.html", {"msg": "参数获取异常", 'cover': None})
        except Exception as e:
            print(f"Error: get image error. message: {e}")
            ret["status_code"] = 10002
            ret["message"] = "图片获取异常"
            ret["image"] = None
            return render(request, "get_cover.html", {"msg": "图片获取异常", 'cover': None})

        try:
            with open(smile_path, 'rb') as f:
                image = base64.b64encode(f.read()).decode()
                image = {'name': "sketch.png", "data": image}
                ret["status_code"] = 20000
                ret["message"] = 'success'
                ret["image"] = image
        except Exception as e:
            print(f"Error: image coding error. message: {e}")
            ret["status_code"] = 10003
            ret["message"] = "图片编码异常"
            ret["image"] = None
            return render(request, "get_cover.html", {"msg": "图片编码异常", 'cover': None})
        img_name="smile.png"
        return render(request, "get_cover.html", {"msg":"success", 'cover': img_name})


class OCR(APIView):
    """ Photo style changes to sketch style.
    """
    def get(self,request):
        img_name = "smile.png"
        return render(request, "card_ocr.html", {"msg": "failed", 'adjusted_image': None,
                                                         "face_image": None, "text_content": None,
                                                         "origin_image": None, "direction": None})
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
        url = request.POST.get("url")
        file = request.FILES.get("file")

        # with open(raw_ocr, "wb") as f:
        #     for chunk in file.chunks(chunk_size=1024):
        #         f.write(chunk)

        if url:
            # download image
            os.system(f"ocr --url {url}")
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
                with open(text_path,'r',encoding='utf-8') as f:
                    text_content = f.read()

                ret = {"status_code": 20000,
                       "message": "提取成功！",
                       "raw_img": raw_image,
                       "new_img": correct_image,
                       "face": face,
                       # "text": text_path,
                       "text": text_content,
                       }
                if text_content:
                    ret["direction"] = "正面"
                    direction = ret["direction"]
                else:
                    ret["direction"] = "反面"
                    direction = ret["direction"]

                adjusted_image = "correct.png"
                face_image = "face.png"
                origin_image = "raw_ocr.png"

                # return JsonResponse(ret)
                return render(request, "card_ocr.html", {"msg": "success", 'adjusted_image': adjusted_image,
                                                         "face_image": face_image, "text_content": text_content,
                                                         "origin_image": origin_image, "direction": direction})
            else:
                ret = {"status_code": 10002,
                       "message": "图片保存异常",
                       "raw_img": None,
                       "correct_img": None,
                       "face": None,
                       "text": None}
                return JsonResponse(ret)
        elif file:
            destination = open(raw_ocr, 'wb+')
            for chunk in file.chunks():
                destination.write(chunk)
            destination.close()
            os.system(f"ocr --file {file}")
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
                with open(text_path,'r',encoding='utf-8') as f:
                    text_content = f.read()
                ret = {"status_code": 20000,
                       "message": "提取成功！",
                       "raw_img": raw_image,
                       "new_img": correct_image,
                       "face": face,
                       # "text": text_path}
                       "text": text_content}
                if text_content:
                    ret["direction"] = "正面"
                    direction = ret["direction"]
                else:
                    ret["direction"] = "反面"
                    direction = ret["direction"]
                # return JsonResponse(ret)
                adjusted_image = "correct.png"
                face_image = "face.png"
                origin_image = "raw_ocr.png"

                return render(request, "card_ocr.html", {"msg": "success", 'adjusted_image': adjusted_image,
                                                         "face_image": face_image, "text_content": text_content,
                                                         "origin_image": origin_image,"direction":direction})

            else:
                ret = {"status_code": 10002,
                       "message": "图片保存异常",
                       "raw_img": None,
                       "correct_img": None,
                       "face": None,
                       "text": None}
                # return JsonResponse(ret)
                return render(request, "card_ocr.html", {"msg": "failed", 'adjusted_image': None,
                                                         "face_image": None, "text_content": None,
                                                         "origin_image": None, "direction": None})

        else:
            ret = {"status_code": 10001,
                   "message": "参数获取异常",
                   "raw_img": None,
                   "correct_img": None,
                   "face": None,
                   "text": None}
            # return JsonResponse(ret)
            return render(request, "card_ocr.html", {"msg": "failed", 'adjusted_image': None,
                                                     "face_image": None, "text_content": None,
                                                     "origin_image": None,"direction":None})

# return render(request, "card_ocr.html", {"msg": "success", 'adjusted_image': adjusted_image,
#                                                      "face_image": face_image, "text_content": text_content,
#                                                      "origin_image": origin_image})
#         else:
#             return render(request, "card_ocr.html", {"msg": "failed", 'adjusted_image': None,
#                                                      "face_image": None, "text_content": None, "origin_image": None})
