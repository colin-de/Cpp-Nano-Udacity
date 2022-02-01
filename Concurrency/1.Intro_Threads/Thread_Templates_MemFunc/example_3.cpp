#include <iostream>
#include <thread>
#include <string>

void printName(std::string &name, int waitTime)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(waitTime));
    name += " (from Thread)";
    std::cout << name << std::endl;
}

int main()
{
    std::string name("MyThread");
    
    // When passing the string variable name to the thread function, 
    // we need to explicitly mark it as a reference, so the compiler will treat it as such. 
    // This can be done by using the std::ref function. 
    // In the console output it becomes clear that the string has been successfully modified 
    // within the thread function before being passed to main.
    
    // starting thread
    std::thread t(printName, std::ref(name), 50);

    // wait for thread before returning
    t.join();

    // print name from main
    name += " (from Main)";
    std::cout << name << std::endl;

    return 0;
}