'''
Created on Jun 28, 2017

@author: make ma
'''
from AudioSrc import AudioSrc
from threading import Thread
from MsgHandler import MsgHandler, Message, MsgConst
from VoiceCmd import VoiceCmd
from OlamiNlp import OlamiNlp
#import uartapi
import time
import json 
import bluetooth

class SpeechProcess(Thread):
    '''
    classdocs
    '''

    def __init__(self):
        Thread.__init__(self, name = "SpeechProcess")
        
        
    def init(self, handler):
        self.handler = handler  
        self.audioSrc = AudioSrc()        
        self.audioSrc.startRecord()  
        self.voiceCmd = VoiceCmd()
        self.voiceCmd.init(self.audioSrc)
        self.setDaemon(True)
        self.nlp = OlamiNlp()
        self.nlp.setLocalization("https://tw.olami.ai/cloudservice/api")
        self.nlp.setAuthorization("a0d832f393f84d9a915ad5c8ea1bb692", "11228d6aca034ae88e17fcc4a6b92214")     
        self.start()      
        return True
    
    def destroy(self):
        self.needStop = True
        self.join(2000)        
        self.audioSrc.stopRecord()        
        self.voiceCmd.destroy()
        
    def wakeupNow(self):
        self.voiceCmd.cancelDetect()
             
        
    def run(self):
        self.needStop = False        
    
        bd_addr = '98:D3:32:30:A6:53'
        port = 1
        sock = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
        sock.connect((bd_addr,port))
        #uart = uartapi.UartAPISample()
        #uart.openUart()
        while not self.needStop:
            #uart.RunGREEN()
            wakeup = self.voiceCmd.startDetect()   
            self.handler.sendEmptyMessage(MsgConst.MSG_FORCE_STOP_TTS)
            if wakeup != VoiceCmd.STATE_STOPPED:  
                    
                if wakeup == VoiceCmd.STATE_DETECTED_KEY:
                    #uart.RunROTATE()
                    msg = self.handler.obtainMessage1(MsgConst.MSG_NORMAL_TTS_PLAY)
                    msg.obj = "在"                
                    self.handler.sendMessage(msg)
                    time.sleep(0.5)
                    self.audioSrc.clearData() 
                nlpResult = self.nlp.getNlpResult(self.audioSrc) #json about the specch from olami api
                print(nlpResult)
                data = nlpResult
                #uart.RunRED()
                if data != None:
                    if data[0]['type'] == 'guide_dog':
                        data = data[0]['semantic'][0] ['modifier'] # analzye data and find the key word
                        print(data)
                        if data == ['go_toward']: # if the key word is go_toward then tell Arduino to go forward 
                            print('Got go_toward')
                            sock.send('0') #send a bit to arduino via bluetooth and controll it 
                        elif data == ['go_backward']: # if the key word is go_backward then tell Arduino to go backward 
                            print('Got go_backward')
                            sock.send('1') #send a bit to arduino via bluetooth and controll it 
                        elif data == ['go_right']: # if the key word is go_right then tell Arduino to go right 
                            print('Got go_right')
                            sock.send('2') #send a bit to arduino via bluetooth and controll it 
                        elif data == ['go_left']: # if the key word is go_left then tell Arduino to go left
                            print('Got go_left')
                            sock.send('3') #send a bit to arduino via bluetooth and controll it 
                        elif data == ['slow']: # if the key word is slow then tell Arduino to slow down 
                            print('Got slow')
                            sock.send('4') #send a bit to arduino via bluetooth and controll it 
                        elif data == ['stop']: # if the key word is stop then tell Arduino to stop
                            print('Got stop')
                            sock.send('5') #send a bit to arduino via bluetooth and controll it 
                        else:
                            print('Not match anything')
                else:
                    print('Data is none')

                if nlpResult != None:
                    msg = self.handler.obtainMessage1(MsgConst.MSG_DATA_FROM_SERVER)
                    msg.obj = nlpResult            
                    self.handler.sendMessage(msg)
              
                self.audioSrc.clearData()
                

                

        
                
