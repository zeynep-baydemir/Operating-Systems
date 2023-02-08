#include<iostream>
#include <vector>
#include<cstdlib>
#include<algorithm>
#include <cmath>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <cmath>
#include <time.h>
#include <iomanip>


using namespace std;
vector<int> numbers;
int N;

    int findMin(){
        int min = numbers[0];
        return min;
    }

    int findMax(){
        int max = numbers[N-1];
        return max;
    }

    int findRange(int max, int min){
        int range = max - min;
        return range;
    }

    int findMode(){
        int mode = numbers[0];
        int count = 1;
        int modeCount = 1;
        for (int i =1; i<N; i++){
            if (numbers[i]== numbers[i-1]){
                count++;
            }
            else{
                if(count>modeCount){
                    modeCount = count;
                    mode = numbers[i-1];
                }
                count = 1;
            }
        }
        if(count > modeCount) {
            modeCount = count;
            mode = numbers[N-1];
        }
        return mode;

    }

    double findMedian(){
        double median;
        if (N % 2==0){
            int index = N/2;
            int total = numbers[index]+ numbers[index-1];
            median = (double)total/2;
        }
        else{
            int index = (N-1)/2;
            median = numbers[index];
        }
        return median;
    }

    int findSum(){
        int sum=0;
        for(int i=0; i<N;i++) {
            sum += numbers[i];
        }

        return sum;
    }
    
    //  ARITHMETIC MEAN
    double findArMean(){
        int sum=0;
        for(int i=0; i<N;i++) {
            sum += numbers[i];
        }
        double arithmeticMean = (double)sum/N;
        return arithmeticMean;
    }


    //  HARMONIC MEAN
    double findHarMean(){
        double harmonicSum = 0;                             
        for (int i=0; i<N;i++){
            harmonicSum = harmonicSum + ((double)1) / (numbers[i]);
        }
        double harmonicMean=((double)N)/harmonicSum;
        return harmonicMean;
    }

    //  STANDARD DEVIATION

    double findDeviation(){
        int sum=0;
        for(int i=0; i<N;i++) {
            sum += numbers[i];
        }
        double arithmeticMean = (double)sum/N;
        double standardTotal = 0.0;
        double standardDeviation = 0.0;
        for(int i=0; i<N;i++){
            standardTotal += pow(((double)numbers[i]-arithmeticMean),2);
        }
        double standardD = standardTotal/(double)(N-1);
        standardDeviation = sqrt(standardD);
        return standardDeviation;
    }
    
    double findIqr(){
        double iqr;
        double lowerQ;
        double upperQ;
        int lowerQind;
        int upperQind;
        if (N % 2==0){
            if( N%4 ==0){
                int medIndex = N/2-1;
                int index = N/4;
                lowerQ = (double)(numbers[index-1] + numbers[index])/2;
                upperQ = (double)(numbers[N-1-index] + numbers[N-index])/2;
                iqr = upperQ-lowerQ;
            }
            else{
                int medIndex = N/2-1;
                int index = medIndex/2;
                lowerQ = (double)(numbers[index]);
                upperQ = (double)(numbers[N-1-index]);
                iqr = upperQ-lowerQ;
            }
        }    
        else{ 
            int medIndex = (N-1)/2;
            if( medIndex%2 ==0 ){
                int index = medIndex/2;
                lowerQ = (double)(numbers[index-1] + numbers[index])/2;
                upperQ = (double)(numbers[N-1-index] + numbers[N-index])/2;
                iqr = upperQ-lowerQ;
            }
            else{
                int index = (medIndex-1)/2;
                lowerQ = (double)(numbers[index]);
                upperQ = (double)(numbers[N-1-index]);
                iqr = upperQ-lowerQ;
            }
        }
        return iqr;
    }
int main(int argc,char* argv[]){
    string str = argv[1];
    N = stoi(str); 
    // CREATING RANDOM NUMBERS
    srand((unsigned) time(NULL)); 
    for(int i=0; i<N; i++){        
        int random = 1000 + (rand() % 9001);
        numbers.push_back(random);
    }

    // SORTING NUMBERS
    sort(numbers.begin(), numbers.end());   

    timespec ts1,ts2;
    clock_gettime(CLOCK_REALTIME, &ts1);

    int min, max, range,median,mode,sum;
    double mean, harmean,deviation, iqr;
    min = findMin();
    max = findMax();
    range= findRange(max,min);
    mode = findMode();
    median = findMedian();
    sum = findSum();
    mean = findArMean();
    harmean = findHarMean();
    deviation = findDeviation();
    iqr = findIqr();
    
    clock_gettime(CLOCK_REALTIME, &ts2);
    double dif = double( ts2.tv_nsec - ts1.tv_nsec );
    double time = dif/1e9;

    ofstream output ("output1.txt");
    if (output.is_open()){
        output << min<<endl;
        output <<max<<endl;
        output <<range<<endl;
        output <<mode<<endl;
        output << fixed<<setprecision(5)<<median<<endl;
        output <<sum<<endl;
        output << fixed<<setprecision(5)<<mean<<endl;
        output << fixed<<setprecision(5)<<harmean <<endl;
        output << fixed<<setprecision(5)<<deviation<<endl;
        output << fixed<<setprecision(5)<<iqr <<endl;
        output << fixed<<setprecision(5)<< time << endl;
        output.close();
    }
    else cout << "Unable to open file";
    return 0;

}