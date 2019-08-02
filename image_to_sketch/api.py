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

"""Interface for converting images to sketches"""

import base64
import os

from django.http import JsonResponse
from rest_framework.views import APIView

raw_image = "/home/daydao-dlcv/dlcv/cc/image_to_sketch/static/raw.png"
new_image = "/home/daydao-dlcv/dlcv/cc/image_to_sketch/static/sketch.png"


class CCImageToSketch(APIView):
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
        if not os.path.exists("/home/daydao-dlcv/dlcv/cc/image_to_sketch/static"):
            os.mkdir("/home/daydao-dlcv/dlcv/cc/image_to_sketch/static")
        try:
            if url:
                # download image
                os.system(f"sketch {url} {raw_image} {new_image}")
            else:
                ret["status_code"] = 10001
                ret["message"] = "参数获取异常"
                ret["image"] = None
                return JsonResponse(ret)
        except Exception as e:
            print(f"Error: get image error. message: {e}")
            ret["status_code"] = 10002
            ret["message"] = "图片获取异常"
            ret["image"] = None
            return JsonResponse(ret)

        try:
            with open(new_image, 'rb') as f:
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
            return JsonResponse(ret)
        return JsonResponse(ret)
