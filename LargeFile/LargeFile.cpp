#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <atomic>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <chrono>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>


const std::string largeFileName = "large_numbers.txt";
const std::string sumFileName = "sum.txt";
const size_t numNumbers = 9000000; // Total number of random numbers to generate
const size_t numThreads = 4; // Number of threads to use
using namespace std::chrono;
static std::ifstream inputFile("C:\\Users\\Abhijeet\\source\\repos\\ParallismANDConcurency\\LargeFile\\LargeFile\\large_numbers.txt");
std::mutex mtx;
long long totalSum = 0;
long long totalSummulti = 0;


std::mutex fileMutex; // Mutex for protecting file writes
//std::atomic<long long> totalSum(0); // Atomic variable to store the sum of all numbers
/*
void generateNumbers(size_t start, size_t end) {
    std::ofstream largeFile(largeFileName, std::ios::app); // Open file in append mode
    if (!largeFile.is_open()) {
        std::cerr << "Error opening file for writing: " << largeFileName << std::endl;
        return;
    }

    long long localSum = 0; // Local sum for this thread
    std::srand(static_cast<unsigned>(std::time(0)) + start); // Seed random number generator

    for (size_t i = start; i < end; ++i) {
        int randomNum = std::rand() % 1000; // Generate random number between 0 and 999
        {
            std::lock_guard<std::mutex> lock(fileMutex);
            largeFile << randomNum << "\n"; // Write number to file
        }
        localSum += randomNum; // Accumulate local sum
    }

    totalSum += localSum; // Update total sum atomically
    largeFile.close();
}
/*void createLareFile()
{
    // Calculate the range of numbers each thread will generate
    size_t numbersPerThread = numNumbers / numThreads;
    std::vector<std::thread> threads;

    // Create and start threads
    for (size_t i = 0; i < numThreads; ++i) {
        size_t start = i * numbersPerThread;
        size_t end = (i == numThreads - 1) ? numNumbers : start + numbersPerThread;
        threads.emplace_back(generateNumbers, start, end);
    }

    // Wait for all threads to complete
    for (auto& thread : threads) {
        thread.join();
    }

    // Write the total sum to another file
    std::ofstream sumFile(sumFileName);
    if (!sumFile.is_open()) {
        std::cerr << "Error opening file for writing: " << sumFileName << std::endl;
        void;
    }

    sumFile << "Sum of numbers: " << totalSum.load() << std::endl;
    sumFile.close();

    std::cout << "Large file created and sum stored in " << sumFileName << std::endl;
}

*/


void sumChunkold(const std::vector<std::string>& lines, int start, int end) {
    long long sum = 0;
    for (int i = start; i <= end; ++i) {
        std::istringstream iss(lines[i]);
        long long number;
        while (iss >> number) {
            sum += number;
        }
    }

    std::lock_guard<std::mutex> guard(mtx);
    totalSummulti += sum;
}


void findSumSingleThread()
{
        std::ifstream inputFile("C:\\Users\\Abhijeet\\source\\repos\\ParallismANDConcurency\\LargeFile\\LargeFile\\large_numbers.txt");
        if (!inputFile.is_open()) {
            std::cerr << "Error opening file!" << std::endl;
            return ;
        }

        long long sum = 0;
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream iss(line);
            long long number;
            while (iss >> number) {
                sum += number;
            }
        }

        inputFile.close();

        std::cout << "The sum of numbers in the file is: " << sum << std::endl;

        return ;

}
void computeSumSinglethreaded()
{
    // Single-threaded multiplication
    auto startSingle = high_resolution_clock::now();
    findSumSingleThread();
    auto endSingle = high_resolution_clock::now();
    auto durationSingle = duration_cast<seconds>(endSingle - startSingle).count();
    std::cout << "Single-threaded Sum took " << durationSingle << " seconds" << std::endl;
}
int findSumMultithreaded()
{
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    int totalLines = lines.size();
    int numThreads = 6;// std::thread::hardware_concurrency();
    std::cout << "Number of thread " << numThreads<<std::endl;
    if (numThreads == 0) numThreads = 2; // Fallback to 2 threads if hardware_concurrency is 0
    int linesPerThread = totalLines / numThreads;
    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        
        int start = i * linesPerThread;
        
        int end = (i == numThreads - 1) ? (totalLines - 1) : (start + linesPerThread - 1);
        std::cout << "Start: " << i * start << " End: " << end << std::endl;
        threads.emplace_back(sumChunkold, std::ref(lines), start, end);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "The sum of numbers in the file is: " << totalSummulti << std::endl;

    return 0;
}


void computeSumMultithreaded()
{
    // Single-threaded multiplication
    auto startSingle = high_resolution_clock::now();
    findSumMultithreaded();
    auto endSingle = high_resolution_clock::now();
    auto durationSingle = duration_cast<seconds>(endSingle - startSingle).count();
    std::cout << "Multi-threaded Sum took " << durationSingle << " seconds" << std::endl;
}
void findSumStringStream()
{
    std::ifstream inputFile("C:\\Users\\Abhijeet\\source\\repos\\ParallismANDConcurency\\LargeFile\\LargeFile\\large_numbers.txt", std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return ;
    }

    // Read the entire file into a single string
    inputFile.seekg(0, std::ios::end);
    std::size_t fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    std::vector<char> buffer(fileSize);
    inputFile.read(buffer.data(), fileSize);
    inputFile.close();

    // Use a stringstream to process the entire file in memory
    std::stringstream ss(std::string(buffer.data(), buffer.size()));
    long long sum = 0;
    long long number;

    while (ss >> number) {
        sum += number;
    }

    std::cout << "stream ||| The sum of numbers in the file is: " << sum << std::endl;

    return ;
}
void computeSumSinglethreadedStringStream()
{
    // Single-threaded multiplication
    auto startSingle = high_resolution_clock::now();
    findSumStringStream();
    auto endSingle = high_resolution_clock::now();
    auto durationSingle = duration_cast<seconds>(endSingle - startSingle).count();
    std::cout << "Single-threaded string stream Sum took " << durationSingle << " seconds" << std::endl;
}

int main() {
    //computeSumSinglethreaded();
    computeSumSinglethreadedStringStream();
    computeSumMultithreaded();
    return 0;
}
