/******************************
 * Author: Alperen Kantarcı   *
 * Id : 150140140             *
 *                            *
 *****************************/

 /*****************************************************
  *  IMPORTANT COMPILING NOTE:                        *
  * In order to compile the program use this command: *
  * g++ -std=c++11 150140140.cpp                      *
  *****************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <fstream>
#include <algorithm>
#include <ctime>


using namespace std;

int numberOfCalculation=0;

struct Ball{
    int x,y,z;
};

double min(double x, double y){
    if (x < y) return x;
    else       return y;
}

double ecludianDist(Ball a , Ball b){
    numberOfCalculation++;
    return sqrt( pow(a.x - b.x,2) + pow(a.y - b.y,2) + pow(a.z - b.z,2)  );
}

//This function calculates closespair in the O(n^2) but i will use only if there is 3 ball
//I also used this function to whether my solution is correct or not.
double closestPair(Ball BallList[], int n)
{
    double min = numeric_limits<double>::max();
    for (int i = 0; i < n; i++){
        for (int j = i+1; j < n; j++){
            double tmp = ecludianDist(BallList[i],BallList[j]);
            if (tmp < min)// If distance is smaller than current smallest pair
                min = tmp;// Make current value, smallest value
        }
    }
    return min;
}

double minOfPlane(double s, Ball plane[],int NumInsidePlane){
    sort(plane,plane+NumInsidePlane,[](Ball a, Ball b){ return a.y < b.y; }); //Sort according to the Y values
    double min = s;//Minimum of right and left side

    for (int i = 0; i < NumInsidePlane; i++) {
        for (int j = i+1; j < NumInsidePlane && (plane[j].y - plane[i].y) < min ; j++) {
            double tmp = ecludianDist(plane[i],plane[j]);
            if (tmp < min)
                min = tmp;
        }
    }
    return min;
}

double closestPairRec(Ball BallList[],int n){
    if (n<=3)
        return closestPair(BallList,n);

    int mid = n/2; //Find minimum with flooring
    Ball midBall = BallList[mid];

    double dl = closestPairRec(BallList, mid);
    double dr = closestPairRec(BallList + mid, n-mid);

    //We got the smallest of two sides. We can create our vertical plane by this value
    double s = min(dl, dr);
    Ball verticalPlane[n];
    int NumInsidePlane = 0;
    for(int i = 0; i < n; i++){
        if( abs(BallList[i].x - midBall.x) < s){
            verticalPlane[NumInsidePlane] = BallList[i];
            NumInsidePlane++;
        }
    }

    //Calculating minimum distance inside plane and compare with minimum of the two sides(min of dl and dr).
    return min(minOfPlane(s,verticalPlane,NumInsidePlane),s);
}

int main(int argc, char const *argv[]){
    if(argc > 2 ){
        cerr << "You have given more parameters than expected.";
        return 0;
    }
    string textfile = argv[1];
    ifstream input(textfile);
    string line;
    getline( input, line );
    int inputSize = stoi(line);

    Ball ballList[inputSize];

    for(int i=0 ; i < inputSize ;i++){
        int x,y,z;
        input >> x >> y >> z;
        ballList[i]={x,y,z};
    }

    //Sort the data according to the X since i will divide according to it.
    clock_t startTime = clock();
    sort(ballList,ballList+inputSize,[](Ball a, Ball b){ return a.x < b.x; });
    cout << "The distance is " << closestPairRec(ballList,inputSize) << endl;
    clock_t endTime = clock();
    cout << "Number of total distance calculations is " << numberOfCalculation << endl;
    cout << "Running time is " << ((float)endTime-startTime)*1000/CLOCKS_PER_SEC << " ms" << endl;

    return 0;
}
