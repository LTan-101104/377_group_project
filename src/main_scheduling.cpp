#include <scheduling.h>
#include <iostream>
#include <string>

using namespace std;
void compare_MLFQ_STCF_RR(string filename, int time_reboost, int num_queues, int time_slice){
  pqueue_arrival workload = read_workload(filename);
  list<Process> mlfq_complete = MLFQ(workload, time_reboost, num_queues, time_slice);
  list<Process> stcf_complete = stcf(workload);
  list<Process> rr_complete = rr(workload, time_slice);
  cout << "____________________________Running scheduling on " << filename << "____________________________" << endl;
  cout << "**MLFQ: " << endl << "- Average turnaround time: " << avg_turnaround(mlfq_complete) << endl << "- Average response time: " << avg_response(mlfq_complete) << endl;
  cout << "**STCF: " << endl << "- Average turnaround time: " << avg_turnaround(stcf_complete) << endl << "- Average response time: " << avg_response(stcf_complete) << endl;
  cout << "**RR: " << endl << "- Average turnaround time: " << avg_turnaround(rr_complete) << endl << "- Average response time: " << avg_response(rr_complete) << endl;
}

int main(int argc, char* argv[]) {
  // if (argc != 3) {
  //   cout << "usage: [fifo|sjf|stcf|rr] workload_file" << endl;
  //   exit(1);
  // }
  // string algorithm = argv[1];
  // string workload_file = argv[2];
  // pqueue_arrival workload = read_workload(workload_file);

  // // while (!workload.empty()) {
  // //   Process p = workload.top();
  // //   std::cout << p.arrival << p.duration << " ";
  // //   workload.pop();
  // // }
  // // std::cout << std::endl;

  // if (algorithm == "fifo") {
  //   show_metrics(fifo(workload));
  // } else if (algorithm == "sjf") {
  //   show_metrics(sjf(workload));
  // } else if (algorithm == "stcf") {
  //   show_metrics(stcf(workload));
  // } else if (algorithm == "rr") {
  //   show_metrics(rr(workload));
  // } else {
  //   cout << "Error: Unknown algorithm: " << algorithm << endl;
  //   cout << "usage: [fifo|sjf|stcf|rr] workload_file" << endl;
  //   exit(1);
  // }
  // return 0;

  int time_reboost = 100000;
  int num_queues = 3;
  int time_slice = 10;
  compare_MLFQ_STCF_RR("main_workload/workload_01.txt", time_reboost, num_queues, time_slice);
  compare_MLFQ_STCF_RR("main_workload/workload_02.txt", time_reboost, num_queues, time_slice);
  compare_MLFQ_STCF_RR("main_workload/workload_03.txt", time_reboost, num_queues, time_slice); 
  compare_MLFQ_STCF_RR("main_workload/workload_04.txt", time_reboost, num_queues, time_slice);
  compare_MLFQ_STCF_RR("main_workload/workload_05.txt", time_reboost, num_queues, time_slice);
}



// void compare_MLFQ_RR(string filename, int time_reboost, int num_queues, int time_slice){
//   pqueue_arrival workload = read_workload(filename);
//   list<Process> mlfq_complete = MLFQ(workload, time_reboost, num_queues, time_slice);
//   list<Process> rr_complete = rr(workload);
//   cout << "**MLFQ: " << endl << "- Average turnaround time: " << avg_turnaround(mlfq_complete) << endl << "- Average response time: " << avg_response(mlfq_complete) << endl;
//   cout << "**RR: " << endl << "- Average turnaround time: " << avg_turnaround(rr_complete) << endl << "- Average response time: " << avg_response(rr_complete) << endl;
// }

