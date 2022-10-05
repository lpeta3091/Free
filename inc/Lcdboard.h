#ifndef LCDBOARD_H
#define LCDBOARD_H

#pragma once

#include "lcd_ili9341.h"
#include "fonts.h"
#include <ctime>
#include <dirent.h>
#include <iterator>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include "Mqtt.h"
//#include "def.h"
#include "Pugiparam.h"

using namespace std;

class Lcdboard {
private:
Mqtt *mqtta;
DisplayILI9341_240x320x16_SPI *display; //(73,{1, 0, 70, 0,-1,-1});

uint8_t canvasData[30000];
NanoCanvas16 *canvas; //(221, 41, canvasData);

time_t t;
tm* now;

char path_temp[64];
FILE *TEMPFile;
float temp;

int ADS_address = 0x48;       // Address of our device on the I2C bus
int I2CFile;
float vvv;
int doExit = 0;
bool MESSAGEf = false;
vector<string> strForParce;
vector<string>keyTopic = {"Min", "Max"};
std::vector<std::string>::iterator it;
Pugiparam *pprm;

public:
   Lcdboard()
{
        mqtta = new Mqtt(string("default"));
        display = new DisplayILI9341_240x320x16_SPI(73,{1, 0, 70, 0,-1,-1});
        canvas = new NanoCanvas16(221, 41, canvasData);
        setup();
};
   void  setupLcd();
   void  setup() { setupLcd(); setupTemp(); setupVolt(); };
   void  loop();
   void  vidCircle();
   void  setupTemp();
   float getTemp();
   void  setupVolt();
   float getVolt();
   int   center(int x, int w, int len) { return (x + (w - len * 16)/2); }
   std::string centerS (string s, int w);
   int   getExit() { return doExit; }
   void  doEnd() { display->clear(); display->printFixedN(50, 60, "THE END !", STYLE_NORMAL,1); }
   void  addTopicMqtt(string topic) { strForParce.push_back(topic); mqtta->addSubTopic(topic); }
   void  initTopics() { mqtta->subscribe(); }
   int   checkTopics(string str);
   string getKeyTopic(int index) { return keyTopic[index]; }
   void setPugiParam(Pugiparam *ppm) { pprm = ppm; }
   ~Lcdboard() { doEnd(); display->end(); delete mqtta; delete canvas; fclose(TEMPFile); close(I2CFile); }
};

#endif
