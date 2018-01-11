#include <string.h>
#include <cassert>
#include "dp832.h"
#include <unistd.h>
using namespace std;



dp830::dp830(const char *devname) {
    fhandle_.open(devname,std::fstream::in | std::fstream::out | std::fstream::app);
}

dp830::~dp830(){fhandle_.close();}

int dp830::write_(const char *command) {
    assert(fhandle_);
    int len = strlen(command);
    fhandle_.write(command,len);
    //printf("write: -->%s<--\n",command);
    fhandle_.flush();
    return len;
}

int dp830::read_(char *buf, int len){
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
    write_(buf);
    read_(buf2,sizeof(buf2));
    return strcmp(buf2,"OFF")?1:0;
}

int dp830::On(int channel) {
    char buf[40];
    snprintf(buf,sizeof(buf),":OUTP:STAT CH%d,ON",channel);
    return write_(buf);
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
    return write_(buf);
}
int dp830::AllOff() {
    Off(1);
    Off(2);
    Off(3);
    return 0;
}


// Routines for setting operating parameters.  Note these are not
//  overvoltage/overcurrent protections.
int dp830::SetLimit_(int channel, const char *tag, double limit){
    char buf[40];
    snprintf(buf,sizeof(buf),":SOUR%d:%s:IMM %6.3f",channel,tag,limit);
    return write_(buf);
}
int dp830::SetVoltage(int channel, double limit){
    return SetLimit_(channel,"VOLT",limit);
}
int dp830::SetCurrent(int channel, double limit){
    return SetLimit_(channel,"CURR",limit);
}

double dp830::GetSetPoint_(int channel, const char *tag) {
    char buf[40];
    char buf2[40];
    snprintf(buf,sizeof(buf),":SOUR%d:%s?",channel,tag);
    write_(buf);
    read_(buf2,sizeof(buf2));
    return std::stod(buf2,NULL);
}
double dp830::GetVoltageSetPoint(int channel) {
    return GetSetPoint_(channel, "VOLT");
}
double dp830::GetCurrentSetPoint(int channel) {
    return GetSetPoint_(channel, "CURR");
}
// Measurement routines
double dp830::Measure_(int channel, const char *tag) {
    char buf[40];
    char buf2[40];
    snprintf(buf,sizeof(buf),":MEAS:%s? CH%d",tag,channel);
    write_(buf);
    read_(buf2,sizeof(buf2));
    return std::stod(buf2,NULL);
}
double dp830::MeasureVoltage(int channel) {
    return Measure_(channel,"VOLT");
}
double dp830::MeasureCurrent(int channel) {
    return Measure_(channel,"CURR");
}
double dp830::MeasurePower(int channel) {
    return Measure_(channel,"POWE");
}

//Bounce will set the output to 0.0V for delay seconds
//  Set to 0.0 instead of turning the output off so that
//    it will present a low impendence and help discharge
//    the target (if the circuit is designed in such a manner
//    to make this useful)
int dp830::Bounce(int channel, double delay) {
    double volt = GetVoltageSetPoint(channel);
    SetVoltage(channel, 0.0);
    usleep(delay*1e6);
    SetVoltage(channel, volt);
    return 0;
}