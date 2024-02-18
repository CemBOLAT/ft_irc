/*
4.3.6 Trace message

      Command: TRACE
   Parameters: [<server>]

   TRACE command is used to find the route to specific server.  Each
   server that processes this message must tell the sender about it by
   sending a reply indicating it is a pass-through link, forming a chain
   of replies similar to that gained from using "traceroute".  After
   sending this reply back, it must then send the TRACE message to the
   next server until given server is reached.  If the <server> parameter
   is omitted, it is recommended that TRACE command send a message to
   the sender telling which servers the current server has direct
   connection to.

   If the destination given by "<server>" is an actual server, then the
   destination server is required to report all servers and users which
   are connected to it, although only operators are permitted to see
   users present.  If the destination given by <server> is a nickname,
   they only a reply for that nickname is given.

   Numeric Replies:

           ERR_NOSUCHSERVER

   If the TRACE message is destined for another server, all intermediate
   servers must return a RPL_TRACELINK reply to indicate that the TRACE
   passed through it and where its going next.

           RPL_TRACELINK
   A TRACE reply may be composed of any number of the following numeric
   replies.

           RPL_TRACECONNECTING             RPL_TRACEHANDSHAKE
           RPL_TRACEUNKNOWN                RPL_TRACEOPERATOR
           RPL_TRACEUSER                   RPL_TRACESERVER
           RPL_TRACESERVICE                RPL_TRACENEWTYPE
           RPL_TRACECLASS

   Examples:

TRACE *.oulu.fi                 ; TRACE to a server matching *.oulu.fi




Oikarinen & Reed                                               [Page 30]

RFC 1459              Internet Relay Chat Protocol              May 1993


:WiZ TRACE AngelDust            ; TRACE issued by WiZ to nick AngelDust
*/