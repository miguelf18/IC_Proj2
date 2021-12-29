#include <iostream>
#include <cmath>
#include <opencv2/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include "bitStream.hpp"
#include "Golomb.h"

using namespace std;
using namespace cv;

// convert image to YUV 4:2:0
Mat convertToYUV420(Mat image) {
    Mat result;
    cvtColor(image, result, COLOR_BGR2YUV_I420); // The function cvtColor converts an image from one color space to another (in this case to YUV 4:2:0 format)
    return result;
}

// non-linear predictor of JPEG-LS
// c | b
// a | x
Vec3b pixelPredictor(Vec3b a, Vec3b b, Vec3b c) {
    Vec3b x;
    for(int i=0; i<3; i++) {
        if(c[i] >= max(a[i], b[i])) x[i] = min(a[i], b[i]);
        else if(c[i] <= min(a[i], b[i])) x[i] = max(a[i], b[i]);
        else x[i] = a[i] + b[i] - c[i];
    }
    return x;
}

Mat predictor(Mat image) {
    Mat result (image.rows, image.cols, CV_8U);
    Vec3b a, b, c, x;
    for(int row=0; row<image.rows; row ++) {
        for(int col=0; col<image.cols; col++) {
            if(row==0 && col!=0) a = image.at<Vec3b>(row, col-1);
            else if(row!=0 && col==0) b = image.at<Vec3b>(row-1, col);
            else if(row!=0 && col!=0) {
                a = image.at<Vec3b>(row, col-1);
                b = image.at<Vec3b>(row-1, col);
                c = image.at<Vec3b>(row-1, col-1);
            }
            x = pixelPredictor(a, b, c);
            result.at<Vec3b>(row, col) = x;
        }
    }
    return result;
}

void encoder(int m, string file_name, Mat& y_predictor, Mat& cb_predictor, Mat& cr_predictor){// file mode -> 1 for encoding; 0 for decoding

    Golomb g(m, file_name, '1');

    for (int r = 0; r < y_predictor.rows; r++){
        for (int col = 0; col < y_predictor.cols; col++){
            char aux = y_predictor.at<char>(r,col);
            int* auxi = new int[1];
            auxi[0] = (int)aux;
            g.encode(auxi);
        }
    }

    for (int r = 0; r < cb_predictor.rows; r++){
        for (int col = 0; col < cb_predictor.cols; col++){   
            char aux = cb_predictor.at<char>(r,col);
            int* auxi = new int[1];
            auxi[0] = (int)aux;
            g.encode(auxi);
        }
    }

    for (int r = 0; r < cr_predictor.rows; r++){
        for (int col = 0; col < cr_predictor.cols; col++){
            char aux = cr_predictor.at<char>(r,col);
            int* auxi = new int[1];
            auxi[0] = (int)aux;
            g.encode(auxi);
        }
    }
}

void decoder(int m, string file_name, Mat& y_predictor, Mat& cb_predictor, Mat& cr_predictor, Mat& y_decoder, Mat& cb_decoder, Mat& cr_decoder){

    Golomb g(m,file_name, '0');

    for (int r = 0; r < y_predictor.rows; r++){
        for (int col = 0; col < y_predictor.cols; col++){
            char c = g.decode();
            y_decoder.at<char>(r,col) = c;
        }
    }

    for (int r = 0; r < cb_predictor.rows; r++){
        for (int col = 0; col < cb_predictor.cols; col++){
            char c = g.decode();
            cb_decoder.at<char>(r,col) = c;
        }
    }

    for (int r = 0; r < cr_predictor.rows; r++){
        for (int col = 0; col < cr_predictor.cols; col++){
            char c = g.decode();
            cr_decoder.at<char>(r,col) = c;
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <image_file> " << endl;
        return -1;
    }

    //The function imread loads an image from the specified file and returns it.
    Mat image_file = imread(argv[1], IMREAD_COLOR); //IMREAD_COLOR -> If set, always convert image to the 3 channel BGR color image.

    if (image_file.empty()) {
        cout << "No image data" << endl;
        return -2;
    }

    imshow("input image", image_file);

    Mat image_converted = convertToYUV420(image_file);
    imshow("image converted", image_converted);

    // // Y, Cb e Cr separados
    Mat y (image_file.rows, image_file.cols, CV_8U);
    Mat cb (image_file.rows/2, image_file.cols/2, CV_8U);
    Mat cr (image_file.rows/2, image_file.cols/2, CV_8U);
    for(int row=0; row<image_file.rows; row++) {
        for(int col=0; col<image_file.cols; col++) {
            Vec3b pixel = image_file.at<Vec3b>(row, col);
            double b = pixel[2];
            double g = pixel[1];
            double r = pixel[0];
            // y.at<uchar>(row, col) = (int)(0.299*r + 0.587*g + 0.114*b);
            // if(row%2==0 && col%2==0) {
            //     cb.at<uchar>(row/2, col/2) = (int)(128 − 0.168736*r − 0.331264*g + 0.5*b);
            //     cr.at<uchar>(row/2, col/2) = (int)(128 + 0.5*r − 0.418688*g − 0.081312*b);
            // }
            r = r/255;
            g = g/255;
            b = b/255;

            int y_val = 16 + 65.481*r + 128.553*g + 24.966*b;
            if(y_val < 16) y_val = 16;
            else if(y_val > 235) y_val = 235;
            y.at<uchar>(row, col) = y_val;

            if(row%2==0 && col%2==0){ //se as colunas e as linhas forem pares
                int cb_val = 128 - 37.797*r - 74.203*g + 112.0*b;
                if(cb_val < 16) cb_val = 16;
                if(cb_val > 240) cb_val = 240;
                cb.at<uchar>(row/2, col/2) = cb_val;

                int cr_val = 128 + 112.0*r - 93.786*g - 18.214*b;
                if(cr_val < 16) cr_val = 16;
                if(cr_val > 240) cr_val = 240;
                cr.at<uchar>(row/2, col/2) = cr_val;
            }
        }
    }

    imshow("y",y);
    imshow("cb",cb);
    imshow("cr",cr);

    // Mat y_predictor (image_file.rows, image_file.cols, CV_8U);
    // predictor(y, y_predictor);
    // Mat cb_predictor (image_file.rows/2, image_file.cols/2, CV_8U);
    // predictor(cb, cb_predictor);
    // Mat cr_predictor (image_file.rows/2, image_file.cols/2, CV_8U);
    // predictor(cr, cr_predictor);
    Mat y_predictor = predictor(y);
    Mat cb_predictor = predictor(cb);
    Mat cr_predictor = predictor(cr);

    encoder(5, argv[1], y_predictor, cb_predictor, cr_predictor);

    Mat y_decoder (image_file.rows, image_file.cols, CV_8U);
    Mat cb_decoder (image_file.rows/2, image_file.cols/2, CV_8U);
    Mat cr_decoder (image_file.rows/2, image_file.cols/2, CV_8U);
    decoder(5, argv[1], y_predictor, cb_predictor, cr_predictor, y_decoder, cb_decoder, cr_decoder);

    waitKey();

    return 0;
}