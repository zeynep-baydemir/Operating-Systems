#include<iostream>
#include <stdio.h>
#include<vector>
#include<cstdlib>
#include<algorithm>
#include<cmath>
#include <pthread.h>
#include <fstream>
#include <time.h>
#include <iomanip>




using namespace std;
vector <int> numbers;
int N;
int minimum;
int maximum;
int range;
int mode;
double median;
int sum;
double mean;
double harmean;
double deviation;
double iqr;


void findMin(void* params){
    int min = numbers[0];
    minimum = min;
}

void findMax(void*params){
    int max = numbers[N-1];
    maximum = max;
}
void findRange(void *params){
    int ran = numbers[N-1] - numbers[0];
    range = ran;
}
void findMode(void *params){
    int m = numbers[0];
    int count = 1;
    int modeCount = 1;
    for (int i =1; i<N; i++){
        if (numbers[i]== numbers[i-1]){
            count++;
        }
        else{
            if(count>modeCount){
                modeCount = count;
                m = numbers[i-1];
            }
            count = 1;
        }
    }
    if(count > modeCount) {
        modeCount = count;
        m = numbers[N-1];
    }
    mode = m;
}

//  MEDIAN
void findMedian(void* params){
    double med;
    if (N % 2==0){
        int index = N/2;
        int total = numbers[index]+ numbers[index-1];
        med = (double)total/2;
    }
    else{
        int index = (N-1)/2;
        med = numbers[index];
    }
    median = med;
}

void findSum(void* params){
    int summ=0;
    for(int i=0; i<N;i++) {
        summ += numbers[i];
    
    }
    sum = summ;
}


//  ARITHMETIC MEAN   

void findArMean(void* params){
    int summ=0;
    for(int i=0; i<N;i++) {
        summ += numbers[i];
    }
    double arithmeticMean = (double)summ/(double)N;
    mean = arithmeticMean;
}


//  HARMONIC MEAN
void findHarmonic(void* params){
    double harmonicSum = 0;                             
    for (int i=0; i<N;i++){
        harmonicSum = harmonicSum + ((double)1) / ((double)numbers[i]);
    }
    double harmonicMean=((double)N)/harmonicSum;
    harmean = harmonicMean;
}

//  STANDARD DEVIATION

void findDeviation(void *params){
    int summ=0;
    for(int i=0; i<N;i++) {
        summ += numbers[i];
    }
    double arithmeticMean = (double)summ/(double)N;
    double standardTotal = 0.0;
    double standardDeviation = 0.0;
    for(int i=0; i<N;i++){
        standardTotal += pow(((double)numbers[i]-arithmeticMean),2);
    }
    double standardD = standardTotal/(double)(N-1);
    deviation = sqrt(standardD);
}

void findIqr(void *params){
        double lowerQ;
        double upperQ;
        if (N % 2==0){
            if( N%4 ==0){
                int medIndex = N/2-1;
                int index = N/4;
                lowerQ = (double)(numbers[index-1] + numbers[index])/2;
                upperQ = (double)(numbers[N-1-index] + numbers[N-index])/2;
            }
            else{
                int medIndex = N/2-1;
                int index = medIndex/2;
                lowerQ = (double)(numbers[index]);
                upperQ = (double)(numbers[N-1-index]);
            }
        }    
        else{ 
            int medIndex = (N-1)/2;
            if( medIndex%2 ==0 ){
                int index = medIndex/2;
                lowerQ = (double)(numbers[index-1] + numbers[index])/2;
                upperQ = (double)(numbers[N-1-index] + numbers[N-index])/2;
            }
            else{
                int index = (medIndex-1)/2;
                lowerQ = (double)(numbers[index]);
                upperQ = (double)(numbers[N-1-index]);
            }
        }
    iqr = upperQ-lowerQ;

    }

void* thread_1(void* params){
    findMin(params);
    findMax(params);
    pthread_exit(0);
}
void* thread_2(void* params){
    findRange(params);
    findMode(params);
    pthread_exit(0);
}
void* thread_3(void* params){
    findMedian(params);
    findSum(params);
    pthread_exit(0);
}
void* thread_4(void* params){
    findArMean(params);
    findHarmonic(params);
    pthread_exit(0);
}
void* thread_5(void* params){
    findDeviation(params);
    findIqr(params);
    pthread_exit(0);
}

int main(int argc, char *argv[]){
    string str = argv[1];
    N=stoi(str);


    srand((unsigned) time(NULL)); 
    for(int i=0; i<N; i++){        
        int random = 1000 + (rand() % 9001);
        numbers.push_back(random);
    }
    sort(numbers.begin(), numbers.end());

    timespec ts1,ts2;
    clock_gettime(CLOCK_REALTIME, &ts1);

    pthread_t tid_1; 
    pthread_t tid_2; 
    pthread_t tid_3; 
    pthread_t tid_4;
    pthread_t tid_5;

    pthread_create(&tid_1, NULL, thread_1, NULL);
    pthread_create(&tid_2, NULL, thread_2, NULL);
    pthread_create(&tid_3, NULL, thread_3, NULL);
    pthread_create(&tid_4, NULL, thread_4, NULL);
    pthread_create(&tid_5, NULL, thread_5, NULL);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);
    pthread_join(tid_3, NULL);
    pthread_join(tid_4, NULL);
    pthread_join(tid_5, NULL);

    clock_gettime(CLOCK_REALTIME, &ts2);
    double dif = double( ts2.tv_nsec - ts1.tv_nsec );
    double time = dif/1e9;

    ofstream output ("output3.txt");
    if (output.is_open()){
        output << minimum<<endl;
        output << maximum<<endl;
        output << range<<endl;
        output << mode<<endl;
        output << fixed<<setprecision(5)<<median<<endl;
        output << sum<<endl;
        output <<fixed<<setprecision(5)<< mean<<endl;
        output << fixed<<setprecision(5)<<harmean <<endl;
        output << fixed<<setprecision(5)<<deviation<<endl;
        output << fixed<<setprecision(5)<<iqr <<endl;
        output <<fixed<<setprecision(5)<<time << endl;
        output.close();
    }
    else cout << "Unable to open file";
    return 0;

}