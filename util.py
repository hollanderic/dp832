#!/usr/bin/env python

#import Tkinter as tk
import tkinter as tk
import dp832
import sys


class Plotter(tk.Canvas):
  def __init__(self, master = None, w=100, h=100):
    tk.Canvas.__init__(self, master, width=w, height=h, background="#101010")
    self.dp = master.dp
    self.bind("<Configure>", self.onResize)
    self.place(relx=0, rely=0)
    self.width = w
    self.height = h
  def update(self, state=None):
    self.create_line(0, self.height-8, self.width, self.height-8, fill="white")
    for y in range(self.height - 28, 0, -20):
      print(y)
      self.create_line(0, y, self.width, y, fill="grey", dash=(2,2))
    if state:
      pass


  def onResize(self, event):
    self.width = self.winfo_reqwidth()
    self.height = self.winfo_reqheight()
    self.update()

'''
On off button for toggling a psu channel
'''
class OnOff(tk.Button):
  def __init__(self, master):
    tk.Button.__init__(self, master)
    self["text"] = "On\nOff"
    self["command"] = self.Toggle
    self.setColor()

  def setColor(self, state = None):
    if state == None:
      state = self.master.dp.GetState(self.master.channel)
    if state["State"] == "ON":
      self["background"] = "green"
      self["activebackground"] = "#10c910"
    else:
      self["background"] = "#d9d9d9"
      self["activebackground"] = "#e0e0e0"

  def Toggle(self):
    state = self.master.dp.GetState(self.master.channel)
    if state["State"] == "ON":
      self.master.dp.Off(self.master.channel)
    else:
      self.master.dp.On(self.master.channel)
    self.setColor()

  def update(self):
    self.setColor()

#Represents one channels worth of controls/monitors
class PsuOutput(tk.Frame):
  def __init__(self, dp, master = None, ch=1, w=200, h=200):
    tk.Frame.__init__(self, master, width=w, height=h)
    self.width = w
    self.height = h
    self["background"]="grey"
    self.dp = dp
    self.channel = ch
    self.ON = OnOff(self)
    self.ON.place(relx=0, rely=0.7)

    self.PLOT = Plotter(self, self.width-8, self.height*.7)
    self.place(relx=0.33*(ch-1), rely=0)
  def resize(self):
    self.width = self.master.winfo_reqwidth()/3
    self.height = self.winfo_reqheight()
    self.config(width = self.width, height=self.height)
    self.PLOT.config(width = self.width - 8)


class Application(tk.Frame):
  def createThis(self):
    self.QUIT = tk.Button(self)
    self.QUIT["text"] = "Quit"
    self.QUIT["command"] = self.quit
    self.QUIT.place(relx=0.9, rely=0.9)
    self.out1 = PsuOutput(self.dp, self, ch=1, w = self['width']/3)
    self.out2 = PsuOutput(self.dp, self, ch=2, w = self['width']/3)
    self.out3 = PsuOutput(self.dp, self, ch=3, w = self['width']/3)

  def __init__(self, dp, master =  None):
    tk.Frame.__init__(self, master, width=800, height=600)

    self.dp = dp
    self.createThis()
    self.pack_propagate(0)
    self.pack(fill="both", expand=True)
    self.bind("<Configure>", self.onResize)
  def onResize(self, event):
    self.config(width = event.width, height=event.height)
    self.out1.resize()
    self.out2.resize()
    self.out3.resize()

running=True
def destroy_callback():
  global running
  print("destroy cb")
  running = False
  root.destroy()

root = tk.Tk()
root.protocol("WM_DELETE_WINDOW", destroy_callback)
#dp = dp832.dp832(ip="192.168.8d6.79")
dp = dp832.dp832()

if not dp.isConnected():
  print("Could not open connection to Rigol")
  sys.exit(-1)
print(dp.GetState(1))
app = Application(dp, master = root)
root.title("Rigol DP832")
root.mainloop()
if running:
  print("destroyend")
  root.destroy()

