/*
4.3.8 Info command

      Command: INFO
   Parameters: [<server>]

   The INFO command is required to return information which describes
   the server: its version, when it was compiled, the patchlevel, when
   it was started, and any other miscellaneous information which may be
   considered to be relevant.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_INFO                        RPL_ENDOFINFO

   Examples:

   INFO csd.bu.edu                 ; request an INFO reply from
   csd.bu.edu

   :Avalon INFO *.fi               ; INFO request from Avalon for first
                                   server found to match *.fi.



Oikarinen & Reed                                               [Page 31]

RFC 1459              Internet Relay Chat Protocol              May 1993


   INFO Angel                      ; request info from the server that
                                   Angel is connected to.
*/