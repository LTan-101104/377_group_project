#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <queue>
#include <string>

using namespace std;

pqueue_arrival read_workload(string filename) { //TODO: recheck
  pqueue_arrival workload;
  std::ifstream file(filename);

  if (!file){
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return workload;
  }
  std::string line;
  //pass each line to the line variable, the use the issstringstream to facilitate line as an input stream for reading data
  while(std::getline(file, line)){
    std::istringstream iss(line);
    Process p;
    if (iss >> p.arrival >> p.duration){
      p.first_run = -1; //initialize first run
      p.completion = -1;
      workload.push(p);
    }
  }
  file.close();
  return workload;
}

void show_workload(pqueue_arrival workload) {
  pqueue_arrival xs = workload;
  cout << "Workload:" << endl;
  while (!xs.empty()) {
    Process p = xs.top();
    cout << '\t' << p.arrival << ' ' << p.duration << endl;
    xs.pop();
  }
}

void show_processes(list<Process> processes) {
  list<Process> xs = processes;
  cout << "Processes:" << endl;
  while (!xs.empty()) {
    Process p = xs.front();
    cout << "\tarrival=" << p.arrival << ", duration=" << p.duration
         << ", first_run=" << p.first_run << ", completion=" << p.completion
         << endl;
    xs.pop_front();
  }
}

int getMax(int a1, int a2){
  if (a1 < a2){
    return a2;
  }
  else{
    return a1;
  }
}

list<Process> fifo(pqueue_arrival workload) {
  list<Process> complete;
  int time = 0; 
  while (!workload.empty()) {
    Process p = workload.top();
    // if (cur == NULL){
    //   time = p.arrival; //initialize time to keep track of time flow, avoid edge case where first process arrival != 0
    //   cur = p;
    // }
    p.first_run = getMax(time, p.arrival); // will be time when process arrival time <= last completion time, else when it starts way latter, first_run have to be arrival_time
    p.completion = p.first_run + p.duration;
    time = p.completion;
    complete.push_back(p);
    workload.pop();
  }
  return complete;
}

list<Process> sjf(pqueue_arrival workload) {
  //by default if tie, pqueue_arrival will already prioritized the ones with shorter duration, which is already setted up for sjf
  list<Process> complete;
  int time = 0; 
  while (!workload.empty()) {
    Process p = workload.top();
    // if (cur == NULL){
    //   time = p.arrival; //initialize time to keep track of time flow, avoid edge case where first process arrival != 0
    //   cur = p;
    // }
    p.first_run = getMax(time, p.arrival);
    p.completion = p.first_run + p.duration;
    time = p.completion;
    complete.push_back(p);
    workload.pop();
  }
  return complete;
}



list<Process> stcf(pqueue_arrival workload) {
  list<Process> complete;
  pqueue_duration ready;
  int time = 0;
  while (!workload.empty() || !ready.empty()){
    while (!workload.empty() && workload.top().arrival <= time){
      ready.push(workload.top());
      workload.pop();
    }
    if (!ready.empty()){
      Process cur = ready.top();
      ready.pop();
      if (cur.first_run == -1){
        cur.first_run = time;
      }

      if (cur.duration == 0){ //avoid cases where duration is initially 0
        cur.completion = time;
        complete.push_back(cur);
        time ++;
        continue;
      }
      cur.duration --; //!Gauranteed that duration is initially not 0?
      time ++;
      //process is completed
      if(cur.duration == 0){
        cur.completion = time; 
        complete.push_back(cur);
      }
      else{
        ready.push(cur);
      }
    }
    else{
      time ++;
    }
  }
  return complete;
}

list<Process> rr(pqueue_arrival workload) {
  list<Process> complete;
  std::queue<Process> ready_queue;
  int time = 0;
  while (!workload.empty() || !ready_queue.empty()) {
      while (!workload.empty() && workload.top().arrival <= time) {
          ready_queue.push(workload.top());
          workload.pop();
      }
      if (!ready_queue.empty()) {
          Process current = ready_queue.front();
          ready_queue.pop();
          if (current.first_run == -1) {
              current.first_run = time;
          }
          current.duration--;
          time++;
          if (current.duration > 0) {
              ready_queue.push(current);
          } else {
              current.completion = time;
              complete.push_back(current);
          }
      } else {
          time++;
      }
  }
  return complete;
}

float avg_turnaround(list<Process> processes) {
  //turnaround = completion time - arrival time
  int sum = 0;
  for (Process p : processes){
    if (p.completion == -1){
      std::cerr << "Error: some processes have completion time as -1\n";
    }
    sum += p.completion - p.arrival;
  }
  return (float)sum / (float)processes.size();
}

float avg_response(list<Process> processes) {
  int sum = 0;
  for (Process p : processes){
    if (p.first_run == -1){
      std::cerr << "Error: some processes have first run time as -1\n";
    }
    sum += p.first_run - p.arrival;
  }
  return (float)sum / (float)processes.size(); //TODO: do I have to turn this to strictly float
}

void show_metrics(list<Process> processes) {
  float avg_t = avg_turnaround(processes);
  float avg_r = avg_response(processes);
  show_processes(processes);
  cout << '\n';
  cout << "Average Turnaround Time: " << avg_t << endl;
  cout << "Average Response Time:   " << avg_r << endl;
}
