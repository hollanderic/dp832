#!/usr/bin/env python

import dp832, getopt, sys


try:
  opts, args = getopt.getopt(sys.argv[1:], "c:v:i:s:b:", )
except getopt.GetoptError as err:
  print(str(erf))
  sys.exit(0)

channel = 1
voltage = 0
current = 0

dp = dp832.dp832()

for o, a in opts:
  print o
  print a
  if o == "-c":
    channel = int(a)
    if not channel in (1,2,3):
      print("Invalid channel")
      sys.exit(0)
  if o == "-v":
    voltage = float(a)
    dp.SetVoltage(channel, voltage)

  if o == "-i":
    current = float(a)
    dp.SetCurrent(channel, voltage)

  if o == "-s":
    if a == "on":
      dp.On(channel)
    if a == "off":
      dp.Off(channel)

