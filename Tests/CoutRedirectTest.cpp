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

#include <CRedirect.h>
#include <iostream>
#include <sstream>

static std::stringstream testBuffer;

class CoutObserver : public StreamObserver {
public:
    void update(const std::string& output) override {
        testBuffer << output;
    }
};

class ClogObserver : public StreamObserver {
public:
    void update(const std::string& output) override {
        testBuffer << output;
    }
};

class CerrObserver : public StreamObserver {
public:
    void update(const std::string& output) override {
        testBuffer << output;
    }
};
    
    
/**
 * @brief Test function for CoutRedirect
 */
int test001() {
    std::string testString = "This is a test string.";
    CoutObserver observer;    
    
    {
        CoutRedirect redirect;
        CoutRedirect::attach(&observer);

        std::cout << testString << std::endl;
    }

    std::string expectedOutput = testString;
    std::string actualOutput = testBuffer.str();
    
    return expectedOutput == actualOutput;
}

/**
 * @brief Test function for ClogRedirect
 */
int test002() {
    std::string testString = "This is a test string.";
    ClogObserver observer;    
    
    {
        ClogRedirect redirect;
        ClogRedirect::attach(&observer);

        std::clog << testString << std::endl;
    }

    std::string expectedOutput = testString;
    std::string actualOutput = testBuffer.str();
    
    return expectedOutput == actualOutput;
}

/**
 * @brief Test function for CerrRedirect
 */
int test003() {
    std::string testString = "This is a test string.";
    CerrObserver observer;    
    
    {
        CerrRedirect redirect;
        CerrRedirect::attach(&observer);

        std::cerr << testString << std::endl;
    }

    std::string expectedOutput = testString;
    std::string actualOutput = testBuffer.str();
    
    return expectedOutput == actualOutput;
}

int parseArguments(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <test_number>" << std::endl;
        return -1;
    }
    
    int testNumber = std::atoi(argv[1]);
    return testNumber;
}

int main(int argc, char** argv) {
    
    int testNumber = parseArguments(argc, argv);
    if (testNumber < 0) {
        return -1;
    }

    switch (testNumber) {
        case 1:
            return test001();
        case 2:
            return test002();
        case 3:
            return test003();

        default:
            std::cerr << "Unknown test number: " << testNumber << std::endl;
            return -1;
    }
}