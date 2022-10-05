#include "inc/lcd_ili9341.h"
//#include "inc/lcdgfx_gui.h"
#include "inc/fonts.h"
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
#include "inc/Mqtt.h"

Mqtt *mqtta;
DisplayILI9341_240x320x16_SPI display(73,{1, 0, 70, 0,-1,-1}); // Use this line for Raspberry  (gpio24=RST, 0=CE, gpio23=D/C)

const int canvasWidth = 221; // Width
const int canvasHeight = 41; // Height
uint8_t canvasData[canvasWidth*canvasHeight*2];
/* Create canvas object */
NanoCanvas16 canvas(canvasWidth, canvasHeight, canvasData);


std::time_t t;   // get time now
std::tm* now;

void setupLcd()
{
    display.begin();
    display.clear();
    display.setFixedFont(ssd1306xled_font8x16);
    display.setColor(RGB_COLOR16(255,255,0));
    display.drawRect(1, 1, 240, 320);

    display.setColor(RGB_COLOR16(255,100,255));
    display.drawRect(3, 3, 237, 50);
    display.drawCircle(120, 160, 50);

    display.setColor(RGB_COLOR16(255,255,255));
    canvas.setMode( CANVAS_MODE_TRANSPARENT );
    canvas.clear();
    //canvas.drawBitmap1( 5, 8, 8, 8, heartImage );
    canvas.drawRect(0,0,220,40);
    display.drawCanvas( 10, 270, canvas );

}

int center(int x, int w, int len)
{
	return (x + (w - len * 16)/2);
}

void loop()
{
	char str[512];
        t = std::time(0);
        now = std::localtime(&t);
        display.setFixedFont(courier_new_font11x16_digits);
        sprintf(str,"%02u:%02u:%04u %02u:%02u:%02u", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);
        display.printFixed(15, 10, str, STYLE_NORMAL);

	int i = 0, j = 0, k = 0;
        display.setColor(RGB_COLOR16(rand() % 255, rand() % 255, rand() % 255));
                if (i <= 30000) {
                        i+=450;
                }
                else { i = 0; j = i + 16; k = j + 16; }
                display.drawCircle(120, 160, rand() % 60 + 10);
                display.setColor(i+130);
                display.drawCircle(120, 160, 50);

                if (mqtta->getLastMessage() != mqtta->getOldMessage()) {
                        canvas.clear();
                        canvas.drawRect(0,0,220,40);
                        display.drawCanvas( 10, 272, canvas );
			display.setFixedFont(ssd1306xled_font8x16);
			strcpy(str, mqtta->getLastMessage().c_str());
                        display.printFixedN(center(10, 220, strlen(str)), 275, str, STYLE_NORMAL, 1);
                        //canvas.printFixedPgm(1, 1, "Hi...", STYLE_NORMAL);
                        mqtta->publish("Test...");
                        usleep(500000);
                }
}

int main()
{
    mqtta = new Mqtt(string("default"));
    mqtta->subscribe();
    setupLcd();
    while (1) {
      loop();
    }
    display.end();
    delete mqtta;
}
