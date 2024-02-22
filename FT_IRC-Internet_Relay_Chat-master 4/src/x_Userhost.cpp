/*
5.7 Userhost message

      Command: USERHOST
   Parameters: <nickname>{<space><nickname>}

   The USERHOST command takes a list of up to 5 nicknames, each
   separated by a space character and returns a list of information
   about each nickname that it found.  The returned list has each reply
   separated by a space.

   Numeric Replies:

           RPL_USERHOST                    ERR_NEEDMOREPARAMS

   Examples:

   USERHOST Wiz Michael Marty p    ;USERHOST request for information on
                                   nicks "Wiz", "Michael", "Marty" and "p"

*/