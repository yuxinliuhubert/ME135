import time
import machine
from machine import Pin, timer,ADC, I2C
from ulab import numpy as np

activated=0
timerFreq= 48000
i2cFreq=480000
windowsize=256
ADCsize = 4096
DACsize=2^16
ADCoffset = ADCsize/2
mu=0.01

FFL_PIN =1
FBL_PIN = 2
FFR_PIN = 3
FBR_PIN =4
FIVE_PIN =5
SCL_PIN = 8
SDA_PIN = 10
OUTA =0
OUTB = 1
OUTC = 2
OUTD = 3
DAC_ADDR=0x55
cmd=24

weigthsL= np.zeros(windowsize, dtype=float)
weightsR= np.zeros(windowsize, dtype=float)
FFLB= np.zeros(windowsize, dtype=float)
FBLB= np.zeros(windowsize, dtype=float)
FFRB= np.zeros(windowsize, dtype=float)
FBRB= np.zeros(windowsize, dtype=float)
FIVEB= np.zeros(windowsize, dtype=float)

i2c=I2C(0,scl=Pin(SCL_PIN),sda=Pin(SDA_PIN),freq=i2cFreq)

FFL=ADC(Pin(FFL_PIN))
FFL.atten(ADC.ATTN_11DB)
FBL=ADC(Pin(FBL_PIN))
FBL.atten(ADC.ATTN_11DB)
FFR=ADC(Pin(FFR_PIN))
FFR.atten(ADC.ATTN_11DB)
FBR=ADC(Pin(FBR_PIN))
FBR.atten(ADC.ATTN_11DB)
FIVE=ADC(Pin(FIVE_PIN))
FIVE.atten(ADC.ATTN_11DB)


def process(timer):
    global weigthsL
    global weightsR
    global FFLB
    global FBLB
    global FFRB
    global FBRB
    global FIVEB
    FFLB= np.roll(FFL,1)
    FBLB= np.roll(FBL,1)
    FFRB= np.roll(FFR,1)
    FBRB= np.roll(FBR,1)
    FIVEB= np.roll(FIVE,1)
    FFLB[0]=FFL.read() - ADCoffset
    FBLB[0]=FBL.read() - ADCoffset
    FFRB[0]=FFR.read() - ADCoffset
    FBRB[0]=FBR.read() - ADCoffset
    FIVE[0]=FIVE.read() - ADCoffset
    if(not activated):
        dL=FFLB[0]
        dR=FFRB[0]
    NLMSL, weightsL = signalProcess(dL,weightsL,FBLB)
    NLMSR,weightsR=signalProcess(dR,weigthsR, FBRB)
    i2c.writeto(DAC_ADDR,((cmd+OUTA).to_bytes()+NLMSL.to_bytes(2)))
    i2c.writeto(DAC_ADDR,((cmd+OUTB).to_bytes()+dL.to_bytes(2)))
    i2c.writeto(DAC_ADDR,((cmd+OUTC).to_bytes()+NLMSR.to_bytes(2)))
    i2c.writeto(DAC_ADDR,((cmd+OUTD).to_bytes()+dR.to_bytes(2)))

t1= Timer(0)
t1.init(mode=Timer.PERIODIC, freq=timerFreq, callback=process)

try:
    while(1):
        micsOut=[FFLB[0],FBLB[0],FFRB[0],FBRB[0],FIVE[0]] #this is the array of the most current audio data. FF is feed forward, FB is feed back L and R are right and left, and FIVE is the fifth mic
        Command=input('')
        #This is where the serial communication code goes. All that needs to be put here is something that changes the "activated" boolean to turn things on and off and maybe a pull request for the data.

except:
    t1.deinit()
    pass

def signalProcess(d, weigths, buffer):
    y=np.dot(weights,buffer)
    out=(y/ADCsize)*DACsize-1
    norm_buffer=np.linalg.norm(buffer)+1e-8
    weights += mu*(d-y)*buffer/norm_buffer
    return y, weights
