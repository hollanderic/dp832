#include <string.h>
#include <cassert>
#include "dp832.h"
#include <unistd.h>
using namespace std;



dp830::dp830(const char *devname) {
    fhandle_.open(devname,std::fstream::in | std::fstream::out | std::fstream::app);
}

dp830::~dp830(){fhandle_.close();}

int dp830::write(const char *command) {
    assert(fhandle_);
    int len = strlen(command);
    fhandle_.write(command,len);
    //printf("write: -->%s<--\n",command);
    fhandle_.flush();
    return len;
}

int dp830::read(char *buf, int len){
    assert(fhandle_);
    if (!buf) return 0;
    memset(buf,0,len);
    fhandle_.getline(buf,len);
    return fhandle_.gcount();
}

int dp830::GetState(int channel){
    char buf[40];
    char buf2[40];
    snprintf(buf,sizeof(buf),":OUTP:STAT? CH%d",channel);
    write(buf);
    read(buf2,sizeof(buf2));
    return strcmp(buf2,"OFF")?1:0;
}

int dp830::On(int channel) {
    char buf[40];
    snprintf(buf,sizeof(buf),":OUTP:STAT CH%d,ON",channel);
    return write(buf);
}

int dp830::AllOn() {
    On(1);
    On(2);
    On(3);
    return 0;
}

int dp830::Off(int channel) {
    char buf[40];
    snprintf(buf,sizeof(buf),":OUTP CH%d,OFF",channel);
    return write(buf);
}
int dp830::AllOff() {
    Off(1);
    Off(2);
    Off(3);
    return 0;
}

int dp830::SetVoltage(int channel, double limit){
    char buf[40];
    snprintf(buf,sizeof(buf),":SOUR%d:VOLT:IMM %6.3f",channel,limit);
    return write(buf);
}

int dp830::SetCurrent(int channel, double limit){
    char buf[40];
    snprintf(buf,sizeof(buf),":SOUR%d:CURR:IMM %6.3f",channel,limit);
    return write(buf);
}

double dp830::MeasureVoltage(int channel) {
    char buf[40];
    char buf2[40];
    snprintf(buf,sizeof(buf),":MEAS:VOLT? CH%d",channel);
    write(buf);
    read(buf2,sizeof(buf2));
    return std::stod(buf2,NULL);
}
double dp830::MeasureCurrent(int channel) {
    char buf[40];
    char buf2[40];
    snprintf(buf,sizeof(buf),":MEAS:CURR? CH%d",channel);
    write(buf);
    read(buf2,sizeof(buf2));
    return std::stod(buf2,NULL);
}
double dp830::MeasurePower(int channel) {
    char buf[40];
    char buf2[40];
    snprintf(buf,sizeof(buf),":MEAS:POWE? CH%d",channel);
    write(buf);
    read(buf2,sizeof(buf2));
    return std::stod(buf2,NULL);
}