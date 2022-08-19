#include "faceengine.h"
#include <iostream>
#include <dirent.h>
#include <vector>
#include <chrono>
#include <pthread.h>
#include <string.h>

//#include <opencv2/opencv.hpp>

void test_get_head_count(std::string path) {
    
    std::vector<FaceLocation_t> locs = detectFace(path);
    
    std::cout << "boxes = \n" << locs.size() << std::endl;
   
    /*
    // draw
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    int num_head_count = locs.size();
    for (int i=0;i<num_head_count;i++) {
        float x, y, w, h;
        x = locs[i].xmin;
        y = locs[i].ymin;
        w = locs[i].xmax - x;
        h = locs[i].ymax - y;
        cv::Rect rect(x, y, w, h);
        cv::rectangle(image, rect, cv::Scalar(0, 0, 255), 1, cv::LINE_8,0);
    }
    cv::imwrite("./result.box.jpg", image);
    */
}

void *thread_func(void *arg) {

    char *path = (char *)arg;

    std::vector<FaceLocation_t> locs = detectFace(path);
    if(locs.size() > 0) {
        std::cout << "Detected faces: " << locs.size() << std::endl;
        for(int i=0; i<locs.size(); i++) {
            std::cout << "(" << locs[i].xmin << "," << locs[i].ymin << ") - (" << locs[i].xmax << "," << locs[i].ymax << "), conf=" << locs[i].confidence << std::endl;
        }
    }

    std::vector<FaceInfo_t> infos = getFaceFeature(path);
    if(infos.size() > 0) {
        std::cout << "feature faces: " << infos.size() << std::endl;
        for(int i=0; i<infos.size(); i++) {
            std::cout << "(" << infos[i].location.xmin << "," << infos[i].location.ymin << ") - (" << infos[i].location.xmax << "," << infos[i].location.ymax << "), conf=" << infos[i].location.confidence << std::endl;
            std::cout << "feature dim = " << infos[i].feature.size() << std::endl;
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {

    pthread_t ids[10];

    if(argc == 2)
        init_engine(true);
    else {
        if(strcmp(argv[2], "cpu") == 0)
            init_engine(false);
        else
            init_engine(true);
    }

    set_image_resize_args(2, 512, 512);

    test_get_head_count(argv[1]);

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    for(int i=0; i<10; i++)
        pthread_create(&(ids[i]), NULL, thread_func, argv[1]);

    char *s = NULL;
    for(int i=0; i<10; i++)
        pthread_join(ids[i], (void **)&s);

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::chrono::duration<double> time_used = std::chrono::duration_cast<std::chrono::duration<double>>(end-start);
    double time_t = time_used.count();
    std::cout << "time use : " << time_t << " sec" << std::endl;
    

    std::cout << "Finished" << std::endl;

    return 0;
}
           
