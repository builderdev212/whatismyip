#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <regex>

#ifdef __linux__
// Include for sleep on linux.
#include<unistd.h>
#endif

// Function to get the current time.
const std::string currentDateTime();

// Function to get your public facing IP address.
std::string whatismyip();

// Constants for finding your IP.
const std::string IP_FILE = "myip.txt";
const int NUM_OF_ATTEMPTS = 3;
const int PAUSE_TIME = 60;
const std::string ERROR_OUT = "Error: Unable to access the internet.";

// Global filename that stores IP addresses.
const std::string FILENAME = "publicIP.txt";

// Functions to read/store to the IP file.
bool isSame(std::string);
void storeToFile(std::string);
void logIP();

const int DATE_LEN = 22;

int main() {
    logIP();
}

const std::string currentDateTime() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "[%Y-%m-%d][%X]", &tstruct);

    return buf;
}

std::string whatismyip() {
    // File to hold data read.
    std::fstream cmdOutput;
    std::string current;
    int attemptNumber = 0;

    do {
        if (attemptNumber > 0) sleep(PAUSE_TIME);

        // Get the executables in the file path of the executable.
#ifdef __linux__
        std::system(("dig TXT +short o-o.myaddr.l.google.com @ns1.google.com | tr -d \'\"\"\' >"+IP_FILE).c_str());
#endif

        // Open the file and read the data.
        cmdOutput.open(IP_FILE, std::ios::in);

        // Get the output from the command.
        getline(cmdOutput, current);

        // Delete the file.
        cmdOutput.close();
        std::system(("rm "+IP_FILE).c_str());

        attemptNumber++;
    } while ((current == "") && (attemptNumber < NUM_OF_ATTEMPTS));

    if (current == "") return ERROR_OUT;

    return current;
}

bool isSame(std::string newIP) {
    // Get the current last IP address.
    std::fstream cmdOutput;
    std::string current;
    cmdOutput.open(FILENAME, std::ios::in);
    if (!cmdOutput) return false;
    getline(cmdOutput, current);
    cmdOutput.close();

    // Parse the IP out of the string.
    current.erase(0, DATE_LEN);

    // Compare the two strings.
    return (current == newIP);
}


void storeToFile(std::string newIP) {
    // File to store data in.
    std::fstream inFile;
    std::fstream outFile;
    std::string current;

    // Open the input and output files.
    inFile.open(FILENAME, std::ios::in);
    outFile.open(FILENAME+".temp", std::ios::out);

    // Add the new entry to the beginning of the file.
    outFile << currentDateTime() << newIP << std::endl;

    // Add all the entrys from the previous file to the new file.
    if (inFile) {
        while (getline(inFile, current)) {
            outFile << current << std::endl;
        }
    }

    // Move the temp file to the storage file.
    std::system(("mv "+FILENAME+".temp "+FILENAME).c_str());
}

void logIP() {
    // Get your current public IP.
    std::string currentIP = whatismyip();

    if (!isSame(currentIP) && !(currentIP == ERROR_OUT)) {
        storeToFile(currentIP);
    }
}
