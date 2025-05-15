#include <scheduling.h>
#include <iostream>
#include <string>

using namespace std;
void compare_MLFQ(pqueue_arrival workload, int time_reboost, int num_queues, int time_slice){
  list<Process> mlfq_complete = MLFQ(workload, time_reboost, num_queues, time_slice);
  cout << "**MLFQ - time slice: " << time_slice << ", time reboost: " << time_reboost << ", num_queues: " << num_queues<< "- :" << endl << "- Average turnaround time: " << avg_turnaround(mlfq_complete) << endl << "- Average response time: " << avg_response(mlfq_complete) << endl;
}

void compare_RR(pqueue_arrival workload, int time_slice){
  list<Process> rr_complete = rr(workload, time_slice);
  cout << "**RR: - time slice: " << time_slice << "- :" << endl << "- Average turnaround time: " << avg_turnaround(rr_complete) << endl << "- Average response time: " << avg_response(rr_complete) << endl;
}

void compare_STCF(pqueue_arrival workload){
  list<Process> stcf_complete = stcf(workload);
  cout << "**STCF:" << endl << "- Average turnaround time: " << avg_turnaround(stcf_complete) << endl << "- Average response time: " << avg_response(stcf_complete) << endl;
}


int main(int argc, char* argv[]) {
  int time_reboost[] = {20, 100};
  int num_queues[] = {1,4};
  int time_slice[] = {5, 10};
  const char* filename[] = {"main_workload/workload_01.txt", "main_workload/workload_02.txt", "main_workload/workload_03.txt", "main_workload/workload_04.txt", "main_workload/workload_05.txt", "main_workload/workload_06.txt"};
  for (const char* filename : filename){
    cout << "___________________________________Running on " << filename << "___________________________________" << endl;
    cout << endl;
    cout << endl;
    pqueue_arrival workload = read_workload(filename);
    compare_STCF(workload);
    for (int time_slice : time_slice){
      cout << endl;
      compare_RR(workload, time_slice);
      cout << endl;
      for (int num_queues : num_queues){
        for (int time_reboost : time_reboost){
          compare_MLFQ(workload, time_reboost, num_queues, time_slice);
        }
      }
    }
  }
}



// void compare_MLFQ_RR(string filename, int time_reboost, int num_queues, int time_slice){
//   pqueue_arrival workload = read_workload(filename);
//   list<Process> mlfq_complete = MLFQ(workload, time_reboost, num_queues, time_slice);
//   list<Process> rr_complete = rr(workload);
//   cout << "**MLFQ: " << endl << "- Average turnaround time: " << avg_turnaround(mlfq_complete) << endl << "- Average response time: " << avg_response(mlfq_complete) << endl;
//   cout << "**RR: " << endl << "- Average turnaround time: " << avg_turnaround(rr_complete) << endl << "- Average response time: " << avg_response(rr_complete) << endl;
// }

