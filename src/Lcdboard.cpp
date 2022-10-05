#include "lcd_ili9341.h"
#include <assert.h>
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
#include "Lcdboard.h"
#include "Mqtt.h"
#include <linux/reboot.h>
#include <sys/reboot.h>
#include <algorithm>
#include "Lcdboard.inl"

void Lcdboard::setupVolt()
{
   I2CFile = open("/dev/i2c-3", O_RDWR);         // Open the I2C device
   ioctl(I2CFile, I2C_SLAVE, ADS_address);       // Specify the address of the I2C Slave to communicate with
}

float Lcdboard::getVolt()
{
	uint8_t writeBuf[3];          // Buffer to store the 3 bytes that we write to the I2C device
   	uint8_t readBuf[2];           // 2 byte buffer to store the data read from the I2C device
	int16_t val;
	char strVvv[64];

	writeBuf[0] = 1;              // This sets the pointer register so that the following two bytes write to the config register
  	writeBuf[1] = 0xC3;           // This sets the 8 MSBs of the config register (bits 15-8) to 11000011
  	writeBuf[2] = 0x03;           // This sets the 8 LSBs of the config register (bits 7-0) to 00000011

  	readBuf[0]= 0;
  	readBuf[1]= 0;
  	write(I2CFile, writeBuf, 3);

  	while ((readBuf[0] & 0x80) == 0)      // readBuf[0] contains 8 MSBs of config register, AND with 10000000 to select bit 15
  	{
          read(I2CFile, readBuf, 2);    // Read the config register into readBuf
  	}

  	writeBuf[0] = 0;                                      // Set pointer register to 0 to read from the conversion register
  	write(I2CFile, writeBuf, 1);

  	read(I2CFile, readBuf, 2);            // Read the contents of the conversion register into readBuf

  	val = readBuf[0] << 8 | readBuf[1];   // Combine the two bytes of readBuf into a single 16 bit result

	vvv = (float)val*4.096/32767.0*4.94;
/*
	if (vvv <= 7.0) {
		halt = 1;
		endWork();
		display.setFixedFont(ssd1306xled_font6x8);
		display.clear();
		sprintf(strVvv, "Low Voltage -%4.1fv", vvv);
		display.printFixedN(15, 60, strVvv, STYLE_NORMAL,1);
		display.printFixedN(80, 80, "Halt !", STYLE_NORMAL,1);
            	doExit = 1;
		system("shutdown -n now");
		continue;
	}
*/
	sprintf(strVvv, "%4.2f (V)", vvv);
	display->setFixedFont(ssd1306xled_font8x16);
	display->printFixed(50, 30, strVvv, STYLE_NORMAL);
	return vvv;
}

void Lcdboard::setupTemp()
{
	strcpy(path_temp, "/sys/devices/virtual/thermal/thermal_zone0/temp");
	TEMPFile = fopen(path_temp, "r");
}

float Lcdboard::getTemp()
{
      unsigned int readBytes = 0;
      char tempBuf[16], strTemp[64];
      fseek(TEMPFile, 0, SEEK_SET);

      while (readBytes < 16) {
         int curRead = fread(tempBuf + readBytes, 1, 16 - readBytes, TEMPFile);
          if (curRead > 0)
             readBytes += curRead;
          else
             break;
      }

      if (readBytes > 0)
         *(tempBuf + readBytes - 1) = 0;

      temp = atof(tempBuf);
      temp /= 1000.f;
      sprintf(strTemp, "Temp=%4.1f", temp);
      display->setFixedFont(ssd1306xled_font8x16);
      display->printFixed(130, 30, strTemp, STYLE_NORMAL);
      return temp;
}

void Lcdboard::setupLcd()
{
    char str[64];
    display->begin();
    display->clear();
    display->setFixedFont(ssd1306xled_font8x16);
    display->setColor(RGB_COLOR16(255,255,0));
    display->drawRect(1, 1, 240, 320);

    display->setColor(RGB_COLOR16(255,100,255));
    display->drawRect(3, 3, 237, 50);
    display->drawCircle(120, 160, 50);

    display->setColor(RGB_COLOR16(255,255,255));
    canvas->setMode( CANVAS_MODE_TRANSPARENT );
    canvas->clear();
    //canvas.drawBitmap1( 5, 8, 8, 8, heartImage );
    canvas->drawRect(0,0,220,40);
    display->drawCanvas( 10, 270, *canvas );
    //strcpy(str, mqtta->str_from_mqtt);
    display->printFixedN(center(10, 220, 7), 275, "Wait...", STYLE_NORMAL, 1);

}

