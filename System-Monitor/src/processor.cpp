#include "processor.h"
#include "linux_parser.h"
#include <vector>
#include <string>

// Return the aggregate CPU utilization //
float Processor::Utilization() { 
    // PrevIdle = previdle + previowait
    // Idle = idle + iowait

    // PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
    // NonIdle = user + nice + system + irq + softirq + steal

    // PrevTotal = PrevIdle + PrevNonIdle
    // Total = Idle + NonIdle

    // # differentiate: actual value minus the previous one
    // totald = Total - PrevTotal
    // idled = Idle - PrevIdle

    // CPU_Percentage = (totald - idled)/totald

    // 0:user   1:nice   2:system   3:idle  4:iowait   5:irq   6:softirq   7:steal   8:guest   9:guest_nice
    std::vector<std::string> cpu = LinuxParser::CpuUtilization();
    long user = std::stol(cpu[0]), 
         nice = std::stol(cpu[1]), 
         system = std::stol(cpu[2]), 
         idle = std::stol(cpu[3]), 
         iowait = std::stol(cpu[4]), 
         irq = std::stol(cpu[5]), 
         softirq = std::stol(cpu[6]), 
         steal = std::stol(cpu[7]),
         guest = std::stol(cpu[8]), 
         guest_nice = std::stol(cpu[9]);
    
    float PrevIdle = idle_ + iowait_;
    float Idle = std::stol(cpu[3]) + std::stol(cpu[4]);

    float PrevNonIdle = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    float NonIdle = user + nice + system + irq + softirq + steal;

    float PrevTotal = PrevIdle + PrevNonIdle;
    float Total = Idle + NonIdle;

    float totald = Total - PrevTotal;
    float idled = Idle - PrevIdle;

    float CPU_Percentage = (totald - idled) / totald;

    user_ = user;
    nice_ = nice;
    system_ = system;
    idle_ = idle;
    iowait_ = iowait;
    irq_ = irq;
    softirq_ = softirq;
    steal_ = steal;
    guest_ = guest;
    guest_nice_ = guest_nice;

    return CPU_Percentage; 
}