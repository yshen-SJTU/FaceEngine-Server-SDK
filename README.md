# FaceEngine Server SDK
A group of libraries for face detection and recognition that provide following features:
- Support for CPU or GPU server (Ubuntu server 20.04 or later)
- Simple C and Java APIs
- Easy to use and integrate

## More About FaceEngine Server SDK
Face detection and recognition are increasingly being used to different applications. Some of cloud operators provide face-recognition SaaS, and applications rent the service and submit their images to clouds for face features. FaceEngine Server SDK makes applicatons be capable to set up (develop and deploy) a face-recognition server easily and keep all data (images) locally. We also share our face engine libraries for terminal device. Please refer to FaceEngine Android.



## Deployment
The SDK is for platform of Ubuntu server 20.04 or later. There are two types of deployments for using FaceEngine Server SDK. The difference is that, for both CPU and GPU servers, the "DYN" deployment needs libtorch while the "STATIC" deployment does not.

| Deployment type | Server | FaceEngine library file               | Volume  | Libtorch     | 
|:----------------|:------:|:--------------------------------------|:--------|:-------------|
| DNY             | CPU    | lib/dyn/CPU/libxcaan_faceengine.so    | 89 MB   | required     |
|                 | GPU    | lib/dyn/GPU/libxcaan_faceengine.so    | 101 MB  | required     | 
| STATIC          | CPU    | lib/static/CPU/libxcaan_faceengine.so | 770 MB  | not required |
|                 | GPU    | lib/static/GPU/libxcaan_faceengine.so | 915 MB  | not required |

<font color="#dd0000"> ** We suggest that you download the above library files from <a href="https://pan.baidu.com/s/1dc756nJc8nyASfv-JZHdqg">Baidu cloud (code: sjtu)</a> since they've exceeded the Git LFS storage quota and may be incorrect. **</font>



### Steps of the DYN deployment

**1. Install CUDA toolkit** 

For CPU server, skip this step.

For GPU server, the prerequisite is to install CUDA toolkit. We recommend version 11.3.1 (with GPU driver version 465.19.01) since FaceEngine was developed and tested using this version. You can download CUDA toolkit from <a href="https://developer.download.nvidia.com/compute/cuda/11.3.1/local_installers/cuda_11.3.1_465.19.01_linux.run">here</a>. 


**2. Install libtorch**

FaceEngine was developed under libtorch 1.11.0. For GPU server, get the <a href="https://download.pytorch.org/libtorch/cu113/libtorch-cxx11-abi-shared-with-deps-1.12.1%2Bcu113.zip">libtorch CUDA version</a>, or for CPU server, get the <a href="https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.11.0%2Bcpu.zip">libtorch CPU version</a>.

Unzip the downloaded file to your home directory. This will deploy libtorch to /home/your_account/libtorch.

**3. Deploy FaceEngine**

Copy the FaceEngine CPU or GPU library file (according to your server type) to /usr/lib64 or any other directory you like.

**4. Load libraries**

```bash
sudo vi /etc/ld.so.conf.d/faceengine.conf 
```

Add following two lines to faceengine.conf,
```bash
/usr/lib64     # the faceengine library dir
/home/your_account/libtorch/lib 
```

and run
```bash
sudo ldconfig
```

**5. The server should be ready. Test it!**
```bash
cd examples/faceengine_test_c
./build/main test.png cpu   # or gpu to run using GPU 
```

If you see the output like the following:
```bash
boxes = 
2
Detected faces: 2
(308.736,108.892) - (358.67,177.344), conf=0.999736
(235.007,144.82) - (282.428,204.207), conf=0.999096
...
```

then congratulations! You can use the FaceEngine server SDK to develop your applications.


### Steps of the STATIC deployment
The static deployment does not need libtorch. Suppose you have installed CUDA toolkit for your GPU server. Then what you need to do are copying and loading the library.

**1. Deploy FaceEngine**

