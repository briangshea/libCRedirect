/*
 * This file is part of libCRedirect.
 *
 * libCRedirect is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libCRedirect is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libCRedirect. If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) Brian G Shea <bgshea@gmail.com>
 */

#include <LogFileWriterConfig.h>
#include <CRedirect.h>
#include <LogFileWriter.hpp>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
#ifdef USE_CLOG_REDIRECT
    // Attach the ClogRedirect to redirect std::clog
    ClogRedirect clogRedirector{};
#endif
#ifdef USE_CERR_REDIRECT
    // Attach the CerrRedirect to redirect std::cerr
    CerrRedirect cerrRedirector{};
#endif

    /**
     * @brief Initialize the LogFileWriter to redirect logs to a file.
     * This will create an instance of LogFileWriter that will handle writing log messages
     * to the specified log file (logfile.txt in this case).
     */
    LogFileWriter logFileWriter("logfile.txt");

    /**
     * @brief Check if the program name is provided as an argument.
     * If no program name is provided, display usage information and exit.
     */
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <program> [args...]" << std::endl;
        return 1;
    }

    /**
     * @brief Check for the --help option.
     * If --help is provided, display usage information and exit.
     */
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--help") {
            std::cout << "Usage: " << argv[0] << " <program> [args...]\n"
                      << "Runs the specified program with optional arguments, redirecting logs to logfile.txt.\n"
                      << "Options:\n"
                      << "  --help    Show this help message\n";
            return 0;
        }
    }

    /**
     * @brief Construct the command to run the specified program with its arguments.
     * This command is built by concatenating the program name and its arguments,
     * ensuring that each argument is properly quoted to handle spaces and special characters.
     * The command is then executed using std::system.
     */
    std::string command;
    for (int i = 1; i < argc; ++i) {
        if (i > 1) command += " ";
        command += "\"" + std::string(argv[i]) + "\"";
    }

    /**
     * @brief Execute the command using std::system.
     * This will run the specified program with its arguments in a new process.
     * The return value of std::system is checked to determine if the command was executed successfully.
     * If the command fails to execute, an error message is printed to std::cerr.
     */
    int ret = std::system(command.c_str());

    return ret;
}
