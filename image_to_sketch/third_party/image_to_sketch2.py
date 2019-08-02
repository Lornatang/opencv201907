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

from PIL import Image
import os

# 图像组成：红绿蓝  （RGB）三原色组成    亮度（255,255,255）
image = "Annie1.jpg"
img = Image.open(image)
img_all = "素描" + image
new = Image.new("L", img.size, 255)
width, height = img.size
img = img.convert("L")
# print(img.size)
# print(img.mode) #RBG
#
# img_get = img.getpixel((0, 0))
# print(img_get) #三原色通道
#
# img_L=img.convert('L')
# print(img_L)
# img_get_L=img_L.getpixel((0,0))    #换算 得到灰度值
# print(img_get_L)

# 定义画笔的大小
Pen_size = 3
# 色差扩散器
Color_Diff = 6
for i in range(Pen_size + 1, width - Pen_size - 1):
    for j in range(Pen_size + 1, height - Pen_size - 1):
        # 原始的颜色
        originalColor = 255
        lcolor = sum([img.getpixel((i - r, j))
                      for r in range(Pen_size)]) // Pen_size
        rcolor = sum([img.getpixel((i + r, j))
                      for r in range(Pen_size)]) // Pen_size

        # 通道----颜料
        if abs(lcolor - rcolor) > Color_Diff:
            originalColor -= (255 - img.getpixel((i, j))) // 4
            new.putpixel((i, j), originalColor)

        ucolor = sum([img.getpixel((i, j - r))
                      for r in range(Pen_size)]) // Pen_size
        dcolor = sum([img.getpixel((i, j + r))
                      for r in range(Pen_size)]) // Pen_size

        # 通道----颜料
        if abs(ucolor - dcolor) > Color_Diff:
            originalColor -= (255 - img.getpixel((i, j))) // 4
            new.putpixel((i, j), originalColor)

        acolor = sum([img.getpixel((i - r, j - r))
                      for r in range(Pen_size)]) // Pen_size
        bcolor = sum([img.getpixel((i + r, j + r))
                      for r in range(Pen_size)]) // Pen_size

        # 通道----颜料
        if abs(acolor - bcolor) > Color_Diff:
            originalColor -= (255 - img.getpixel((i, j))) // 4
            new.putpixel((i, j), originalColor)

        qcolor = sum([img.getpixel((i + r, j - r))
                      for r in range(Pen_size)]) // Pen_size
        wcolor = sum([img.getpixel((i - r, j + r))
                      for r in range(Pen_size)]) // Pen_size

        # 通道----颜料
        if abs(qcolor - wcolor) > Color_Diff:
            originalColor -= (255 - img.getpixel((i, j))) // 4
            new.putpixel((i, j), originalColor)

new.save(img_all)

i = os.system(
    'mshta vbscript createobject("sapi.spvoice").speak("%s")(window.close)' %
    '您的图片转换好了')
os.system(img_all)
