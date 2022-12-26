#include <iostream>
#include <string>
#include <fstream>

// Function to get your public facing IP address.
std::string whatismyip();

// Constants for finding your IP.
const std::string IP_FILE = "myip.txt";

// Global filename that stores IP addresses.
const std::string FILENAME = "publicIP.txt";
const std::string TEMP_FILENAME = "publicIP.txt.temp";

// Functions to read/store to the IP file.
bool isSame(std::string);
void storeToFile(std::string);
void logIP();

int main() {
    logIP();
}

std::string whatismyip() {
    // File to hold data read.
    std::fstream cmdOutput;
    std::string current;

    // Get the executables in the file path of the executable.
    std::system(("dig TXT +short o-o.myaddr.l.google.com @ns1.google.com | tr -d \'\"\"\' >"+IP_FILE).c_str());

    // Open the file and read the data.
    cmdOutput.open(IP_FILE, std::ios::in);

    getline(cmdOutput, current);
    // Delete the file.
    cmdOutput.close();
    std::system(("rm "+IP_FILE).c_str());

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
    outFile.open(TEMP_FILENAME, std::ios::out);

    // Add the new entry to the beginning of the file.
    outFile << newIP << std::endl;

    // Add all the entrys from the previous file to the new file.
    if (inFile) {
        while (getline(inFile, current)) {
            outFile << current << std::endl;
        }
    }
    
    inFile.close();
    outFile.close();

    // Move the temp file to the storage file.
    std::system(("mv "+TEMP_FILENAME+" "+FILENAME).c_str());
}

void logIP() {
    // Get your current public IP.
    std::string currentIP = whatismyip();

    if (!isSame(currentIP)) {
        storeToFile(currentIP);
    }
}

