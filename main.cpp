// CPU Scheduler Simulator
#include <iostream>
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <iomanip>
#include <algorithm>

int MenuPage()
{
    const std::string Menu1 = "1. Add Process Data Manually";
    const std::string Menu2 = "2. Display Simulated Process Data";
    const std::string Menu3 = "3. Run FCFS Simulation";
    const std::string Menu4 = "4. View REAL Windows OS Processes";
    const std::string Menu5 = "5. Exit";

    // Display the choices to the user
    std::cout << "--- CPU Scheduler Menu ---" << std::endl;
    std::cout << Menu1 << std::endl;
    std::cout << Menu2 << std::endl;
    std::cout << Menu3 << std::endl;
    std::cout << Menu4 << std::endl;
    std::cout << Menu5 << std::endl;
    std::cout << "Choose an option (1-5): ";

    // Taking users choice
    int choice = 0;
    std::cin >> choice;
    return choice;
}

void CpuProcesses()
{
    // 1. Take a snapshot of all running processes
    // TH32CS_SNAPPROCESS tells Windows to capture a list of all active processes.
    HANDLE allSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (allSnapshot == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to create system snapshot." << std::endl;
        return;
    }

    // 2. Initialize the PROCESSENTRY32 structure
    // This structure describes a single process from the snapshot.
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32); // Must be set before calling the API

    // 3. Print the table headers
    std::cout << std::left << std::setw(10) << "PID" << std::setw(15) << "Threads" << "Process Name" << std::endl;

    std::cout << std::string(50, '-') << std::endl;

    // 4. Retrieve the first process and loop through the rest
    if (Process32First(allSnapshot, &pe32))
    {
        do
        {
            std::cout << std::left << std::setw(10) << pe32.th32ProcessID << std::setw(15) << pe32.cntThreads << pe32.szExeFile << std::endl;

        } while (Process32Next(allSnapshot, &pe32));
    }
    else
    {
        std::cerr << "Failed to read process data." << std::endl;
        CloseHandle(allSnapshot);
        return;
    }

    // 5. Clean up the snapshot handle
    CloseHandle(allSnapshot);
}

// Function to take user inputs
void getInputs(int arrival[], int burst[], int &n)
{
    std::cout << "Enter number of processes (Max 20): ";
    std::cin >> n;

    if (n > 20)
        n = 20;

    for (int i = 0; i < n; i++)
    {
        std::cout << "\n--- Process " << (i + 1) << " ---\n";

        std::cout << "Enter Arrival Time: ";
        std::cin >> arrival[i]; // Takes arrival first

        std::cout << "Enter Burst Time: ";
        std::cin >> burst[i]; // Then explicitly asks for burst
    }
    std::cout << "\nAll data saved successfully!\n";
}

// Function to display the data
void showData(int arrival[], int burst[], int n)
{
    if (n == 0)
    {
        std::cout << "No processes added yet! Choose Option 1 first.\n";
        return;
    }
    std::cout << "\nPID\tArrival\tBurst\n";
    std::cout << "-----------------------\n";
    for (int i = 0; i < n; i++)
    {
        std::cout << (i + 1) << "\t" << arrival[i] << "\t" << burst[i] << "\n";
    }
}

int main()
{
    bool running = true;

    int arrival[20] = {0};
    int burst[20] = {0};
    int totalProcesses = 0;

    while (running)
    {
        int userChoice = MenuPage();
        switch (userChoice)
        {
        case 1:
        {
            getInputs(arrival, burst, totalProcesses);
            break;
        }

        case 2:
        {
            showData(arrival, burst, totalProcesses);
            break;
        }
        case 3:
        {
            if (totalProcesses == 0)
            {
                std::cout << "Please add processes first!" << std::endl;
                break;
            }

            std::cout << "\nPID\tArrival\tBurst\tFinish Time" << std::endl;
            std::cout << "---------------------------------" << std::endl;

            int finishTime = 0;

            for (int i = 0; i < totalProcesses; i++)
            {
                // If the CPU finishes before the next process arrives,
                // it has to wait until that process arrives.
                if (finishTime < arrival[i])
                {
                    finishTime = arrival[i];
                }

                // Run the process by adding its burst time.
                finishTime = finishTime + burst[i];

                std::cout << (i + 1) << "\t" << arrival[i] << "\t" << burst[i] << "\t" << finishTime << std::endl;
            }

            break;
        }
        case 4:
        {
            CpuProcesses();
            break;
        }
        case 5:
        {
            std::cout << "Program exiting..." << std::endl;
            running = false;
            break;
        }
        default:
        {
            std::cout << "Invalid option. Please choose 1-5." << std::endl;
            break;
        }
        }
    }
    return 0;
}
