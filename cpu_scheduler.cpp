#include <bits/stdc++.h>
#include <fstream>

using namespace std;

struct Process {
    int pid;
    int burstTime;
    int arrivalTime;
    int remainingTime;
    int waitTime;
    int turnaroundTime;
};

struct Execution {
    int pid;
    int startTime; 
    int endTime;
};

void calculateWaitAndTurnaroundTime(vector<Process>& processes) {
    for (auto& process : processes) {
        process.turnaroundTime = process.waitTime + process.burstTime;
    }
}

void printAverageTimes(const vector<Process>& processes, ofstream& outputFile) {
    int totalWaitTime = 0, totalTurnaroundTime = 0;

    for (const auto& process : processes) {
        totalWaitTime += process.waitTime;
        totalTurnaroundTime += process.turnaroundTime;
    }

    outputFile << "Average Waiting Time: " << (float)totalWaitTime / processes.size() << endl;
    outputFile << "Average Turnaround Time: " << (float)totalTurnaroundTime / processes.size() << endl;
}

void printGanttChart(const vector<Execution>& executions, ofstream& outputFile) {
    outputFile << "Gantt Chart:" << endl;
    for (const auto& exec : executions) {
        outputFile << "| P" << exec.pid << " ";
    }
    outputFile << "|" << endl;
    for (const auto& exec : executions) {
        outputFile << exec.startTime << "\t";
    }
    outputFile << executions.back().endTime << endl << endl;
}

void scheduleLJF(vector<Process> processes, ofstream& outputFile) {
    auto compareArrival = [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    };
    sort(processes.begin(), processes.end(), compareArrival);

    vector<Execution> executions;
    vector<Process> readyQueue;
    int currentTime = 0;
    int processIndex = 0;
    int totalProcesses = processes.size();

    while (processIndex < totalProcesses || !readyQueue.empty()) {
        while (processIndex < totalProcesses && processes[processIndex].arrivalTime <= currentTime) {
            readyQueue.push_back(processes[processIndex]);
            processIndex++;
        }

        if (readyQueue.empty()) {
            currentTime = processes[processIndex].arrivalTime;
            continue;
        }

        auto compareBurst = [](const Process& a, const Process& b) {
            return a.burstTime > b.burstTime;
        };
        sort(readyQueue.begin(), readyQueue.end(), compareBurst);

        Process currentProcess = readyQueue.front();
        readyQueue.erase(readyQueue.begin());

        currentProcess.waitTime = max(0, currentTime - currentProcess.arrivalTime);
        executions.push_back({currentProcess.pid, currentTime, currentTime + currentProcess.burstTime});
        currentTime += currentProcess.burstTime;

        for (auto& process : processes) {
            if (process.pid == currentProcess.pid) {
                process.waitTime = currentProcess.waitTime;
                process.turnaroundTime = process.waitTime + process.burstTime;
                break;
            }
        }
    }

    outputFile << "LJF Scheduling:" << endl;
    calculateWaitAndTurnaroundTime(processes);
    printAverageTimes(processes, outputFile);
    printGanttChart(executions, outputFile);
}

void scheduleSJF(vector<Process> processes, ofstream& outputFile) {
    auto compareArrival = [](const Process& a, const Process& b) {
        return a.arrivalTime < b.arrivalTime;
    };
    sort(processes.begin(), processes.end(), compareArrival);

    vector<Execution> executions;
    vector<Process> readyQueue;
    int currentTime = 0;
    int processIndex = 0;
    int totalProcesses = processes.size();

    while (processIndex < totalProcesses || !readyQueue.empty()) {
        while (processIndex < totalProcesses && processes[processIndex].arrivalTime <= currentTime) {
            readyQueue.push_back(processes[processIndex]);
            processIndex++;
        }

        if (readyQueue.empty()) {
            currentTime = processes[processIndex].arrivalTime;
            continue;
        }

        auto compareBurst = [](const Process& a, const Process& b) {
            return a.burstTime < b.burstTime;
        };
        sort(readyQueue.begin(), readyQueue.end(), compareBurst);

        Process currentProcess = readyQueue.front();
        readyQueue.erase(readyQueue.begin());

        currentProcess.waitTime = max(0, currentTime - currentProcess.arrivalTime);
        executions.push_back({currentProcess.pid, currentTime, currentTime + currentProcess.burstTime});
        currentTime += currentProcess.burstTime;

        for (auto& process : processes) {
            if (process.pid == currentProcess.pid) {
                process.waitTime = currentProcess.waitTime;
                process.turnaroundTime = process.waitTime + process.burstTime;
                break;
            }
        }
    }

    outputFile << "SJF Scheduling:" << endl;
    calculateWaitAndTurnaroundTime(processes);
    printAverageTimes(processes, outputFile);
    printGanttChart(executions, outputFile);
}

void scheduleSRTF(vector<Process> processes, ofstream& outputFile) {
    int totalProcesses = processes.size();
    vector<Execution> executions;
    int currentTime = 0;
    int completedProcesses = 0;

    while (completedProcesses != totalProcesses) {
        int shortestProcessIndex = -1;
        int minRemainingTime = INT_MAX;
        for (int i = 0; i < totalProcesses; i++) {
            if (processes[i].arrivalTime <= currentTime && processes[i].remainingTime > 0 && processes[i].remainingTime < minRemainingTime) {
                minRemainingTime = processes[i].remainingTime;
                shortestProcessIndex = i;
            }
        }
        if (shortestProcessIndex == -1) {
            currentTime++;
            continue;
        }
        executions.push_back({processes[shortestProcessIndex].pid, currentTime, currentTime + 1});
        processes[shortestProcessIndex].remainingTime--;
        if (processes[shortestProcessIndex].remainingTime == 0) {
            completedProcesses++;
            processes[shortestProcessIndex].waitTime = currentTime + 1 - processes[shortestProcessIndex].arrivalTime - processes[shortestProcessIndex].burstTime;
        }
        currentTime++;
    }
    calculateWaitAndTurnaroundTime(processes);
    outputFile << "SRTF Scheduling:" << endl;
    printAverageTimes(processes, outputFile);
    printGanttChart(executions, outputFile);
}

