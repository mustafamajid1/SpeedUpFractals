//Timing the speedup gained through parallelization of a fractal generator using OpenMP
//Built from the tutorial of the linear version: https://medium.com/farouk-ounanes-home-on-the-internet/mandelbrot-set-in-c-from-scratch-c7ad6a1bf2d9
#include <QCoreApplication>
#include<iostream>
#include<fstream>
#include<complex>
#include<cmath>
#include<omp.h>
#include<QTime>
using namespace std;
const int width = 3000;
const int height = 3000;
int avgtime = 0;

int* calcValues();
int colorF(int &x, int &y);

int main ()  {
    int *arr;
    for(int thr = 1; thr <= 24;thr++){ //12 cores on the machine this was tested this on (24 threads with hyper-threading)
	avgtime=0;
        omp_set_num_threads(thr);
	for(int tries=1; tries<=3;tries++){
    	 arr = calcValues();
		}
	cout << avgtime/3.0 << endl; //prints out time taken to calculate color values for total number of threads thr=1, thr=2... thr=24
    }
    ofstream fractal ("fractal.ppm");//image generated
    if (fractal.is_open ()) {

        fractal << "P3" << endl << width << " " << height << " 255" << endl;
        for (int i = 0; i < width; i++) {
             for (int j = 0; j < height; j++)  {
                  int val = arr[i*height+j];
                  int val2 = 0;
                  if (val != 0)
                  {
                        val2 = (60%val); //changing the color a bit
                  }
                  fractal << val << ' ' << val2 << ' ' << val << endl;
             }
        }
        fractal.close();
    }
    else
        cout << "FILE NOT FOUND!";
    return 0;
}

int colorF (int &x, int &y)  {
complex<float> pixel((float)x/width-1.5, (float)y/height-0.5); //translation applied
complex<float> val(0, 0);
    unsigned int maxTries = 0;
    while (abs (val) < 2 && maxTries <= 34) {
            val = tan(val*val) + pixel; //strange blobs fractal
           maxTries++;
    }
    if (maxTries < 34)
    return 255*maxTries/13; //color val
    else return 0;
}

int* calcValues()	{
    int *array2D = (int *)malloc(width * height * sizeof(int));

    QTime exect;
     exect.start();
    #pragma omp parallel for collapse(2) //OpenMP parallelization
     for (int i = 0; i < width; i++)
     {
         for (int j = 0; j < height; j++)
             array2D[i*height+j] = colorF(i,j);
     }
avgtime += exect.elapsed();
//cout << exect.elapsed() << endl;//time taken to calculate all pixel colors for the given dimensions
return array2D;
}


