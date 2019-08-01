#!/usr/bin/env bash

pip install -r requirements.txt

python --video_path demo1.mp4 --images_dir demo1
python --video_path demo2.mp4 --images_dir demo2
python --video_path demo3.mp4 --images_dir demo3
python --video_path demo4.mp4 --images_dir demo4

rm -r demo1 demo2 demo3 demo4
