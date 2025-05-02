#include <gtest/gtest.h>
#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

TEST(SchedulingTest, ReadWorkload1) {
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
  EXPECT_EQ(pq.size(), 3);
}

//test for reading new workload 
//read new worload1
TEST (SchedulingTest, ReadWorkloadNew){
  pqueue_arrival pq = read_workload("workloads/new_workload_01.txt");
  EXPECT_EQ(pq.size(), 3);
  while (!pq.empty()){
    Process p = pq.top();
    EXPECT_EQ(p.arrival, 0);
    EXPECT_EQ(p.duration, 10);
    EXPECT_EQ(p.time_demand, 10);
    pq.pop();
  }
}

//test for reading new workload2
TEST (SchedulingTest, ReadWorkloadNew2){
  pqueue_arrival pq = read_workload("workloads/new_workload_02.txt");
  EXPECT_EQ(pq.size(), 3);
  Process p1 = pq.top();
  EXPECT_EQ(p1.arrival, 0);
  EXPECT_EQ(p1.duration, 100);
  EXPECT_EQ(p1.time_demand, 10);
  pq.pop();
  while (!pq.empty()){
    Process p = pq.top();
    EXPECT_EQ(p.arrival, 0);
    EXPECT_EQ(p.duration, 10);
    EXPECT_EQ(p.time_demand, 10);
    pq.pop();
  }
}

TEST(SchedulingTest, FIFO1) {
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
  list<Process> xs = fifo(pq);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 20);
  EXPECT_FLOAT_EQ(r, 10);
}

//list<Process> MLFQ(pqueue_arrival workload, int time_reboost, int num_queues, int time_slice)
TEST(SchedulingTest, MLFQ_test_simple_queue_1){
  //simple case, uniform time demand, no reboost, only 1 num queue
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/new_workload_01.txt")
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 20);
  EXPECT_FLOAT_EQ(r, 10);
}

TEST(SchedulingTest, MLFQ_test_simple_queue_2){
  //simple case, uniform time demand, no reboost, only 1 num queue
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 10;
  list<Process> xs = read_workload("workloads/new_workload_02.txt");
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 20);
  EXPECT_FLOAT_EQ(r, 10);
}

TEST(SchedulingTest, MLFQ_test_double_queue_1){
  //simple case, uniform time demand, no reboost, only 1 num queue
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 10;
  list<Process> xs = read_workload("workloads/new_workload_02.txt");
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 20);
  EXPECT_FLOAT_EQ(r, 10);
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