void Lcdboard::vidCircle()
{
       int i = 0, j = 0, k = 0;
        display->setColor(RGB_COLOR16(rand() % 255, rand() % 255, rand() % 255));
                if (i <= 30000) {
                        i+=450;
                }
                else { i = 0; j = i + 16; k = j + 16; }
        display->drawCircle(120, 160, rand() % 60 + 5);
        display->setColor(i+130);
        display->drawCircle(120, 160, 50);
}

std::string centerS (string s, int w) {
    std::stringstream ss, spaces;
    int pad = w - s.size();                  // count excess room to pad
    for(int i=0; i<pad/2; ++i)
        spaces << " ";
    ss << spaces.str() << s << spaces.str(); // format with padding
    if(pad>0 && pad%2!=0)                    // if pad odd #, add 1 more space
        ss << " ";
    return ss.str();
}

void Lcdboard::loop()
{
	char str[512], strblank[] = "            ";
        t = std::time(0);
        now = std::localtime(&t);
        display->setFixedFont(courier_new_font11x16_digits);
        sprintf(str,"%02u:%02u:%04u %02u:%02u:%02u", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);
        display->printFixed(15, 10, str, STYLE_NORMAL);

	vidCircle();
	display->setColor(RGB_COLOR16(255,255,255));
        if (mqtta->getMessageF()) {
	   syslog(LOG_NOTICE, (mqtta->str_topic + "/" + mqtta->str_payload).c_str());
	    if (mqtta->str_topic == "command") {
		if (mqtta->str_payload == "halt") {
			mqtta->setCmd(0);
			// reboot(LINUX_REBOOT_CMD_POWER_OFF);
		}
	    }
	    if (checkTopics(mqtta->str_topic)) {
		vector<string> vs = split(mqtta->str_topic, '/');
		pprm->setParam(vs[0].c_str(), vs[1].c_str(), atoi(mqtta->str_payload.c_str()));
	    	syslog(LOG_NOTICE, (vs[0] + "*/*" + mqtta->str_payload).c_str());
		}
		display->setFixedFont(ssd1306xled_font8x16);
		string sstr = mqtta->str_payload;
		strcpy(str, sstr.c_str());
        	syslog(LOG_NOTICE, str);
		//string str_center = centerS(sstr, 220);
		    std::stringstream ss, spaces;
		    int w = 13;
		    int pad = w - sstr.size();                  // count excess room to pad
    		    for(int i=0; i<pad/2; ++i) spaces << " ";
    		    ss << spaces.str() << sstr << spaces.str(); // format with padding
    		    if(pad>0 && pad%2!=0)                    // if pad odd #, add 1 more space
        	    ss << " ";
		    //ss << "|";
    		    //ss.str();

		//display->printFixedN(center(10, 220, strlen(strblank)), 275, strblank, STYLE_NORMAL, 1);
        	//display->printFixedN(center(10, 220, strlen(str)), 275, str, STYLE_NORMAL, 1);
		display->printFixedN(18, 275, ss.str().c_str(), STYLE_NORMAL, 1);
		mqtta->publish(ss.str().c_str());
	}
	sprintf(str, "%f %f", getTemp(), getVolt());
        mqtta->publish(str);
	if (!mqtta->getCmd()) { doExit = 1; doEnd(); }
        usleep(500000);
}

int Lcdboard::checkTopics(string str)
{
        it = find(strForParce.begin(), strForParce.end(), str);
	if (it == strForParce.end()) return 0;
	else return 1;
                //if ( std::any_of(strForParce.begin(), strForParce.end(), [](string sp) {return str==sp;} ) )
                //((std::find(strForParce.begin(), strForParce.end(), str) != strForParce.end()) ? 1 : 0 );
}