void maintainQueue(queue<int>& readyQueue, int processIndex) {
    readyQueue.push(processIndex);
}

void checkNewProcesses(int currentTime, int arrival[], int totalProcesses, queue<int>& readyQueue) {
    for (int i = 0; i < totalProcesses; i++) {
        if (arrival[i] <= currentTime && arrival[i] != -1) {
            readyQueue.push(i);
            arrival[i] = -1; 
        }
    }
}

void scheduleRoundRobin(vector<Process> processes, int timeQuantum, ofstream& outputFile) {
    vector<Execution> executions;
    int currentTime = 0;
    queue<int> readyQueue;
    int completedProcesses = 0;

    for (auto& process : processes) {
        process.remainingTime = process.burstTime;
    }

    int totalProcesses = processes.size();
    int arrival[totalProcesses];
    for (int i = 0; i < totalProcesses; i++) {
        arrival[i] = processes[i].arrivalTime;
    }

    while (completedProcesses != totalProcesses) {
        checkNewProcesses(currentTime, arrival, totalProcesses, readyQueue);

        if (readyQueue.empty()) {
            currentTime++;
            continue;
        }

        int currentProcessIndex = readyQueue.front();
        readyQueue.pop();

        int timeSlice = min(timeQuantum, processes[currentProcessIndex].remainingTime);
        executions.push_back({processes[currentProcessIndex].pid, currentTime, currentTime + timeSlice});
        processes[currentProcessIndex].remainingTime -= timeSlice;
        currentTime += timeSlice;

        checkNewProcesses(currentTime, arrival, totalProcesses, readyQueue);

        if (processes[currentProcessIndex].remainingTime > 0) {
            maintainQueue(readyQueue, currentProcessIndex);
        } else {
            completedProcesses++;
            processes[currentProcessIndex].waitTime = currentTime - processes[currentProcessIndex].arrivalTime - processes[currentProcessIndex].burstTime;
        }
    }

    calculateWaitAndTurnaroundTime(processes);
    outputFile << "Round Robin Scheduling (Quantum = " << timeQuantum << "):" << endl;
    printAverageTimes(processes, outputFile);
    printGanttChart(executions, outputFile);
}

string predictBestAlgorithm(const vector<Process>& processes) {
    double totalBurstTime = accumulate(processes.begin(), processes.end(), 0.0, 
        [](double sum, const Process& process) { return sum + process.burstTime; });
    double avgBurstTime = totalBurstTime / processes.size();

    bool allBurstTimesSame = all_of(processes.begin(), processes.end(), 
        [&](const Process& process) { return process.burstTime == processes[0].burstTime; });

    if (allBurstTimesSame) {
        return "FCFS";
    }

    double burstTimeVariance = 0;
    for (const auto& process : processes) {
        burstTimeVariance += (process.burstTime - avgBurstTime) * (process.burstTime - avgBurstTime);
    }
    burstTimeVariance /= processes.size();

    double totalArrivalTime = accumulate(processes.begin(), processes.end(), 0.0, 
        [](double sum, const Process& process) { return sum + process.arrivalTime; });
    double avgArrivalTime = totalArrivalTime / processes.size();

    double arrivalTimeVariance = 0;
    for (const auto& process : processes) {
        arrivalTimeVariance += (process.arrivalTime - avgArrivalTime) * (process.arrivalTime - avgArrivalTime);
    }
    arrivalTimeVariance /= processes.size();

    int longBurstCount = count_if(processes.begin(), processes.end(), [&](const Process& process) {
        return process.burstTime > avgBurstTime;
    });

    if (burstTimeVariance < avgBurstTime && arrivalTimeVariance < avgArrivalTime) {
        return "SJF";
    } else if (burstTimeVariance < avgBurstTime) {
        return "SRTF";
    } else if (longBurstCount > processes.size() / 2) {
        return "LJF";
    } else {
        return "Round Robin";
    }
}

void runScheduler() {
    ifstream inputFile("input.txt");
    ofstream outputFile("output.txt");

    if (!inputFile.is_open()) {
        cerr << "Error reading input file 🥲" << endl;
        return;
    }
    if (!outputFile.is_open()) {
        cerr << "Error reading output file 🥲" << endl;
        return;
    }

    vector<Process> processes;
    int pid, burstTime, arrivalTime;

    while (inputFile >> pid >> burstTime >> arrivalTime) {
        processes.push_back({pid, burstTime, arrivalTime, burstTime, 0, 0});
    }

    if (processes.empty()) {
        outputFile << "No processes found in input file" << endl;
        return;
    } else {
        for (const auto& process : processes) {
            cerr << "Loaded Process - PID: " << process.pid << ", Burst Time: " << process.burstTime << ", Arrival Time: " << process.arrivalTime << endl;
        }
    }

    string predictedAlgorithm = predictBestAlgorithm(processes);
    outputFile << "Best algorithm predicted is " << predictedAlgorithm << endl << endl;

    scheduleSJF(processes, outputFile);
    scheduleLJF(processes, outputFile);
    scheduleSRTF(processes, outputFile);
    scheduleRoundRobin(processes, 2, outputFile);

    inputFile.close();
    outputFile.close();
}

int main() {
    runScheduler();
    return 0;
}
