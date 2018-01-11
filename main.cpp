
#include "dp832.h"

int main () {
    dp830 psu;
    psu.Reset();

    printf("getstate=%d\n",psu.GetState(1));
    //psu.GetState(2);
    psu.SetVoltage(1,3.54);
    //psu.write(":MONI ON");
    psu.On(1);

    //psu.write(":APPL CH1,5.55,1.2");

    printf("getstate=%d\n",psu.GetState(1));
    //usleep(5*1e6);

//psu.GetState(1);

//  myfile.open ("/dev/usbtmc1");
//  myfile << ":OUTP CH1,ON";
//  myfile.close();
  return 0;
}