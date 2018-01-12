#include <unistd.h>
#include "dp832.h"

/* psutil - utility for controlling Rigol dp8xx series power supplies

    Arguments are processing in order that they appear on command line. So
    If you want to perform a sequence of operations on different channels
    they can all be done with a long string of (potentially repeating) flags
    and parameters.

    c - Sets the channel to apply operations to.  Default is 1 (valid 1-3)

    v - Sets the voltage setpoint

    i - Sets the current setpoint

    s - Sets the state of the output (0-off, not 0-on)

    b - Bounce the channel.  Parameter is the off period in seconds.  Will set
        the channel output to 0.0V for that many seconds.

    TODO - better range checking on parameters, but this will be done in the
            underlying dp830 class and handled via return parameters.

*/

int main (int argc, char** argv) {
    int opt;
    int channel=1;
    double voltage=0.0;
    double current=0.0;
    bool state=false;
    bool setvoltage=false;
    bool setcurrent=false;
    bool setstate=false;

    dp830 psu;
    //psu.Reset();

    while ((opt = getopt(argc, argv, "c:v:i:s:b:")) != -1) {
        switch (opt) {
            case 'c':
                channel = atoi(optarg);
                if ((channel < 1) || (channel > 3))
                    return -1;
            case 'v':
                voltage = std::stod(optarg);
                psu.SetVoltage(channel,voltage);
                setvoltage=true;
                break;
            case 'i':
                current = std::stod(optarg);
                psu.SetCurrent(channel,current);
                setcurrent=true;
                break;
            case 's':
                state = !(atoi(optarg) == 0);
                if (state)
                    psu.On(channel);
                else
                    psu.Off(channel);
                setstate=true;
                break;
            case 'b':
                psu.Bounce(channel,std::stod(optarg));
                break;
            default:
                break;
        }
    }
    usleep(1*1e6);



    printf("Voltage = %f\n",psu.MeasureVoltage(channel));
    printf("Current = %f\n",psu.MeasureCurrent(channel));
    printf("Power   = %f\n",psu.MeasurePower(channel));
    printf("Volt sp   = %f\n",psu.GetVoltageSetPoint(channel));
    printf("Curr sp   = %f\n",psu.GetCurrentSetPoint(channel));

    //Test Bounce feature - used to bounce a target
    //usleep(3*1e6);
    //psu.Bounce(1,2.5);

  return 0;
}