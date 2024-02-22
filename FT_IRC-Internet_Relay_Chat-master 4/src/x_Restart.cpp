/*
5.3 Restart message

      Command: RESTART
   Parameters: None

   The restart message can only be used by an operator to force a server
   restart itself.  This message is optional since it may be viewed as a
   risk to allow arbitrary people to connect to a server as an operator
   and execute this command, causing (at least) a disruption to service.



Oikarinen & Reed                                               [Page 39]

RFC 1459              Internet Relay Chat Protocol              May 1993


   The RESTART command must always be fully processed by the server to
   which the sending client is connected and not be passed onto other
   connected servers.

   Numeric Replies:

           ERR_NOPRIVILEGES

   Examples:

   RESTART                         ; no parameters required.
*/