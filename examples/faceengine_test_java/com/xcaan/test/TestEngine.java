package com.xcaan.test;

import com.xcaan.faceengine.*;
import java.util.List;

public class TestEngine{
	public static void main(String[] args){

        String pic = "./com/xcaan/test/test.png";

        FaceEngine.init(true);

        FaceEngine.setImageResizeArgs(2, 512, 512, 80, 0.33f);

        List<FaceLocation> x = FaceEngine.detectFace(pic);
        System.out.println("xsize="+x.toString());

        for (int i=0;i<x.size();i++) {
            FaceLocation loc = x.get(i);
            System.out.println("No. "+i+" ("+loc.xmin+", "+loc.ymin+"), ("+loc.xmax+", "+loc.ymax+") conf="+loc.confidence);
        }

        List<FaceInfo> y = FaceEngine.getFaceFeature(pic);
        System.out.println("ysize="+y.size());
        for (int i=0;i<y.size();i++) {
            FaceInfo info = y.get(i);
            System.out.println("No. "+i+" feature[0]="+info.feature[0]);
        }
	}
}