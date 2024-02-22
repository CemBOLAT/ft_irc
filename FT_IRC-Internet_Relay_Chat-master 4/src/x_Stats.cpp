/*
4.3.2 Stats message

      Command: STATS
   Parameters: [<query> [<server>]]

   The stats message is used to query statistics of certain server.  If
   <server> parameter is omitted, only the end of stats reply is sent
   back.  The implementation of this command is highly dependent on the
   server which replies, although the server must be able to supply
   information as described by the queries below (or similar).

   A query may be given by any single letter which is only checked by
   the destination server (if given as the <server> parameter) and is
   otherwise passed on by intermediate servers, ignored and unaltered.
   The following queries are those found in the current IRC
   implementation and provide a large portion of the setup information
   for that server.  Although these may not be supported in the same way
   by other versions, all servers should be able to supply a valid reply
   to a STATS query which is consistent with the reply formats currently
   used and the purpose of the query.

   The currently supported queries are:

           c - returns a list of servers which the server may connect
               to or allow connections from;
           h - returns a list of servers which are either forced to be
               treated as leaves or allowed to act as hubs;
           i - returns a list of hosts which the server allows a client
               to connect from;
           k - returns a list of banned username/hostname combinations
               for that server;
           l - returns a list of the server's connections, showing how



Oikarinen & Reed                                               [Page 27]

RFC 1459              Internet Relay Chat Protocol              May 1993


               long each connection has been established and the traffic
               over that connection in bytes and messages for each
               direction;
           m - returns a list of commands supported by the server and
               the usage count for each if the usage count is non zero;
           o - returns a list of hosts from which normal clients may
               become operators;
           y - show Y (Class) lines from server's configuration file;
           u - returns a string showing how long the server has been up.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_STATSCLINE                  RPL_STATSNLINE
           RPL_STATSILINE                  RPL_STATSKLINE
           RPL_STATSQLINE                  RPL_STATSLLINE
           RPL_STATSLINKINFO               RPL_STATSUPTIME
           RPL_STATSCOMMANDS               RPL_STATSOLINE
           RPL_STATSHLINE                  RPL_ENDOFSTATS

   Examples:

STATS m                         ; check the command usage for the server
                                you are connected to

:Wiz STATS c eff.org            ; request by WiZ for C/N line
                                information from server eff.org

4.3.3 Links message

      Command: LINKS
   Parameters: [[<remote server>] <server mask>]

   With LINKS, a user can list all servers which are known by the server
   answering the query.  The returned list of servers must match the
   mask, or if no mask is given, the full list is returned.

   If <remote server> is given in addition to <server mask>, the LINKS
   command is forwarded to the first server found that matches that name
   (if any), and that server is then required to answer the query.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_LINKS                       RPL_ENDOFLINKS

   Examples:




Oikarinen & Reed                                               [Page 28]

RFC 1459              Internet Relay Chat Protocol              May 1993


LINKS *.au                      ; list all servers which have a name
                                that matches *.au;

:WiZ LINKS *.bu.edu *.edu       ; LINKS message from WiZ to the first
                                server matching *.edu for a list of
                                servers matching *.bu.edu.

4.3.4 Time message

      Command: TIME
   Parameters: [<server>]

   The time message is used to query local time from the specified
   server. If the server parameter is not given, the server handling the
   command must reply to the query.

   Numeric Replies:

           ERR_NOSUCHSERVER                RPL_TIME

   Examples:

   TIME tolsun.oulu.fi             ; check the time on the server
                                   "tolson.oulu.fi"

   Angel TIME *.au                 ; user angel checking the time on a
                                   server matching "*.au"

*/