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