Copy the FaceEngine CPU or GPU library file (according to your server type) to /usr/lib64 or any other directory you like.

**2. Load libraries**

```bash
sudo vi /etc/ld.so.conf.d/faceengine.conf 
```

Add following line to faceengine.conf,
```bash
/usr/lib64     # the faceengine library dir
```

and run
```bash
sudo ldconfig
```

**3. The server should be ready. Test it!**
```bash
cd examples/faceengine_test_c
./build/main test.png cpu   # or gpu to run using GPU 
```

If you see the output like the following:
```bash
boxes = 
2
Detected faces: 2
(308.736,108.892) - (358.67,177.344), conf=0.999736
(235.007,144.82) - (282.428,204.207), conf=0.999096
...
```

then congratulations! You can start to develop your applications.



## Using FaceEngine server SDK

FaceEngine server SDK provides both C and Java APIs.

### Develop applications using C APIs

A simple C example:

```C
#include <vector>
#include "faceengine.h"

using namespace std;
int main(int argc, char* argv[]) {

    init_engine(false);

    vector<FaceLocation_t> locs = detectFace("./test.png");
	
    if(locs.size() > 0) {
        cout << "Detected faces: " << locs.size() << endl;
        for(int i=0; i<locs.size(); i++) {
            cout << "(" << locs[i].xmin << "," << locs[i].ymin << ")";
            cout << "~ (" << locs[i].xmax << "," << locs[i].ymax << "), conf=" << locs[i].confidence << endl;
        }
    }

    vector<FaceInfo_t> infos = getFaceFeature("./test.png");
    if(infos.size() > 0) {
        cout << "feature faces: " << infos.size() << endl;
        for(int i=0; i<infos.size(); i++) {
           cout << "(" << infos[i].location.xmin << "," << infos[i].location.ymin << ")";
           cout << "~ (" << infos[i].location.xmax << "," << infos[i].location.ymax << "), conf=" << infos[i].location.confidence << endl;
           cout << "feature dim = " << infos[i].feature.size() << endl;
        }
    }

    cout << "Finished" << endl;
}
```

The example of faceengine_test_c including CMakeLists.txt can be the start of your application development.


#### The struct FaceLocation_t
	
It represents the pixel bounding box of a detected face in an image and its confidence.

```C
typedef struct {
    float xmin;
    float ymin;
    float xmax;
    float ymax;
    float confidence;
} FaceLocation_t;

```


#### The struct FaceInfo_t

```C
typedef struct {
    FaceLocation_t location;
    std::vector<float> feature;
} FaceInfo_t;

```

The feature field in FaceInfo_t is a vector containing a detected face's feature. In this SDK the feature is 128-dimensional. Cosine distance can be used to find similar faces.

```C
#define DIMENSION 128

float getSimilarity(std::vector<float> feature1, std::vector<float> feature2) {
    float sim = 0;
    for (int ind=0; ind<DIMENSION; ind++)
        sim += feature1[ind]*feature2[ind];  // Note that face feature has be normalized in the SDK.

    return 0.5 + 0.5 * sim; // Map similarity from [-1, 1] to [0, 1]
}
```

<p id="CAPI"></p>

#### The C APIs of FaceEngine server SDK

1. Init function
```C
void init_engine(bool is_gpu);
```

This function has to be invoked once before any other SDK functions are invoked.

Parameters
 - is_gpu - true if the application wants to use GPU for face computation.
 
2. Face detection
```C
std::vector<FaceLocation_t> detectFace(std::string path);
std::vector<FaceLocation_t> detectFace(char *path);
```

Parameters
 - path - file path of the image to be detected.

Return
 - vector of locations of detected faces in the image.

3. Get face feature
```C
std::vector<FaceInfo_t> getFaceFeature(std::string path);
std::vector<FaceInfo_t> getFaceFeature(char *path);
```

Parameters
 - path - file path of the image to be detected.

Return
 - vector of face information including face features of detected faces in the image.

