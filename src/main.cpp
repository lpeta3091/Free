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
#include <thread>
#include "Lcdboard.h"
#include "Mqttthread.h"
#include "Pugiparam.h"
#include "def.h"

_mData mData;

Pugiparam pparam("setup.xml", &mData);

int pugiStart(Lcdboard *lcdbd)
{
string str;
        pparam.getParam();
	str = pparam.viewParam(0);
	lcdbd->addTopicMqtt(str + "/" + lcdbd->getKeyTopic(0));
	lcdbd->addTopicMqtt(str + "/" + lcdbd->getKeyTopic(1));

	str = pparam.viewParam(1);
        lcdbd->addTopicMqtt(str + "/" + lcdbd->getKeyTopic(0));
        lcdbd->addTopicMqtt(str + "/" + lcdbd->getKeyTopic(1));
	lcdbd->initTopics();
	lcdbd->setPugiParam(&pparam);
	return 0;
}

using namespace std::chrono;
#include "main.inl"

bool doExit = 0;

void do_heartbeat()
{
   // TODO: implement processing code to be performed on each heartbeat
}

int threadFirst()
{
   threadMqtt("first", doFirst);
   return 0;
}

int threadSecond()
{
   threadMqtt("second", doSecond);
   return 0;
}

int main()
{
//   pid_t pid, sid;
//   int halt = 0;

   signal(SIGHUP, signal_handler);
   signal(SIGTERM, signal_handler);
   signal(SIGINT, signal_handler);
   signal(SIGQUIT, signal_handler);
   daemon();
   Lcdboard *lcdboard = new Lcdboard;
   openlog("LCDBOARD", LOG_NOWAIT | LOG_PID, LOG_USER);
   syslog(LOG_NOTICE, "Lcdboard.Successfully started");
   syslog(LOG_NOTICE, ("Current folder : " + get_current_dir()).c_str());

   std::thread thread1(threadFirst);
   std::thread thread2(threadSecond);
   thread1.detach();
   thread2.detach();

   pugiStart(lcdboard);
    while (!doExit && !(lcdboard->getExit())) {
      lcdboard->loop();
    }
    usleep(1000000);
    syslog(LOG_NOTICE, "Lcdboard.Successfully ended");
    delete lcdboard;
    closelog();
}

void endWork(void )
{
}
