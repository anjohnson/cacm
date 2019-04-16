// caClientMonitor.cpp

#include <stdlib.h>
#include "epicsTime.h"
#include "fdManager.h"
#include "cmServer.h"


void
usage(void)
{
    puts("usage: caClientMonitor [-h] [-oA] [-BCDNRbcdnr] [-kmt <seconds>]\n"
         "   -h      Help, prints this message\n"
         "   -o      Once, prints a single report and exits\n"
         "   -A      Generate CA beacon anomaly at startup\n"
         "   -k <n>  How long (seconds) silent clients stay alive\n"
         "   -m <n>  Death reports list client PV names up to max <n>\n"
         "   -t <n>  How often (seconds) to report clients, rates and names\n"
         "   -BCDNR  Report {births, clients, deaths, names, rates}\n"
         "   -bcdnr  Suppress {births, clients, deaths, names, rates}\n"
         "Specifying -o implies -bCdNR, omitting it implies -BCDnR");
}

int
main(int argc, char **argv)
{
    int show = nothing;
    int hide = nothing;
    int usual = births | deaths | clients | rates;
    double killDelay = 30.0;    // seconds
    unsigned long maxNames = 0;
    double prunePeriod = 5.0;   // seconds
    double reportDelay = 30.0;  // seconds
    bool repeat = true;
    bool beacons = false;
    int c;

    while ((c = getopt(argc, argv, "bcdnrhok:m:t:ABCDNR")) != EOF) {
        switch(c) {
        case 'B': show |= births;   break;
        case 'C': show |= clients;  break;
        case 'D': show |= deaths;   break;
        case 'N': show |= names;    break;
        case 'R': show |= rates;    break;
        
        case 'b': hide |= births;   break;
        case 'c': hide |= clients;  break;
        case 'd': hide |= deaths;   break;
        case 'n': hide |= names;    break;
        case 'r': hide |= rates;    break;

        case 'A':
            beacons = true;
            break;

        case 'h':
            usage();
            exit(0);

        case 'k':
            killDelay = atof(optarg);
            if (killDelay < 1.0)
                killDelay = 1.0;
            if (killDelay < prunePeriod)
                prunePeriod = killDelay;
            break;

        case 'm':
            maxNames = atoi(optarg);
            break;

        case 'o':
            repeat = false;
            usual = clients | names | rates;
            break;

        case 't':
            reportDelay = atof(optarg);
            break;
        }
    }

    if (!beacons) {
        static char beaconEnv[] = "EPICS_CAS_BEACON_PORT=65535";
        putenv(beaconEnv);
    }

    cmServer server((usual & ~hide) | show);
    
    epicsTime timeNow = epicsTime::getCurrent();
    epicsTime whenPruned = timeNow;
    epicsTime whenReported = timeNow;
    
    bool active = true;
    while (active) {
        fileDescriptorManager.process(0.1);
        timeNow = epicsTime::getCurrent();
        if (timeNow - whenPruned >= prunePeriod) {
            server.prune(timeNow - killDelay, maxNames);
            whenPruned = timeNow;
        }
        if (timeNow - whenReported >= reportDelay) {
            server.report(timeNow, timeNow - whenReported);
            whenReported = timeNow;
            server.reset();
            active = repeat;
        }
    }

    return 0;
}
