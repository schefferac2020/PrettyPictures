#ifndef GENERATE_FRACTAL_H
#define GENERATE_FRACTAL_H

#include <iostream>
#include <vector>
#include <math.h>
#include <getopt.h>
#include <exception>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

struct Point {
    double x, y;
};


class Variation {
    public:
        virtual Point operator () (double x_, double y_) const = 0;
};

class MidpointFunctor : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        MidpointFunctor (double x_in, double y_in) : x(x_in), y(y_in) { } 
        
        Point operator () (double x_, double y_) const {
            return {(x_ + x) / 2, (y_+ y)/2};
        }
};

class Identity : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        Identity () { } 
        
        Point operator () (double x_, double y_) const {
            return {x_, y_};
        }
};

class Sinusoidal : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        Sinusoidal () { } 
        
        Point operator () (double x_, double y_) const {
            return {sin(x_), sin(y_)};
        }
};

class Spherical : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        Spherical () { }
        
        Point operator () (double x_, double y_) const {
            double r = sqrt(x_*x_ + y_*y_);
            //cout << r << endl;
            return { (1/(r * r)) * x_, (1/(r * r)) * y_};
        }
};

class Swirl : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        Swirl () { } 
        
        Point operator () (double x_, double y_) const {
            double r = sqrt(x_*x_ + y_*y_);
            return {x_ * sin(r*r) - y_ * cos(r*r), x_ * cos(r*r) + y_ * sin(r*r)};
        }
};

class V0 : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        V0 () { } 
        
        Point operator () (double x_, double y_) const {
            double r = sqrt(x_*x_ + y_*y_);
            double new_x = x_ * sin(r*r) - y_ * cos(r*r);
            double new_y = x_ * cos(r*r) + y_ * sin(r * r);
            return {new_x, new_y};
        }
};

class V1 : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        V1 () { } 
        
        Point operator () (double x_, double y_) const {
            double new_x = sin(x) / cos(y);
            double new_y = tan(y);
            return {new_x, new_y};
        }
};

class V2 : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        V2 () { } 
        
        Point operator () (double x_, double y_) const {
            double r = sqrt(x_*x_ + y_*y_);
            double new_x = (1/(r * r)) * x_;
            double new_y = (1/(r * r)) * y_;
            return {new_x, new_y};
        }
};

class V3 : public Variation {
    private:
        double x = 0;
        double y = 0;
    public:
        V3 () { } 
        
        Point operator () (double x_, double y_) const {
            double r = sqrt(x_*x_ + y_*y_);
            double theta = atan(x_/y_);

            double new_x = (1/r) * cos(theta) + sin(r);
            double new_y = (1/r) * sin(theta) - cos(r);
            return {new_x, new_y};
        }
};








class GenerateFractal {
    
    public:
        //Constructor
        GenerateFractal(int argc, char **argv) {
            this->height = 1000;
            this->width = 1000;
            get_arg_info(argc, argv);

            image = cv::Mat(height, width, CV_8UC1, cv::Scalar::all(0));
            drawFractal(100000000);
        }

    private:
        void drawFractal(int iterations = 8000){

            
            Variation *m1 = new MidpointFunctor(-1.0, 1.0);
            Variation *m2 = new MidpointFunctor(1.0, 0.0);
            Variation *m3 = new MidpointFunctor(-1.0, -1.0);
            Variation *iden = new Identity();
            Variation *sinusoidal = new Sinusoidal();
            Variation *spherical = new Spherical();

            Variation *v0 = new V0();
            Variation *v1 = new V1();
            Variation *v2 = new V2();
            Variation *v3 = new V3();
            //Variation *swirl = new Swirl();

            //vector<Variation*> functions = {/*m1, m2, m3,*/ iden, sinusoidal, spherical};
            vector<Variation*> functions = {m1, spherical, v0, v1, v2, v3};

            Point last = {0.5, 0.5};
            for (int i = 0; i < iterations; ++i){
                int rnd = rand()%functions.size(); 
                Variation* func = functions[rnd];
                
                Point newPoint = (*func)(0.5 *last.x + 0.4 * last.y + 0.1, 0.32*last.x + 0.7*last.y + 0.23);
                //cout << last.x << " " << last.y << endl;
                //cout << "(" << last.x << ", " << last.y << endl;
                if (place_point(last.x, last.y)) {
                    last = newPoint;
                }
                last = newPoint;
            }

            cv::imshow("Fractal", image);
            cv::waitKey();
        }

        bool place_point(double x, double y){
            // x++;
            // y++;

            // double scale_factor = (height * 1.0) / 2.0;



            int real_x = (int)(x * 350) + 200;
            int real_y = (int)(y * 350) + 200;
            if ((real_x < width && real_y < height && real_x > 0 && real_y > 0)){
                image.at<uchar>(real_x, real_y) = 255;
                return true;
            }else {
                return false;
            }
        }



        //----------------------------------------------------------------//
        //---------------- Processessing and Reading Data ----------------//
        //----------------------------------------------------------------//
        void print_help(){
            cout << "Bruh IDK what is going on lol why are you asking me?";
        }

        //EFFECTS: Sets the is_stack and output_type class 
        //level variables
        void get_arg_info(int argc, char **argv){
            int choice;
            int option_index = 0;
            option long_options[] = {
                { "help",    no_argument,        nullptr, 'h'  },
                { nullptr,   0,                  nullptr, '\0' }
            };

            while ((choice = getopt_long(argc, argv, "h", long_options, &option_index)) != -1) {
                switch (choice){
                    case 'h':
                        print_help();
                        exit(0);

                    default:
                        throw "Unknown option";
                }
            } // while

        }

        //--------------------------------------------------//
        //---------------- Class Level Vars ----------------//
        //--------------------------------------------------//
        int height, width;
        cv::Mat image;
};

#endif // GENERATE_FRACTAL_H