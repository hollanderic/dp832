#!/usr/bin/env python
import time, socket
class dp832_mock:
  def __init__(self):
    pass
  def GetState(self, channel):
    return {"State":"OFF", "VLimit":10.2, "ILimit":2.2, "V":1.1, "I":2.2, "P":2.42}

class dp832:
  def __init__(self, fname="/dev/usbtmc3", ip=None, port=5555):
    self.ip = ip
    self.connected = False
    if self.ip:
      try:
        self.clientsock = socket.socket(family=socket.AF_INET, type=socket.SOCK_STREAM)
        self.clientsock.connect((self.ip, 5555))
        print("connection made to %s"%self.ip)
        self.connected = True
      except socket.error as msg:
        self.clientsock = None
        self.connected = False
    else:
      try:
        self.fd = open(fname,"w+", buffering=1)
        self.connected = True
      except IOError as e:
        self.fd = None
        print("Could not open %s -- %s"%(fname,e.strerror))
  def isConnected(self):
    return self.connected

  def Write(self, command):
    if self.ip:
      self.clientsock.send(command)
    else:
      self.fd.write(command)
  def Read(self):
    if self.ip:
      data = self.clientsock.recv(1024)
      return data
    else:
      return self.fd.readline()

  def GetState(self, channel):
    self.Write(":OUTP:STAT? CH%d\n"%channel)
    state = self.Read().strip()
    retval = {}
    retval["State"] = state
    retval["VLimit"] = self.GetVoltageSetPoint(channel)
    retval["ILimit"] = self.GetCurrentSetPoint(channel)
    retval["V"] = self.MeasureVoltage(channel)
    retval["I"] = self.MeasureCurrent(channel)
    retval["P"] = self.MeasurePower(channel)
    return retval

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
