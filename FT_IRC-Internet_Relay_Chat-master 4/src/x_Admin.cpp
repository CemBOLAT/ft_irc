/*
4.3.7 Admin command

      Command: ADMIN
   Parameters: [<server>]

   The admin message is used to find the name of the administrator of
   the given server, or current server if <server> parameter is omitted.
   Each server must have the ability to forward ADMIN messages to other
   servers.

   Numeric Replies:

           ERR_NOSUCHSERVER
           RPL_ADMINME                     RPL_ADMINLOC1
           RPL_ADMINLOC2                   RPL_ADMINEMAIL

   Examples:

   ADMIN tolsun.oulu.fi            ; request an ADMIN reply from
                                   tolsun.oulu.fi

   :WiZ ADMIN *.edu                ; ADMIN request from WiZ for first
                                   server found to match *.edu.
*/