/*
    4.1.7 Server quit message

    Command: SQUIT
    Parameters: <server> <comment>

    The SQUIT message is needed to tell about quitting or dead servers.
    If a server wishes to break the connection to another server it must
    send a SQUIT message to the other server, using the the name of the
    other server as the server parameter, which then closes its
    connection to the quitting server.

    This command is also available operators to help keep a network of
    IRC servers connected in an orderly fashion.  Operators may also
    issue an SQUIT message for a remote server connection.  In this case,
    the SQUIT must be parsed by each server inbetween the operator and
    the remote server, updating the view of the network held by each
    server as explained below.

    The <comment> should be supplied by all operators who execute a SQUIT
    for a remote server (that is not connected to the server they are
    currently on) so that other operators are aware for the reason of
    this action.  The <comment> is also filled in by servers which may
    place an error or similar message here.

    Both of the servers which are on either side of the connection being
    closed are required to to send out a SQUIT message (to all its other
    server connections) for all other servers which are considered to be
    behind that link.

    Similarly, a QUIT message must be sent to the other connected servers
    rest of the network on behalf of all clients behind that link.  In
    addition to this, all channel members of a channel which lost a
    member due to the split must be sent a QUIT message.

    If a server connection is terminated prematurely (e.g. the server  on
    the  other  end  of  the  link  died),  the  server  which  detects
    this disconnection is required to inform the rest of  the  network
    that  the connection  has  closed  and  fill  in  the comment field
    with something appropriate.

    Numeric replies:

            ERR_NOPRIVILEGES                ERR_NOSUCHSERVER

    Example:

    SQUIT tolsun.oulu.fi :Bad Link ? ; the server link tolson.oulu.fi has
                                    been terminated because of "Bad Link".

    :Trillian SQUIT cm22.eng.umd.edu :Server out of control
                                     ; message from Trillian to disconnect
                                    "cm22.eng.umd.edu" from the net
                                     because "Server out of control".

*/