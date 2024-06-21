# Process Scheduler

## Overview
The Process Scheduler is a command-line program written in C++ that simulates the scheduling of processes using different algorithms such as Shortest Job First (SJF), Longest Job First (LJF), Shortest Remaining Time First (SRTF), and Round Robin. It reads process details from an input file, predicts the best scheduling algorithm based on process characteristics, executes each algorithm, and outputs scheduling results including average waiting times, average turnaround times, and Gantt charts.

## Installation Instructions
To run the Process Scheduler on your local machine, follow these steps:

### Prerequisites
- C++ compiler (e.g., GCC)
- Make sure the C++ standard library is available

### Steps
1. **Clone the repository:**
   ```bash
   git clone https://github.com/rishavkapoor9/CPU-Scheduler.git
   cd ACM
   ```

2. **Compile the program:**
   ```bash
   g++ -o cpu_scheduler cpu_scheduler.cpp
   ```

3. **Run the program:**
   ```bash
   ./cpu_scheduler
   ```

## Usage
### Input File (`input.txt`)
Create a file named `input.txt` in the same directory as the executable. The format should be:
```
<process_id> <burst_time> <arrival_time>
<process_id> <burst_time> <arrival_time>
...
```
Example:
```
1 6 0
2 8 1
3 7 2
4 3 3
```

### Output
Upon execution, the program will generate an `output.txt` file in the same directory containing the following:
- Predicted best scheduling algorithm
- Results and Gantt charts for each scheduling algorithm (SJF, LJF, SRTF, Round Robin)

### Example Output File (`output.txt`)
```
Best algorithm predicted is SJF

SJF Scheduling:
Average Waiting Time: 4.5
Average Turnaround Time: 10.25
Gantt Chart:
| P1 | P4 | P3 | P2 |
0	6	9	16	23


LJF Scheduling:
Average Waiting Time: 5.5
Average Turnaround Time: 11.75
Gantt Chart:
| P2 | P3 | P1 | P4 |
1	9	16	22	25


SRTF Scheduling:
Average Waiting Time: 5.25
Average Turnaround Time: 10.75
Gantt Chart:
| P1 | P1 | P1 | P1 | P1 | P1 | P4 | P4 | P4 | P3 | P3 | P3 | P3 | P3 | P3 | P3 | P2 | P2 | P2 | P2 | P2 | P2 | P2 | P2 |
0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	21	22	23


Round Robin Scheduling (Quantum = 2):
Average Waiting Time: 5.75
Average Turnaround Time: 11.5
Gantt Chart:
| P1 | P2 | P3 | P4 | P1 | P2 | P3 | P1 | P2 | P3 | P2 |
0	2	4	6	8	10	12	14	16	17	19	20
```

## Dependencies
- C++ Standard Library



