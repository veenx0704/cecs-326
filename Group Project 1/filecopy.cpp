// Montserrat Alonso
// Taiki Tsukahara
// CECS-326 Operating Systems
// Group Project 1: Warm up of Interprocess Comunication
// Due:  2/24/2024

#include <fstream>  // Reading and writing files
#include <iostream>
#include <stdexcept>  // Provides standard exceptions
#include <string>
#include <unistd.h>  // Provides operating system features like fork, pipe, close
using namespace std;

int main(int argc, char* argv[]) {
    //checks if user provided 2 additional arguments w/ program name
    if (argc != 3) {
        //error message:
        cerr << "Usage: " << argv[0] << " <source_file> <destination_file>\n";
        return 1;
    }

    try {
        // Open input file to get its size
      //needed for memory allocation
        ifstream inputFile(argv[1], ios::binary);
        if (!inputFile.is_open()) {
            throw runtime_error("Unable to open source file: " + string(argv[1]));
        }
        inputFile.seekg(0, ios::end);          // Seek to the end of the file
        streampos fileSize = inputFile.tellg();  //get current position of file size
        inputFile.seekg(0, ios::beg);  // Rewind file pointer to the beginning
        inputFile.close();              //close file

        // resize the string to the file size so it can be read into memory efficiently
        string message;
        message.resize(fileSize);

        // Read source file
        inputFile.open(argv[1], ios::binary);
        if (!inputFile.is_open()) {
            throw runtime_error("Unable to reopen source file: " + string(argv[1]));
        }
        inputFile.read(&message[0], fileSize);
        inputFile.close();

        // Create pipe
        int pipefd[2];            //pipefd[0] is read end, pipefd[1] is write end
        if (pipe(pipefd) == -1) {  //if the creation of the pipe fails:
            perror("pipe");      //prints error message
            return 1;
        }

        // Create child process
        pid_t child_pid = fork();    //indicates parent process: process ID of child process, Child process: 0
        if (child_pid == -1) {  //checks if fork failed, then error message
            perror("fork");
            return 1;
        }
        else if (child_pid == 0) {  // Child process
            close(pipefd[0]);  // Close read end of the pipe
            write(pipefd[1], message.c_str(), fileSize);  // Write end to pipe with specified bytes to write.
            close(pipefd[1]);  // Close write end of the pipe
            exit(EXIT_SUCCESS);
        }
        else {  // Parent process, should be when child_pid != 0 and is the pid
            close(pipefd[1]);  // Close write end of the pipe

            // Open destination file
            ofstream outputFile(argv[2], ios::binary);
            if (!outputFile) {
                throw runtime_error("Unable to open destination file: " + string(argv[2]));
            }

            // Read from pipe and write to destination file
            char buffer[BUFSIZ];  //an array of characters set to buffer size
            ssize_t bytes_read;   //number of bytes read from the pipe,successful: non negative value representing number of bytes read, error: -1 
            while ((bytes_read = read(pipefd[0], buffer, BUFSIZ)) > 0) {  //if == 0 then pipe is empty,reached the end. if -1, then error
                outputFile.write(buffer, bytes_read);
            }
            close(pipefd[0]);  // Close read end of the pipe
            outputFile.close();  // Close destination file

            cout << "File successfully copied from " << argv[1] << " to " << argv[2] << endl;
        }
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
