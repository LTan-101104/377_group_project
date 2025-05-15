#include <list>
#include <queue>
#include <string>

using namespace std;

typedef struct Process Process;
struct Process {
  int arrival;
  int first_run;
  int duration;
  int completion;
  int remain_time_on_slice; // for MLFQ to avoid anti-gaming
  int time_demand; // actual time demanded on MLFQ 
};

class ArrivalComparator { //compare by arrival time
 public:
  bool operator()(const Process lhs, const Process rhs) const {
    if (lhs.arrival != rhs.arrival)
      return lhs.arrival > rhs.arrival; //TODO: recheck whether this means we will put lhs first or rhs
    else
      return lhs.duration > rhs.duration;
  }
};

class DurationComparator { //compare by duration time
 public:
  bool operator()(const Process lhs, const Process rhs) const {
    if (lhs.duration != rhs.duration)
      return lhs.duration > rhs.duration;
    else
      return lhs.arrival > rhs.arrival;
  }
};

typedef priority_queue<Process, vector<Process>, ArrivalComparator>
    pqueue_arrival; //for now it will compare primaily by arrival time and it is a min heap
typedef priority_queue<Process, vector<Process>, DurationComparator>
    pqueue_duration;

pqueue_arrival read_workload(string filename);
void show_workload(pqueue_arrival workload);
void show_processes(list<Process> processes);

list<Process> fifo(pqueue_arrival workload);
list<Process> sjf(pqueue_arrival workload);
list<Process> stcf(pqueue_arrival workload);
list<Process> rr(pqueue_arrival workload);
list<Process> MLFQ(pqueue_arrival workload, int time_reboost, int num_queues, int time_slice);

float avg_turnaround(list<Process> processes);
float avg_response(list<Process> processes);
void show_metrics(list<Process> processes);

#ifdef DEBUGMODE
#define debug(msg) \
    std::cout <<"[" << __FILE__ << ":" << __LINE__ << "] " << msg << std::endl;
#else
#define debug(msg)
#endif
