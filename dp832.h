
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

    void Reset() { write_("*RST"); };

    int SetCurrent(int channel, double limit);
    int SetVoltage(int channel, double limit);
    double GetCurrentSetPoint(int channel);
    double GetVoltageSetPoint(int channel);

    int Bounce(int channel, double delay);

    double MeasureVoltage(int channel);
    double MeasureCurrent(int channel);
    double MeasurePower(int channel);

private:

    int write_(const char *command);
    int read_(char *buf, int len);

    int SetLimit_(int channel, const char *tag, double limit);
    double Measure_(int channel, const char *tag);
    double GetSetPoint_(int channel, const char *tag);
    fstream fhandle_;
};