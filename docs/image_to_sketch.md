# 彩图转素描

更新时间：2019-07-22

### 能力介绍

#### 接口能力

- 无损照片格式的转换：使用该应用不会导致图像的失真，噪声增加等不良影响；
- 支持多种照片类型：支持生活照、风景照、动物照等类型的风格转换。

#### 业务应用

- 快速素描：用计算机来实现手工画图；
- 信息隐藏：可以让照片上重要的信息变得不那么容易被识别。 

### 调用方式

#### 请求URL数据格式

向API服务地址使用POST发送请求，必须严格按照urls中的链接。

例如此接口，使用HTTPS POST发送：

```text
http://127.0.0.1:8000/api/cc/image_to_sketch/
```

POST中Body的参数，按照下方请求参数说明选择即可。

### 请求说明

#### 注意事项

- Base64编码：请求的图片需经过Base64编码，图片的base64编码指将图片数据编码成一串字符串，使用该字符串代替图像地址。您可以首先得到图片的二进制，然后用Base64格式编码即可。需要注意的是，图片的base64编码是不包含图片头的，如data:image/jpg;base64,
- 图片格式：现支持PNG、JPG、JPEG、BMP，不支持GIF图片。

#### 请求示例

HTTP方法：POST

请求URL： http://127.0.0.1:8000/api/cc/image_to_sketch/

#### 请求参数

| 参数 |必选|类型|说明|
|:---:|:---:|:---:|:---:|
| url |是|string|必须是互联网上未加密，且不需要使用VPN才可以访问的链接|

#### 返回说明

| 字段  |必选|类型|说明|
|:-:|:-:|:-:|:-:|
|status_code| 是|string|程序运行状态码返回|
|message|是|string|提示程序运行时候的中断错误|
|image|是|base64|生成图片的64位编码|

#### 返回示例

```json
{
    "status_code": 200,
    "message": "success",
    "image": {
        "name": "sketch.png",
        "data": "iVBORw0KGgoAAAANSUhEUgAAAzUAAAQACAAAAAD9lyigAAAgAElEQVR4AYzBW3IkSdZm172PmjmAqEs3u6U5oSpShA8UIec/i6rMCMDN9HxUcyAumRlZP9fy/3Vs2yY9z9lB5KssLCGICgnh95KwaNXYtn2MUr4SCCA/kktIuIRFvhMIICCKyEUQuYiKyEP3cT+6bk+3UfIuJAFEBflP0vM4ZgO17fsoBQICeZidpM95dhK0kGDVWEpICD8nl/Q8jqOpUmvs2yj5LiREvhJF5CHpNFYpf0Jq0eACZAG1tHyHJJ2O1ldK+jyWmXBJQhZCgLAEkHdJ9+wGLatGjapCLqHTs+ec55xUjaLP4+2YWKNKQ3BUSdI9e3ZT27ZvVQgIgjyIICCEgMiHBBBIOgFBIEAgBALhkj6P85xzZuxPT9sQUNJzMkbNt1/+9a9///p2TPdPf/+f//v//j///unmnCf70/NtzNfPX96mt0+fXob/j2PbRpE5Z4cfJWEJBFQMIfxeQgC1xraMUvkgEEB+JJcQwhIu8o1AAAFRRC6CIBdFEVnS87gfc9yeblspDwkJICrIf5I+z2N2sLZ920q"
        }
}
```

#### 错误码

若请求错误，服务器将返回的JSON文本包含以下参数：

- status_code：错误码；
- message：错误描述信息，帮助理解和解决发生的错误。
- image：None。

例如URL失效返回：
```json
{
  "status_code": 1002,
  "message": "图片获取异常",
  "image": "None"
}
```
上述问题重新获取新的URL再次请求即可。

#### 通用及业务错误码

| 错误码  | 错误信息|描述|处理建议|
|:---:|:---:|:---:|:---:|
|1001 |参数获取异常|参数异常，具体异常原因是缺少URL链接|增加有效的URL链接|
|1002 |图片获取异常|图片异常，具体异常原因是URL链接失效|修改成有效的URL链接|
|1003 |图片编码异常|编码异常，具体异常原因是未生成sketch.png文件|检查文件夹是否存在|





























