/*
4.6.4 Error

      Command: ERROR
   Parameters: <error message>

   The ERROR command is for use by servers when reporting a serious or
   fatal error to its operators.  It may also be sent from one server to
   another but must not be accepted from any normal unknown clients.

   An ERROR message is for use for reporting errors which occur with a
   server-to-server link only.  An ERROR message is sent to the server
   at the other end (which sends it to all of its connected operators)
   and to all operators currently connected.  It is not to be passed
   onto any other servers by a server if it is received from a server.

   When a server sends a received ERROR message to its operators, the
   message should be encapsulated inside a NOTICE message, indicating
   that the client was not responsible for the error.

   Numerics:

           None.

   Examples:

   ERROR :Server *.fi already exists; ERROR message to the other server
                                   which caused this error.

   NOTICE WiZ :ERROR from csd.bu.edu -- Server *.fi already exists
                                   ; Same ERROR message as above but sent
                                   to user WiZ on the other server.
*/