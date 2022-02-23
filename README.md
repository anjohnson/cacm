# CA Client Monitor

The CA Client Monitor program is a CA Server that listens for CA
client name searches and reports statistics on what it sees. It can be
configured to run continuously, or to take a one-off snapshot of the
searches occurring over a short (timed) period. Whether it generates a
CA beacon anomaly or not when it starts is also configurable.

## Building

This is a standard EPICS support module which requires an installation
of EPICS Base (any version from 3.14 to 7.0.x or later should work). For
Base versions 3.16 and later you will also need the PCAS module which
provides the Channel Access server.

Edit the `configure/RELEASE` file to set the paths. If building against
a Base version that contains the CA server libraries (libcas and libgdd)
the `PCAS` variable can be commented out. Then run `make` from the top of
the module. This should install a single executable in the `bin/<arch>`
directory called `caClientMonitor`.

## Running

The `-h` option prints usage instructions:

```
$ caClientMonitor -h
usage: caClientMonitor [-h] [-oA] [-BCDNRbcdnr] [-kmt <seconds>]
   -h      Help, prints this message
   -o      Once, prints a single report and exits
   -A      Generate CA beacon anomaly at startup
   -k <n>  How long (seconds) silent clients stay alive
   -m <n>  Death reports list client PV names up to max <n>
   -t <n>  How often (seconds) to report clients, rates and names
   -BCDNR  Report {births, clients, deaths, names, rates}
   -bcdnr  Suppress {births, clients, deaths, names, rates}
Specifying -o implies -bCdNR, omitting it implies -BCDnR
$
```

### Server mode

Running the program with no arguments starts a server that will run
indefinitely. For each new CA client it sees it immediately prints the
current date/time along with the hostname and UDP port number that sent
the search request (a "birth announcement"). It also prints a similar
message (a "death notice") when a client has not sent any searches for
the kill period (set by the `-k` option, which defaults to 30 seconds);
note that the date/time on the death notice tells when client last sent
out a search, not its "time of death", so it is independent of the kill
period requested.

Every 30 seconds (configurable with the `-t` option) it also prints a
report of how many clients are still alive, the total number of searches
it has seen and the search rate (searches per second for this report
period), then for each live client it shows how old they are in seconds,
how many requests they have made and how many different names they have
asked for. Running `caClientMonitor` with no options is equivalent to

```
$ caClientMonitor -BCDnR -k 30 -m0 -t 30
```

The options `-B` and `-b` enable and disable the birth announcement
messages, while `-D` and `-d` similarly control the death notices. `-R`
enables and `-r` disables the periodic display of the overall search
rate statistics, while the `-C` option enables and `-c` disables the
periodic report of the individual clients. Reporting of the individual
PV names is controlled by `-N` and `-n`, but they are only shown when
the client report is also enabled (`-C`). The `-m` option defaults to
0 and can be used to enable a list of the PV names in the death notice
for clients that have asked for up to that many different PV names (no
PV names will be printed for clients that requested more than that many
names in their lifetime).

### One-shot mode

The other main common usage is with the `-o` option for a one-shot report.
Running `caClientMonitor -o` is equivalent to

```
$ caClientMonitor -o -bCdNR -k 30 -m0 -t 30
```

In this case the birth annoucements and death notices are disabled,
and after 30 seconds (configurable with `-t` again) the search rates and
clients seen during that period will be displayed once and the program
will exit.

The program will not normally generate a CA beacon anomaly when it starts
in either server or one-shot mode, so it can report the current live
searches without triggering reconnection attempts from longer-lived
clients which have stopped searching for some names. Giving the `-A`
option will trigger a beacon anomaly at start-up.

-----

ANJ 2022-02-23

