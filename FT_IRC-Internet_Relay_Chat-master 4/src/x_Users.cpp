/*
5.5 Users

      Command: USERS
   Parameters: [<server>]



Oikarinen & Reed                                               [Page 40]

RFC 1459              Internet Relay Chat Protocol              May 1993


   The USERS command returns a list of users logged into the server in a
   similar  format  to  who(1),  rusers(1)  and finger(1).  Some people
   may disable this command on their server for security related
   reasons.   If disabled, the correct numeric must be returned to
   indicate this.

   Numeric Replies:

           ERR_NOSUCHSERVER                ERR_FILEERROR
           RPL_USERSSTART                  RPL_USERS
           RPL_NOUSERS                     RPL_ENDOFUSERS
           ERR_USERSDISABLED

   Disabled Reply:

           ERR_USERSDISABLED

   Examples:

USERS eff.org                   ; request a list of users logged in on
                                server eff.org

:John USERS tolsun.oulu.fi      ; request from John for a list of users
                                logged in on server tolsun.oulu.fi

*/