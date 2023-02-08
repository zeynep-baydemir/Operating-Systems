#include <iostream>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include <queue>
#include <string.h>

using namespace std;

// total payments for companies
int totals[5] = {0,0,0,0,0}; 
// mutexes to prevent inconsistent situation to make payment to companies
pthread_mutex_t companies[5];
// keeping the number of customers who sent data to the machine
int dataReady[10] = {0,0,0,0,0,0,0,0,0,0};
// keeping the total number of customers which will use the machine 
int dataArray[10] = {0,0,0,0,0,0,0,0,0,0};
// checks whether all customers came
bool completed[10] = {0,0,0,0,0,0,0,0,0,0};
// machine threads
pthread_t machineThreads[10];
// mutexes to prevent inconsistent situation when pushing customer to machine
pthread_mutex_t machine[10];
// mutexes to prevent inconsistent situation when writing to output file
pthread_mutex_t out;
// output to write to a file
string comp[5] = {"Kevin", "Bob", "Stuart", "Otto", "Dave"};
ofstream output;

// keeps customers of machines
queue < struct Customer> customerToMachine[10];

// Customer struct
struct Customer {
    int customerID; 
    int sleepTime;  
    int machineNumber;
    int company; 
    int prepayment;
};

// function of customer threads 
// thread sleeps and sends its data to machine queue
void *customerFunc(void *param){ 
    Customer customer = *(Customer*) param;
    int machineIndex = customer.machineNumber-1;
    usleep(customer.sleepTime*1000);
    pthread_mutex_lock(&machine[machineIndex]);
    customerToMachine[machineIndex].push(customer);
    dataReady[machineIndex] += 1;
    pthread_mutex_unlock(&machine[machineIndex]);
    pthread_exit(0);
}

// function of machine threads
// machine makes the payment to company
void *prepayment(void *param){
    int machineIndex = *((int *) param);
    // while all customers which will come to this machine have not come yet
    while (!completed[machineIndex]){
        // while there is still customer in queue
        while (!customerToMachine[machineIndex].empty()){ 
        // locks machine queue to get payment details and prevent queue from pushing new data, then unlocks it   
        pthread_mutex_lock(&machine[machineIndex]);     
        Customer customer = customerToMachine[machineIndex].front();
        customerToMachine[machineIndex].pop();
        pthread_mutex_unlock(&machine[machineIndex]);
        // locks company and make payment
        pthread_mutex_lock(&companies[customer.company]);
        totals[customer.company] += customer.prepayment;
        pthread_mutex_unlock(&companies[customer.company]);
        // locks out mutex to prevent output file from another writing operation
        pthread_mutex_lock(&out);
        output<< "[VTM"<<machineIndex+1<<"]: " << "Customer" << customer.customerID << ","<< customer.prepayment << "TL,"<< comp[customer.company]<<endl ;
        pthread_mutex_unlock(&out);
        }
        // checks whether all customers came and no waiting customer for machine
        if ((dataArray[machineIndex] == dataReady[machineIndex]) && (customerToMachine[machineIndex].empty())) completed[machineIndex] = 1;    
    }
    pthread_exit(0);
}


// reads input file, opens output file, creates thread and writes prepayments of companies 
int main(int argc, char** argv){
    string input_file = argv[1];
    ifstream file(input_file);
    int dot = input_file.find(".");
    string o = input_file.substr(0,dot);
    string output_file = o +"_log.txt";
    string line;
    output.open(output_file);

    // reads input file
    if (file.is_open()){
        getline(file, line);
        int nofCustomers = atoi(line.c_str());
        Customer customers[nofCustomers];
        pthread_t customerThreads[nofCustomers];
        int number = 0;
        while (getline(file, line)) {
            vector<string> v;
            stringstream ss(line);
            while (ss.good()) {
                string substr;
                getline(ss, substr, ',');
                v.push_back(substr);
            }
            // adds customer to customer array
            customers[number].customerID = number + 1;
            customers[number].sleepTime = atoi(v[0].c_str());
            customers[number].machineNumber = atoi(v[1].c_str());
            dataArray[customers[number].machineNumber-1] += 1;
            string company = v[2];
            if(company.compare("Kevin") == 0){
                customers[number].company = 0;
            }
            if(company.compare("Bob") == 0){
                customers[number].company = 1;
            }
            if(company.compare("Stuart") == 0){
                customers[number].company = 2;
            }
            if(company.compare("Otto") == 0){
                customers[number].company = 3;
            }
            if(company.compare("Dave") == 0){
                customers[number].company = 4;
            }
            customers[number].prepayment = atoi(v[3].c_str());
            number++;
        }
        // this array is created because if i is sent as argument to thread, it gives wrong number to the thread
        int mach[10]= {0,1,2,3,4,5,6,7,8,9};
        // creates machine threads
        for (int i = 0; i< 10; i++){
            pthread_create(&machineThreads[i], NULL, prepayment, &mach[i]);   
        }
        // creates customer threads

        for (int i = 0; i < nofCustomers; i++){
            pthread_create(&customerThreads[i], NULL, customerFunc, &customers[i]);   
        }
        // joins customer threads

        for (int i = 0; i < nofCustomers; i++){
            pthread_join(customerThreads[i], NULL);   
        }
        // joins machine threads
        for (int i = 0; i < 10; i++){
            pthread_join(machineThreads[i], NULL);   
        }
        // locks out mutex to prevent output file from another writing operation and writes prepayments of companies 
        pthread_mutex_lock(&out);
        output << "[Main]: All payments are completed"<<endl;
        output << "[Main]: Kevin: "<< totals[0] <<endl;        
        output << "[Main]: Bob: "<< totals[1] <<endl;
        output << "[Main]: Stuart: "<<totals[2] <<endl;
        output << "[Main]: Otto: "<< totals[3] <<endl;
        output << "[Main]: Dave: "<< totals[4] <<endl;
        pthread_mutex_unlock(&out);
    }
    return 0;
}


    





