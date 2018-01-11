#include <unistd.h>

#include "dp832.h"

int main () {
    dp830 psu;
    psu.Reset();

    psu.SetVoltage(1,3.54);
    psu.SetCurrent(1,0.2);
    psu.On(1);

    printf("Voltage = %f\n",psu.MeasureVoltage(1));
    printf("Current = %f\n",psu.MeasureCurrent(1));
    printf("Power   = %f\n",psu.MeasurePower(1));
    printf("Volt sp   = %f\n",psu.GetVoltageSetPoint(1));
    printf("Curr sp   = %f\n",psu.GetCurrentSetPoint(1));

    //Test Bounce feature - used to bounce a target
    usleep(3*1e6);
    psu.Bounce(1,2.5);

  return 0;
}