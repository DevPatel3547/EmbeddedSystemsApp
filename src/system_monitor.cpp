/*
 * system_monitor.cpp
 *
 * A multi-threaded daemon to monitor CPU, memory, and power usage.
 * It updates a shared global structure and writes the metrics to /tmp/metrics.json.
 */

 #include <iostream>
 #include <fstream>
 #include <sstream>
 #include <string>
 #include <thread>
 #include <mutex>
 #include <chrono>
 #include <cstdlib>
 #include <unistd.h> // for sleep()
 
 // Structure to hold system metrics
 struct SystemMetrics {
     double cpuUsage;   // in percent
     double memUsage;   // in percent
     double powerUsage; // in watts (or simulated value)
 };
 
 std::mutex metricsMutex;
 SystemMetrics globalMetrics = {0.0, 0.0, 0.0};
 
 // Helper: Write the current metrics to a JSON file.
 void writeMetricsToFile() {
     std::lock_guard<std::mutex> lock(metricsMutex);
     std::ofstream ofs("/tmp/metrics.json");
     if (ofs) {
         ofs << "{\n"
             << "  \"cpuUsage\": " << globalMetrics.cpuUsage << ",\n"
             << "  \"memUsage\": " << globalMetrics.memUsage << ",\n"
             << "  \"powerUsage\": " << globalMetrics.powerUsage << "\n"
             << "}\n";
         ofs.close();
     } else {
         std::cerr << "Error: Unable to write to /tmp/metrics.json\n";
     }
 }
 
 // Thread function to monitor CPU usage
 void monitorCPU() {
     long long prevIdle = 0, prevTotal = 0;
     bool firstRun = true;
     while (true) {
         std::ifstream statFile("/proc/stat");
         if (!statFile.is_open()) {
             std::cerr << "Error: Cannot open /proc/stat\n";
             std::this_thread::sleep_for(std::chrono::seconds(1));
             continue;
         }
         std::string line;
         std::getline(statFile, line); // Get the first line: "cpu  ..."
         statFile.close();
         
         std::istringstream iss(line);
         std::string cpu;
         long long user, nice, system, idle, iowait, irq, softirq, steal;
         iss >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
         
         long long idleTime = idle + iowait;
         long long totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
         
         if (!firstRun) {
             long long deltaIdle = idleTime - prevIdle;
             long long deltaTotal = totalTime - prevTotal;
             double usage = 0.0;
             if (deltaTotal != 0)
                 usage = (100.0 * (deltaTotal - deltaIdle)) / deltaTotal;
             {
                 std::lock_guard<std::mutex> lock(metricsMutex);
                 globalMetrics.cpuUsage = usage;
             }
         } else {
             firstRun = false;
         }
         
         prevIdle = idleTime;
         prevTotal = totalTime;
         
         std::this_thread::sleep_for(std::chrono::seconds(1));
     }
 }
 
 // Thread function to monitor memory usage
 void monitorMemory() {
     while (true) {
         std::ifstream memFile("/proc/meminfo");
         if (!memFile.is_open()) {
             std::cerr << "Error: Cannot open /proc/meminfo\n";
             std::this_thread::sleep_for(std::chrono::seconds(1));
             continue;
         }
         std::string line;
         long long memTotal = 0, memAvailable = 0;
         while (std::getline(memFile, line)) {
             std::istringstream iss(line);
             std::string key;
             long long value;
             std::string unit;
             iss >> key >> value >> unit;
             if (key == "MemTotal:") {
                 memTotal = value;
             } else if (key == "MemAvailable:") {
                 memAvailable = value;
                 break; // We have what we need
             }
         }
         memFile.close();
         
         double usage = 0.0;
         if (memTotal != 0)
             usage = (100.0 * (memTotal - memAvailable)) / memTotal;
         {
             std::lock_guard<std::mutex> lock(metricsMutex);
             globalMetrics.memUsage = usage;
         }
         
         std::this_thread::sleep_for(std::chrono::seconds(1));
     }
 }
 
 // Thread function to monitor power usage
 void monitorPower() {
     while (true) {
         double power = 0.0;
         // Attempt to read a real sensor value.
         // Many systems expose power info at /sys/class/power_supply/BAT0/power_now (in microwatts).
         std::ifstream powerFile("/sys/class/power_supply/BAT0/power_now");
         if (powerFile.is_open()) {
             powerFile >> power;
             powerFile.close();
             // Convert microwatts to watts.
             power = power / 1000000.0;
         } else {
             // If no sensor available (common on servers), simulate a value:
             power = 50.0 + (std::rand() % 20); // random value between 50-70 watts
         }
         {
             std::lock_guard<std::mutex> lock(metricsMutex);
             globalMetrics.powerUsage = power;
         }
         
         std::this_thread::sleep_for(std::chrono::seconds(1));
     }
 }
 
 int main() {
     std::cout << "Starting system_monitor daemon...\n";
     
     // Start monitoring threads
     std::thread cpuThread(monitorCPU);
     std::thread memThread(monitorMemory);
     std::thread powerThread(monitorPower);
     
     // Main loop: write metrics to file every 2 seconds.
     while (true) {
         writeMetricsToFile();
         std::this_thread::sleep_for(std::chrono::seconds(2));
     }
     
     // (In a real daemon, you’d catch termination signals and join threads properly.)
     cpuThread.join();
     memThread.join();
     powerThread.join();
     
     return 0;
 }
 