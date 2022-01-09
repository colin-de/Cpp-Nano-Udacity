#include <iostream>
#include <thread>

// Detach

void threadFunction()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate work
    std::cout << "Finished work in thread\n"; 
}

// int main()
// {
//     // create thread
//     std::thread t(threadFunction);

//     // detach thread and continue with main
//     t.detach();

//     // do something in main()
//     std::this_thread::sleep_for(std::chrono::milliseconds(50)); // simulate work
//     std::cout << "Finished work in main\n";

//     return 0;
// }

#include <iostream>
#include <thread>

void threadFunctionEven()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // simulate work
    std::cout << "Even thread\n";
}

/* Student Task START */
void threadFunctionOdd()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(1)); // simulate work
    std::cout << "Odd thread\n";
}
/* Student Task END */

int main()
{
    /* Student Task START */
    for (int i = 0; i < 6; ++i)
    {
        if (i % 2 == 0)
        {
            std::thread t(threadFunctionEven);
            t.detach();
        }
        else
        {
            std::thread t(threadFunctionOdd);
            t.detach();
        }
    }
    /* Student Task END */

    // ensure that main does not return before the threads are finished
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // simulate work

    std::cout << "End of main is reached" << std::endl;
    return 0;
}