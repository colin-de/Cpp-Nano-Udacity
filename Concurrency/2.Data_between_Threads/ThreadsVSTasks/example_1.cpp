#include <iostream>
#include <thread>
#include <future>
#include <cmath>
#include <memory>

/*the return type of the function is changed from void to double as the result of 
the computation will be channeled back to the main thread using a simple return.*/
double divideByNumber(double num, double denom)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work

    if (denom == 0)
        throw std::runtime_error("Exception from thread: Division by zero!");

    return num / denom;
}


int main()
{
    // use async to start a task
    double num = 42.0, denom = 2.0;
    // we need to replace the call to std::thread with std::async.
    std::future<double> ftr = std::async(divideByNumber, num, denom);
    /*The first change we are making is in the thread function: 
    We are removing the promise from the argument list as well as the try-catch block.*/

    // retrieve result within try-catch-block
    try
    {
        double result = ftr.get();
        std::cout << "Result = " << result << std::endl;
    }
    catch (std::runtime_error e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}