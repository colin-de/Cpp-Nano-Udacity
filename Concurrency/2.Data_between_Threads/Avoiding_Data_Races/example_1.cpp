#include <iostream>
#include <thread>
#include <future>

// In this example, one safe way of passing data to a thread would be to carefully synchronize 
// the two threads using either join() or the promise-future concept that can guarantee the availability 
// of a result. Data races are always to be avoided. Even if nothing bad seems to happen, 
// they are a bug and should always be treated as such. Another possible solution for the above example 
// would be to make a copy of the original argument and pass the copy to the thread, thereby preventing the data race.

class Vehicle
{
public:
    //default constructor
    Vehicle() : _id(0)
    {
        std::cout << "Vehicle #" << _id << " Default constructor called" << std::endl;
    }

    //initializing constructor
    Vehicle(int id) : _id(id)
    {
        std::cout << "Vehicle #" << _id << " Initializing constructor called" << std::endl;
    }

    // setter and getter
    void setID(int id) { _id = id; }
    int getID() { return _id; }

private:
    int _id;
};

int main()
{
    // create instances of class Vehicle
    Vehicle v0; // default constructor
    Vehicle v1(1); // initializing constructor

    // read and write name in different threads (which one of the above creates a data race?)
    /* an instance of the proprietary class Vehicle is created and 
    passed to a thread by value, thus making a copy of it. */
    std::future<void> ftr = std::async([](Vehicle v) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate work
        v.setID(2);
    }, v0);

    v0.setID(3);

    ftr.wait();
    std::cout << "Vehicle #" << v0.getID() << std::endl;

    return 0;
}