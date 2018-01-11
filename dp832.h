
#include <iostream>
#include <fstream>

using namespace std;

class dp830 {
public:
    dp830(const char *devname);
    dp830() : dp830("/dev/usbtmc1") {};
    ~dp830();

    int On(int channel);
    int AllOn();
    int Off(int channel);
    int AllOff();

    int GetState(int channel);

    void Reset() { write("*RST"); };

    int SetCurrent(int channel, double limit);
    int SetVoltage(int channel, double limit);

    double MeasureVoltage(int channel);
    double MeasureCurrent(int channel);
    double MeasurePower(int channel);

    int write(const char *command);
    int read(char *buf, int len);
private:
    fstream fhandle_;
};