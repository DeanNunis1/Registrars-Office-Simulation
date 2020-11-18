#include "registrar.h"
#include <fstream>
#include <algorithm>
#include <iostream>


/**
 * Constructor
 * @param filename Name of the file containing the input data.
 */
Registar::Registar(const char *filename)
    : num_windows(0) {

    // Open file
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        throw std::runtime_error("Error: Cannot open file");
    }

    // Read input
    stream >> num_windows;
    int arrive;
    int num_students;
    stream >> arrive >> num_students;
    while (stream) {
        for (int i = 0; i < num_students; ++i) {
            int amount;
            stream >> amount;
            queue.enqueue(Student(arrive, amount));
        }
        stream >> arrive >> num_students;
    }
}


/**
 * Perform the simulation.
 */
void Registar::simulate() {

    // Create num_windows windows
    List <RegistrarWindow> windows;
    for (int i = 0; i < num_windows; ++i) {
        windows.push_back(RegistrarWindow());
    }

    int current_time = 0;           // Current clock tick
    int request_processing = 0;     // Current number of requests being processed
    bool simulation_end = queue.empty();

    /* For keeping track of student idle times */
    int num_students = (int) queue.size();
    int *student_idle_times = new int[num_students];
    int curr_student = 0;

    while (!simulation_end) {
        // Perform tick for current_time

        // Process the windows
        for (int i = 0; i < num_windows && current_time != 0; ++i) {
            // Window is idle
            if (windows[i].occupied_till == -1) {
                windows[i].idle += 1;
            }
            // Student's request is complete. Remove it.
            else if (windows[i].occupied_till == current_time) {
                windows[i].occupied_till = -1;
                request_processing -= 1;
            }
        }

        // Process the students who arrive at current time
        // from the queue if any windows are free.
        while (request_processing < num_windows && !queue.empty()
                                && current_time >= queue.front().arrive) {
            Student student = queue.front();
            queue.dequeue();

            int idle_time = current_time - student.arrive;
            student_idle_times[curr_student++] = idle_time;

            // Skip
            if (student.amount == 0)
                continue;

            // Find the free window and start processing the request
            for (int i = 0; i < num_windows; ++i) {
                if (windows[i].occupied_till == -1) {
                    windows[i].occupied_till = current_time + student.amount;
                    request_processing++;
                    break;
                }
            }
        }

        current_time++;
        simulation_end = request_processing == 0 && queue.empty();
    }

    // Calculate student statistics
    std::sort(student_idle_times, student_idle_times + num_students);
    int student_mean = 0;
    double student_median = 0;
    int student_longest = -1;
    int student_idle = 0;
    for (int i = 0; i < num_students; ++i) {
        student_mean += student_idle_times[i];
        if (student_idle_times[i] > student_longest)
            student_longest = student_idle_times[i];
        if (student_idle_times[i] > 10)
            student_idle++;
    }
    // Calculate median
    if (num_students % 2 == 0) {
        int m = num_students / 2;
        student_median = (student_idle_times[m] + student_idle_times[m-1]) / 2.0;
    }
    else {
        student_median = student_idle_times[num_students / 2];
    }
    delete[] student_idle_times;

    std::cout << "The mean student wait time: " << (1.0 * student_mean / num_students) << std::endl;
    std::cout << "The median student wait time: " << student_median << std::endl;
    std::cout << "The longest student wait time: " << student_longest << std::endl;
    std::cout << "The number of students waiting over 10 minutes: " << student_idle << std::endl;

    // Calculate window statistics
    int window_mean = 0;
    int window_longest = -1;
    int num_window_idle = 0;

    for (int i = 0; i < num_windows; ++i) {
        window_mean += windows[i].idle;
        if (windows[i].idle > window_longest)
            window_longest = windows[i].idle;
        if (windows[i].idle > 5)
            num_window_idle++;
    }

    std::cout << "The mean window idle time: " << (1.0 * window_mean / num_windows) << std::endl;
    std::cout << "The longest window idle time: " << window_longest << std::endl;
    std::cout << "Number of windows idle for over 5 minutes: " << num_window_idle << std::endl;

}

