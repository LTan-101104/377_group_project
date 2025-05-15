#include <gtest/gtest.h>
#include <scheduling.h>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//test for reading new workload
//read new worload1
TEST (SchedulingTest, ReadWorkloadNew){
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
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
  pqueue_arrival pq = read_workload("workloads/workload_02.txt");
  EXPECT_EQ(pq.size(), 3);
  Process p1 = pq.top();
  EXPECT_EQ(p1.arrival, 0);
  // EXPECT_EQ(p1.duration, 100);
  // std::cout << "Duration:" << p1.duration << endl;
  EXPECT_EQ(p1.time_demand, 10);
  pq.pop();
  while (!pq.empty()){
    Process p = pq.top();
    EXPECT_EQ(p.arrival, 0);
    // std::cout << "Duration:" << p.duration << endl;
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
//!MLFQ
//! varied arrival time on single queue testing
//list<Process> MLFQ(pqueue_arrival workload, int time_reboost, int num_queues, int time_slice)
TEST(SchedulingTest, MLFQ_test_simple_queue_1){
  //simple case, uniform time demand, no reboost, only 1 num queue
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 20);
  EXPECT_FLOAT_EQ(r, 10);
}

//! varied reboost test, if num_queue = 0, this should not interfere
TEST(SchedulingTest, MLFQ_test_time_boost_1){
  int NUM_Q = 1;
  int time_reboost = 5; 
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 20);
  EXPECT_FLOAT_EQ(r, 10);
}

//more complicate input
TEST(SchedulingTest, MLFQ_test_simple_queue_2){
  //simple case, uniform time demand, no reboost, only 1 num queue
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/workload_02.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  // ASSERT_NEAR(56.666667f, t, 0.01);
  EXPECT_FLOAT_EQ(t, 50);
  EXPECT_FLOAT_EQ(r, 10);
}

TEST(SchedulingTest, MLFQ_test_time_boost_2){
  //simple case, uniform time demand, no reboost, only 1 num queue
  int NUM_Q = 1;
  int time_reboost = 10; 
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/workload_02.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 50);
  EXPECT_FLOAT_EQ(r, 10);
}

//! test how the anti-gaming mechanism work
TEST(SchedulingTest, MLFQ_test_simple_queue_3){
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 10; // first workload will have time demand < time slice
  pqueue_arrival pq = read_workload("workloads/workload_03A.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  ASSERT_NEAR(t, 23.3334f, 0.01);
  EXPECT_FLOAT_EQ(r, 10);
}

//!test difference time slice
//all current test used time_slice < time_demand so not account for the edge cases of time demand yet
TEST(SchedulingTest, MLFQ_test_time_slice_1){
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 5; //time slice < time demand so will always take the time slice
  pqueue_arrival pq = read_workload("workloads/workload_01.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(r, 5);
  EXPECT_FLOAT_EQ(t, 25);
}

TEST(SchedulingTest, MLFQ_test_time_slice_2){
  int NUM_Q = 1;
  int time_reboost = 10000; // high time reboost to avoid reboosting
  int time_slice = 5; //time slice < time demand so will always take the time slice
  pqueue_arrival pq = read_workload("workloads/workload_02.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(r, 5);
  EXPECT_FLOAT_EQ(t, 55);
}

//!new added tests for multiple queues

TEST(SchedulingTest, MLFQ_test_double_queue_1){
  //simple case, uniform time demand, no reboost, only 2 num queue with no gaming
  int NUM_Q = 2;
  int time_reboost = 1000000; // high time reboost to avoid reboosting
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/workload_02.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 50);
  EXPECT_FLOAT_EQ(r, 10.0f);
}

//2 queue with low reboost time
TEST(SchedulingTest, MLFQ_test_double_queue_reboost_1){
  int NUM_Q = 2;
  int time_reboost = 40; // high time reboost to avoid reboosting
  int time_slice = 10;
  pqueue_arrival pq = read_workload("workloads/workload_02A.txt");
  list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
  float t = avg_turnaround(xs);
  float r = avg_response(xs);
  EXPECT_FLOAT_EQ(t, 57.5f);
  EXPECT_FLOAT_EQ(r, 15);
}

//3 queue with reboost
TEST(SchedulingTest, MLFQ_test_triple_queue_reboost_mid) {
    int NUM_Q = 3;
    int time_reboost = 50;
    int time_slice = 10;
    pqueue_arrival pq = read_workload("workloads/workload_03B.txt");
    list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
    float t = avg_turnaround(xs);
    float r = avg_response(xs);
    EXPECT_FLOAT_EQ(t, 58.75f);  // Expected turnaround time with 3 queues
    EXPECT_FLOAT_EQ(r, 11.25f);   // Expected response time with 3 queues
}


TEST(SchedulingTest, MLFQ_test_four_queue_same_workload) {
    int NUM_Q = 4;
    int time_reboost = 50;
    int time_slice = 10;
    pqueue_arrival pq = read_workload("workloads/workload_03B.txt");
    list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
    float t = avg_turnaround(xs);
    float r = avg_response(xs);
    EXPECT_FLOAT_EQ(t, 58.75f);  // Expected better turnaround with more queues
    EXPECT_FLOAT_EQ(r, 11.25f);   // Response time might remain same
}



// Test with very low reboost time - frequent reset of all processes to top queue
TEST(SchedulingTest, MLFQ_test_low_reboost) {
    int NUM_Q = 3;
    int time_reboost = 20;  // Low reboost time
    int time_slice = 10;
    pqueue_arrival pq = read_workload("workloads/workload_02A.txt");
    list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
    float t = avg_turnaround(xs);
    float r = avg_response(xs);
    EXPECT_FLOAT_EQ(t, 57.5f);  // Expected turnaround with frequent reboost
    EXPECT_FLOAT_EQ(r, 15.0f);  // Expected response time
}


TEST(SchedulingTest, MLFQ_anti_gaming2_queues) {
    int NUM_Q = 2;
    int time_reboost = 20000000;  // Low reboost time
    int time_slice = 10;
    pqueue_arrival pq = read_workload("workloads/workload_03C.txt");
    list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
    float t = avg_turnaround(xs);
    float r = avg_response(xs);
    EXPECT_FLOAT_EQ(t, 15);  // Expected turnaround with frequent reboost
    EXPECT_FLOAT_EQ(r, 2.5);  // Expected response time
}

//for countering time slice/ actual time runs exceed finish time
TEST(SchedulingTest, Edge_case) {
    int NUM_Q = 2;
    int time_reboost = 20000000;  // Low reboost time
    int time_slice = 10;
    pqueue_arrival pq = read_workload("workloads/workload_06.txt");
    list<Process> xs = MLFQ(pq, time_reboost, NUM_Q, time_slice);
    float t = avg_turnaround(xs);
    float r = avg_response(xs);
    EXPECT_FLOAT_EQ(t, 7);  // Expected turnaround with frequent reboost
    EXPECT_FLOAT_EQ(r, 0);  // Expected response time
}
TEST(SchedulingTest, Edge_case_rr) {
    int time_slice = 10;
    pqueue_arrival pq = read_workload("workloads/workload_06.txt");
    list<Process> xs = rr(pq, time_slice);
    float t = avg_turnaround(xs);
    float r = avg_response(xs);
    EXPECT_FLOAT_EQ(t, 7);  // Expected turnaround with frequent reboost
    EXPECT_FLOAT_EQ(r, 0);  // Expected response time
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
