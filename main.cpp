#include <unistd.h>
#include "dp832.h"
#include <string>
#include <chrono>

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

    d - Path to the power supply's USB device, defaults to /dev/usbtmc1

    x - extra control mode - measurements -m and wait times -w can be specified in sequence

    w - wait time in ms

    m - measure selected channel

      e.g. ./psutil -d /dev/usbtmc0 -x -c1 -m -w500 -m -v4.5 -w200 -m
      select ch1; measure ch1; wait 500ms; measure ch1; set voltage to 4.5V; etc

    TODO - better range checking on parameters, but this will be done in the
            underlying dp830 class and handled via return parameters.

*/

const std::string kValidArgs = "c:v:i:s:b:d:xmw:";
const std::string kDefaultUSBDevicePath = "/dev/usbtmc1";

using namespace std::chrono;

// Finds and returns the device path argument from the argument list.
// Returns kDefaultUSBDevicePath if none is provided.
const std::string getDevicePathArg(int argc, char** argv) {
    char opt;
    std::string result = kDefaultUSBDevicePath;
    while ((opt = getopt(argc, argv, kValidArgs.c_str())) >= 0) {
        if (opt == 'd') {
            result = string(optarg);
            break;
        }
    }

    // Reset option parser.
    optind = 1; 

    return result;
}

bool isExtraMode(int argc, char** argv) {
    char opt;
    bool result = false;
    while ((opt = getopt(argc, argv, kValidArgs.c_str())) >= 0) {
        if (opt == 'x') {
            result = true;
            break;
        }
    }

    // Reset option parser.
    optind = 1; 

    return result;
}

int main (int argc, char** argv) {
    int opt;
    int channel=1;
    double voltage=0.0;
    double current=0.0;
    bool state=false;
    bool setvoltage=false;
    bool setcurrent=false;
    bool setstate=false;
    bool extra = isExtraMode(argc, argv);

    const std::string psuDevicePath = getDevicePathArg(argc, argv);
    dp830 psu(psuDevicePath.c_str());

    while ((opt = getopt(argc, argv, kValidArgs.c_str())) != -1) {
        switch (opt) {
            case 'c':
                channel = atoi(optarg);
                if ((channel < 1) || (channel > 3)) {
                   fprintf(stderr, "Channel must be in range 1..3\n");
                   return -1;
                }
                break;
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
            case 'm':
                if (extra) {
                  printf("t=%lu,c=%d,s=%d,vs=%0.03f,is=%0.03f,v=%0.03f,i=%0.03f,p=%0.03f\n", 
                    (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count(),
                    channel,
                    psu.GetState(channel),
                    psu.GetVoltageSetPoint(channel),
                    psu.GetCurrentSetPoint(channel),
                    psu.MeasureVoltage(channel),
                    psu.MeasureCurrent(channel),
                    psu.MeasurePower(channel));
                } else { 
                    fprintf(stderr, "Can't use -m in legacy mode, add -x flag\n");
                }
                break;
            case 'w': // ms delay
                if (extra) {
                    usleep(1000*std::stod(optarg));
                } else { 
                    fprintf(stderr, "Can't use -w in legacy mode, add -x flag\n");
                }
                break;
            default:
                break;
        }
    }

    if (!extra) {
      usleep(1*1e6);

      printf("Voltage = %f\n",psu.MeasureVoltage(channel));
      printf("Current = %f\n",psu.MeasureCurrent(channel));
      printf("Power   = %f\n",psu.MeasurePower(channel));
      printf("Volt sp   = %f\n",psu.GetVoltageSetPoint(channel));
      printf("Curr sp   = %f\n",psu.GetCurrentSetPoint(channel));
    }
    //Test Bounce feature - used to bounce a target
    //usleep(3*1e6);
    //psu.Bounce(1,2.5);

  return 0;
}
