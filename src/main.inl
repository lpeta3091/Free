#define GetCurrentDir getcwd

string get_current_dir() {
   char buff[FILENAME_MAX]; //create string buffer to hold path
   GetCurrentDir( buff, FILENAME_MAX );
   string current_working_dir(buff);
   return current_working_dir;
}

extern bool doExit;

void endWork(void );

inline void signal_handler(int sig) {
 
    switch(sig) {
        case SIGHUP:
            syslog(LOG_WARNING, "Lcdboard. Received SIGHUP signal.");
            break;
        case SIGTERM:
            syslog(LOG_WARNING, "Lcdboard. Received SIGTERM signal.");
            endWork();
            doExit = 1;;
            break;
        default:
            syslog(LOG_WARNING, "Lcdboard. Unhandled signal");
            break;
    }
}

inline void daemon()
{
   int pid, sid;
   pid = fork();
   if(pid > 0)
   {
      exit(EXIT_SUCCESS);
   }
   else if(pid < 0)
   {
      exit(EXIT_FAILURE);
   }
   umask(0);

   sid = setsid();
   if(sid < 0)
   {
      exit(EXIT_FAILURE);
   }

   if((chdir("/")) < 0)
   {
      exit(EXIT_FAILURE);
   }

   close(STDIN_FILENO);
   close(STDOUT_FILENO);
   close(STDERR_FILENO);
}

void doFirst()
{
	// syslog(LOG_WARNING, "Lcdboard. doFirst");
}

void doSecond()
{
        // syslog(LOG_WARNING, "Lcdboard. doSecond");
}



inline int threadMqtt(string id, void (*doIt)())
{
        stringstream strm, strmb, strme;
        std::vector<string> stl;
        stl.push_back("first/in");
        Mqtt *mqtt = new Mqtt(id.c_str(), id.c_str(), stl, "127.0.0.1", 1883, "user", "passw0rd");
	strmb << "Lcdboard. " <<  id << " " << "started" << std::endl;
        syslog(LOG_NOTICE, strmb.str().c_str());
        auto started = std::chrono::high_resolution_clock::now();
    while(!doExit)
    {
	doIt();
        usleep(500000);
        auto done = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_span = duration_cast<duration<double>>(done - started);
        strm << id.c_str() << " : " << time_span.count() << " seconds." << std::endl;
        mqtt->publish(strm.str().c_str());
    }
   delete mqtt;
   strme << "Lcdboard. " <<  id << "ended" << std::endl;
   syslog(LOG_NOTICE, strme.str().c_str());
   return 0;
}
