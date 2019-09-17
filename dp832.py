#!/usr/bin/env python
import time

class dp832:
  def __init__(self, fname="/dev/usbtmc3"):
    self.fd = open(fname,"w+")
    if not self.fd:
      print("Failed to open %s"%fname)


  def Write(self, command):
    self.fd.write(command)
  def Read(self):
    return self.fd.readline()


  def GetState(self, channel):
    self.Write(":OUTP:STAT? CH%d\n"%channel)
    return self.Read()


  def On(self, channel):
    self.Write(":OUTP:STAT CH%d,ON\n"%channel)
    self.GetState(channel)
  def Off(self, channel):
    self.Write(":OUTP:STAT CH%d,OFF\n"%channel)
    self.GetState(channel)
  def AllOn(self):
    for ch in range(1,4):
      self.On(ch)
  def AllOff(self):
    for ch in range(1,4):
      self.Off(ch)
  def Bounce(self, channel, delay=1.0):
    volt = self.GetVoltageSetPoint(channel)
    self.SetVoltage(channel, 0.0)
    time.sleep(delay)
    self.SetVoltage(channel, volt)


  def __SetLimit(self, channel, tag, limit):
    self.Write("SOUR%d:%s:IMM %6.3f\n"%(channel, tag, limit))
    self.GetState(channel)
  def SetVoltage(self, channel, voltage):
    self.__SetLimit(channel, "VOLT", voltage)
  def SetCurrent(self, channel, current):
    self.__SetLimit(channel, "CURR", current)


  def GetSetPoint(self, channel, tag):
    self.Write(":SOUR%d:%s?\n"%(channel, tag))
    return float(self.Read())
  def GetVoltageSetPoint(self, channel):
    return self.GetSetPoint(channel, "VOLT")
  def GetCurrentSetPoint(self, channel):
    return self.GetSetPoint(channel, "CURR")


  def Measure(self, channel, tag):
    self.Write(":MEAS:%s? CH%d\n"%(tag, channel))
    return float(self.Read())
  def MeasureVoltage(self, channel):
    return (self.Measure(channel, "VOLT"))
  def MeasureCurrent(self, channel):
    return (self.Measure(channel, "CURR"))
  def MeasurePower(self, channel):
    return (self.Measure(channel, "POWE"))