4. Resize image
```C
void set_image_resize_args(int resize_flag, int resizeto_width, int resizeto_height, int min_size, float min_ratio);
```

Images can be resized to smaller size to reduce computation. The invocation of this API has global effect that means all the following images will be resized according to its parameters until the next invocation of the API.

Parameters
 - resize_flag - 
	- 0   - images will not be resized;
    - < 0 - images will be resized and will not keep image's width/height ratio; 
    - \> 0 - images will be resized and will keep image's width/height ratio. Note that the ratios of image_width/resizeto_width and image_height/resizeto_height may have different values. resize_flag=1 indicates that images are resized using the bigger ratio.

 - resizeto_width - width that images will be resized to; Note that if resizeto_width is less than min_size, min_size will be take as resizeto_width.

 - resizeto_height - height that images will be resized to; Note that if resizeto_height is less than min_size, min_size will be take as resizeto_height.

 - min_size - the minimal width/height value that images will be resized to. This is a threshold size to avoid too small image. The default value is 80, and if min_size has a value less than 80, the SDK will use 80 instead. 

 - min_ratio - the minimal ratio that images will be resized to. For a high resolution image, the faces in the final resized image may be very small even if we resize it to the size of (resizeto_width, resizeto_height). This is a threshold ratio to avoid too small faces in a resized image. The default value is 0.3.


 
### Develop applications using Java APIs

A simple Java example:

```Java
package com.xcaan.test;

import com.xcaan.faceengine.*;
import java.util.List;

public class TestEngine{
	public static void main(String[] args){

        String pic = "./com/xcaan/test/test.png";

        FaceEngine.init(true); // use GPU

        FaceEngine.setImageResizeArgs(2, 512, 512, 80, 0.33f);

        List<FaceLocation> x = FaceEngine.detectFace(pic);
        for (int i=0;i<x.size();i++) {
            FaceLocation loc = x.get(i);
            System.out.println("No. "+i+" ("+loc.xmin+", "+loc.ymin+"), ("+loc.xmax+", "+loc.ymax+") conf="+loc.confidence);
        }

        List<FaceInfo> y = FaceEngine.getFaceFeature(pic);
        for (int i=0;i<y.size();i++) {
            FaceInfo info = y.get(i);
            System.out.println("No. "+i+" feature[0]="+info.feature[0]);
        }
    }
}
```

#### The class FaceLocation

```Java
package com.xcaan.faceengine;

public class FaceLocation {
    public float xmin, ymin, xmax, ymax, confidence;
}
```

#### The class FaceInfo

```Java
package com.xcaan.faceengine;

import java.util.*;

public class FaceInfo {
    public float[] feature;
    public FaceLocation location;
}
```

#### The class FaceEngine

```Java
package com.xcaan.faceengine;

import java.util.List;

public class FaceEngine {

    static {
        System.loadLibrary("xcaan_faceengine");
    }

    public static native void init(boolean is_gpu);
    public static native void setImageResizeArgs(int flag, int width, int height, int min_size, float min_ratio);
    public static native List<FaceLocation> detectFace(String path);
    public static native List<FaceInfo> getFaceFeature(String path);

}
```

#### The Java APIs of FaceEngine

There are only four API functions in the class FaceEngine. About their parameters and returns, please refer to ["The C APIs of FaceEngine server SDK"](#CAPI).



## License

The SDK can be used for any personal or commercial purpose. The network models embedded in the SDK will be expired and upgraded by Sept. 2024. 

The SDK is distributed 'as is' and with no warranties of any kind, whether express or implied. The user must assume the entire risk of using the SDK, and the development team won't be held responsible for any problem in your using the SDK.


## The Development Team

FaceEngine is a cooperation between Shanghai Jiao Tong University and XCAAN with several researchers, engineers and students contributing to it.

Please contact <yshen@cs.sjtu.edu.cn> if you have problems about using the SDK. Any technical exchange and cooperation are welcomed.



