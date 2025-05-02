#include <scheduling.h>
#include <fstream>
#include <iostream>
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
    if (iss >> p.arrival >> p.duration >> p.time_demand){
      p.first_run = -1; //initialize first run
      p.completion = -1;
      p.remain_time_on_slice = -1; //initialize remain time on slice
      workload.push(p);
      cout << "adding workload:" << p.arrival << " " << p.duration << " " << p.time_demand << endl;
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
    cout << '\t' << p.arrival << ' ' << p.duration << ' ' << p.time_demand << endl;
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
         << ", remain_time_on_slice=" << p.remain_time_on_slice << ", time_demand=" << p.time_demand << endl;
    xs.pop_front();
  }
}

//TODO: implement MLFQ scheduling
/*
-Parameters:
  -workload: a priority queue of processes sorted by arrival time (priority queue on arrival time)
  - time_reboost: times between each reboost (push all processes to highest queue)
  - num_queues: number of queues used (0 being the highest priority)
  - time_slice: time_slice for each queue

- the choice in each queue is the same as in RR implementation
- after processing a process, reset cur_queue_no to 0
- input format of a workload line <arrival_time> <duration>

--> return: list<Process>
*/
// check if all queues are empty
bool check_all_queues_empty(vector<queue<Process>> queues)
{
  for (auto & cur : queues){
    if (!cur.empty()){
      return false;
    }
  }
  return true;
}
//find the index of the highest priority queue which is not empty
int getHighestPriorityQueue(vector<queue<Process>> queues){
  int res =-1;
  for (int i = 0; i < queues.size(); i++){
    if (!queues[i].empty()){
      res = i;
      break;
    }
  }
  return res;
}


list<Process> MLFQ(pqueue_arrival workload, int time_reboost, int num_queues, int time_slice){
  show_workload(workload);
  list<Process> complete;
  vector<std::queue<Process>> list_queues; // container for all queues
  int max_queue_idx = num_queues - 1;
  cout << "time slice: " << time_slice << endl;

  //Assign queues
  for (int i = 0; i < num_queues; i++){
    std::queue<Process> cur_queue;
    list_queues.push_back(cur_queue);
    cout << "queue num " << i << " created" << endl;
  }
  cerr << "queues assigned" << endl;

  int time = 0; // track time flow
  int time_since_reboost = 0;

  while (!workload.empty() || !check_all_queues_empty(list_queues)){
    //cout << workload.empty() << endl;
    cout << "loop instance" << endl;
    //check if need to reboost
    //!Assuming reboost and adding new processes will happen at the same time and not cost time
    if (time_since_reboost >= time_reboost){
      time_since_reboost = 0;
      for (int i = 1; i < num_queues; i ++){
        while (!list_queues[i].empty()){
          list_queues[0].push(list_queues[i].front());
          list_queues[i].pop();
        }
      }
      cout << "reboosted" << endl;
    }
    //DEBUG
    //cerr << workload.size() << endl;

    //keep running algorithm
    //move all valid workload to the highest queue
    while (!workload.empty() && workload.top().arrival <= time){
      Process to_add = workload.top();
      workload.pop();
      to_add.remain_time_on_slice = time_slice;
      list_queues[0].push(to_add);
    }

    if (!check_all_queues_empty(list_queues)){
      int highest = getHighestPriorityQueue(list_queues);
      if (highest == -1){
        time += time_slice;
        time_since_reboost += time_slice;
        cout << "skipped" << endl;
        continue;
      }

      // TODO: run rr on found queue, update any process that is pushed down as well, this should manage only 1 PROCESS only because after that there might be incoming processes that need to be dealt with first
      //TODO: update time and time_since_reboost accordingly
      //TODO: update priority of process, or pop out if done accordingly
      queue<Process>& ready_queue = list_queues[highest];
      cout << "ready queue before popping has size:" << ready_queue.size() << endl;
      cout << "queue no. : " << highest << endl;

      //add any new incoming workload during processing
      while (!workload.empty() && workload.top().arrival <= time) {
          ready_queue.push(workload.top());
          workload.pop();
          cout << "workload added" << endl;
      }
      if (!ready_queue.empty()) {

          Process current = ready_queue.front();
          ready_queue.pop();
          cout << "ready queue has size: " << ready_queue.size() << endl;
          if (current.first_run == -1) {
              current.first_run = time;
          }
          //demand check
          cout << "current process has duration " << current.duration << " and demand " << current.time_demand << " with remaining time " << current.remain_time_on_slice << " on slice " << highest << endl;

          if (current.remain_time_on_slice < current.time_demand)
          {
            current.duration -= current.remain_time_on_slice;
            current.remain_time_on_slice -= current.remain_time_on_slice;
          } else
          {
             current.duration -= current.time_demand;
             current.remain_time_on_slice -= current.time_demand;
          }
          cout << "current process has duration " << current.duration << " left" << endl;

          if (current.duration > 0)
          {
            if (current.remain_time_on_slice == 0)
            {
                list_queues[highest + 1].push(current);
                cout << "pushed to lower queue" << endl;
                current.remain_time_on_slice = time_slice; //reset slice allotment
                continue;
            } else
            {
              ready_queue.push(current);
              cout << "stay on current queue" << endl;
              current.remain_time_on_slice = time_slice; //reset slice allotment
              continue;
            }
          } else { //finished
              current.completion = time + time_slice;
              cout << "process completed. First run: " << current.first_run << ", completion: " << current.completion << endl;
              complete.push_back(current);
              cout << ready_queue.size() << " items left on current queue" << endl;
          }
      }
      time += time_slice;
    }
  }
  cout << "MLFQ completed";
  return complete;
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
  list<Process> complete;
  pqueue_duration ready_queue;
  int current_time = 0;

  while (!workload.empty() || !ready_queue.empty()) {
    while (!workload.empty() && workload.top().arrival <= current_time) {
      Process p = workload.top();
      ready_queue.push(p);
      workload.pop();
    }

    if (ready_queue.empty() && !workload.empty()) {
      current_time = workload.top().arrival;
      continue;
    }

    if (!ready_queue.empty()) {
      Process p = ready_queue.top();
      ready_queue.pop();

      p.first_run = current_time;
      p.completion = current_time + p.duration;

      current_time = p.completion;

      complete.push_back(p);
    }
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

list<Process> rr(pqueue_arrival workload) {//workload is a heap sorted by arrival time
  list<Process> complete; //completed list of Processes
  std::queue<Process> ready_queue;
  int time = 0;
  while (!workload.empty() || !ready_queue.empty()) {
      while (!workload.empty() && workload.top().arrival <= time) {//move all from workload to ready_queue?
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
