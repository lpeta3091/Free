#ifndef LCDBOARD_H
#define LCDBOARD_H

#pragma once

//#include <mosquittopp.h>
#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include "lcd_ili9341.h"
#include "fonts.h"
#include "Mqtt.h"

using namespace std;

class Lcdboard : public Mqtt {
private:
    int active;

public:
   Lcdboard(string id);

    ~Lcdboard();
};

#endif //LCDBOARD_H
