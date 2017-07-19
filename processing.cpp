# include <opencv2/opencv.hpp>
# include "opencv2/imgproc/imgproc.hpp"
# include "opencv2/highgui/highgui.hpp"
# include "opencv2/imgcodecs.hpp"
# include <stdlib.h>
# include <iostream>
# include <unistd.h>
# include <vector>
# include <cmath>
# include <csignal>

using namespace cv;
using namespace std;

vector<Point> centres;
vector <Point> anchor;
vector<double> times;

// returns slope for implementation of method of least squares
double getSlope(){
    double xSum = 0, ySum = 0;
    for(int i = 0; i<centres.size();i++){
        xSum = xSum + centres[i].x;
        ySum = ySum + centres[i].y;
    }
    double num = 0, den = 0;
    for(int i = 0; i<centres.size(); i++){

        num = num + (centres[i].x - xSum/(centres.size()+1))*(centres[i].y - ySum/(centres.size()+1));
        den = den + (centres[i].x - xSum/(centres.size()+1))*(centres[i].x - xSum/(centres.size()+1));

    }
    return num/den;

}

// returns slope for implementation of method of least squares for fitting y as a function of t
double getSlopeyoft(){
    double tSum = 0, ySum = 0;
    for(int i = 0; i<centres.size();i++){
        tSum = tSum + times[i];
        ySum = ySum + centres[i].y;
    }
    double num = 0, den = 0;
    for(int i = 0; i<centres.size(); i++){

        num = num + (times[i] - tSum/(centres.size()+1))*(centres[i].y - ySum/(centres.size()+1));
        den = den + (times[i] - tSum/(centres.size()+1))*(times[i] - tSum/(centres.size()+1));

    }
    return num/den;

}

// returns slope for implementation of method of least squares for fitting x as a function of t
double getSlopexoft(){
    double tSum = 0, xSum = 0;
    for(int i = 0; i<centres.size();i++){
        tSum = tSum + times[i];
        xSum = xSum + centres[i].x;
    }
    double num = 0, den = 0;
    for(int i = 0; i<centres.size(); i++){

        num = num + (times[i] - tSum/(centres.size()+1))*(centres[i].x - xSum/(centres.size()+1));
        den = den + (times[i] - tSum/(centres.size()+1))*(times[i] - tSum/(centres.size()+1));

    }
    return num/den;

}

//Returns y intercept  for the implementation of method of least squares
double getYInt(double slope){
    double xSum = 0, ySum = 0;

    for(int i = 0; i<centres.size();i++){
        xSum = xSum + centres[i].x;
        ySum = ySum + centres[i].y;
    }
    return (ySum/(centres.size()+1) - slope*xSum/(centres.size()+1));
}

//Returns y intercept  for the implementation of method of least squares to get y as a function of t
double getYIntoft(double slope){
    double tSum = 0, ySum = 0;

    for(int i = 0; i<centres.size();i++){
        tSum = tSum + times[i];
        ySum = ySum + centres[i].y;
    }
    return (ySum/(centres.size()+1) - slope*tSum/(centres.size()+1));
}

//Returns y intercept  for the implementation of method of least squares to get x as a function of t
double getXIntoft(double slope){
    double tSum = 0, xSum = 0;

    for(int i = 0; i<centres.size();i++){
        tSum = tSum + times[i];
        xSum = xSum + centres[i].x;
    }
    return (xSum/(centres.size()+1) - slope*tSum/(centres.size()+1));
}



