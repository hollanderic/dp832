
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

  return 0;
}