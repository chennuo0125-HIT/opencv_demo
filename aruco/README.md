# Aruco Marker class

### prerequisite

- [OpenCV](https://github.com/opencv/opencv)

### compile

```
​```
git clone
cd aruco
mkdir build
cd build
cmake ..
make
```

### generate marker

```
../bin/generate_marker 6X6_250 1 5 10 15 20
```

"6X6_250"指的是字典名，表示生成一个拥有250个6X6 marker的字典

"1 5 10 15 20"marker的id号，需要生成几个marker就在后面写几个id

### estimate pose

```
../bin/estimate_pose 6X6_250 ../config/param.yaml ../data/test.jpg
```

"param.yaml"配置文件，包含相机内参和实际打印出来的marker的边长

"test.jpg"测试照片

### 结果

![](/home/cn/learn/opencv_demo/test.jpg)

![](/home/cn/learn/opencv_demo/result.jpg)