int main(void){
    Mat input, output, img;
    double slope = 0, yInt = 0;

    //For timing
    clock_t start;

    //Start capturing the video
    VideoCapture cap(0);
    if(!cap.isOpened()){
        return -1;
    }

    cap>>input;

    Vec2i position;
    int loopcount = 0;

    vector<Mat> split_frame;
    vector<int> loop;
    start = clock();
    double tim;
    while(true){
        loopcount++;
        cap >> input;
        if(input.empty()){
            cout << "can not open " << endl;
            continue;
        }

        usleep(1000);




        split(input, split_frame);

        //We had used a green target. The code can be changed according to hte colour of the target
        Mat green = split_frame[1];

        //The threshold values need to be set based on lighting, colour of hte object etc
        threshold(green, img, 155, 255, THRESH_BINARY);


        vector<Vec3f> circles;

        //Blurring the image so that no extra unnecessary circles are detected because HoughCircles is pretty powerful, and hence detects even vague manifestations circles as circles
        medianBlur(img, img, 27);

        //Adjust these parameters for the size of the circle, etc
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 2, 190, 100, 50,20, 90 );

        loop.push_back(loopcount);
        for(int i = 0; i<circles.size();i++){
            Vec3i circle1 = circles[i];
            circle( input, Point(circle1[0], circle1[1]), circle1[2], Scalar(0,0,255), 3, LINE_AA);
            centres.push_back(Point(circle1[0], circle1[1]));
            times.push_back((clock() - start)/(double)CLOCKS_PER_SEC);

        }

        if(centres.size()>1){
            slope = getSlope();
            yInt = getYInt(slope);
        }

        //Draws a line showing the most probable trajectory to be taken by the ball
        //Currently it is assumed that the ball travels in a straight line
        line(input, Point(0,yInt), Point(1200, slope*1200 + yInt), Scalar(200, 200, 255),3);

        string s = "loopcount = " + to_string(loopcount);
        if(loopcount%10 == 0){
            anchor.push_back(centres[centres.size() - 1]);
        }

        if(loopcount>=90){
            double slopeyt = getSlopeyoft();
            double slopext = getSlopexoft();
            double yintt = getYIntoft(slopeyt);
            double xintt = getXIntoft(slopext);
            if(loopcount == 30){
                tim = 3*(clock() - start)/(double)CLOCKS_PER_SEC;
            }
            double y = slopeyt*tim + yintt;
            double x = slopext*tim + xintt;
            int xx = x;
            int yy = y;
            circle( input, Point(xx, yy), 10, Scalar(255,0,0), 5, LINE_AA);
            yy = 800-yy;
            xx = 1260-xx;
            //xx = 1260-xx;
            cout<<xx<<"  xx  "<<endl;
            cout<<yy<<"  yy  "<<endl;
            /**According to camera calibration measurements, 1280x800corresponds to ~ 60x40 in terms of stepper steps


            And then the camera is placed perpendicular to the ground, the amount the laser needs to move vertically to get into the field of view of the cam is ~32 steps
            */

            // The fifty is addee to get sensible chars as data is being transmtted as chars
            int xcalib = -9;
            int ycalib = -2;
            int stepx = 1.0*xx/1280*60 + 50+xcalib; //between 50 and 110
            int stepy = 1.0*yy/800*40 + 50;
            int yoffset = 30;
            stepy = stepy + yoffset + ycalib;//between 80 and 120
            char stepxc = stepx;
            char stepyc = stepy;
            string filename = "cpparduinobridge.py ";
            filename = filename + stepxc + " " + stepyc;
            cout<<"cpp stepx  " ;
            cout<<stepxc<<endl;
            cout<<"cpp stepy  ";
            cout<< stepyc;

            string command  = "python ";
            command += filename;

            //There may be some errors here when the char sent is a ';' or a '>' etc, because the terminal has special meanings for them
            system(command.c_str());
            cout<<loopcount;
            break;



            //double x_dist = anchor[2] - centres[0];
        }

        //cout<<anchor.size()<<endl;
        for(int i = 0; i< anchor.size();i++){
            circle( input, anchor[i], 5, Scalar(255,0,0), 3, LINE_AA);
            putText(input, "" + to_string(i+1) , anchor[i], FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 0));

        }

        putText(input, s , Point(50,50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar(00, 00, 250));
        imshow("hey", input);
        //usleep(10000000);
        //cout<<"yo"<<endl;
        //destroyAllWindows();
        //cout<<"hello"<<endl;
        if(waitKey(1) >= 0){
            cout<<"loopcounts :  "<<loopcount<<endl;
            //cout<<"fdffffffff"<<endl;
            //cout<<(clock() - start)/(double)CLOCKS_PER_SEC<<endl;
            break;
        }
        //cout<<"yo"<<endl;
    }
    destroyAllWindows();
    return 0;
}
