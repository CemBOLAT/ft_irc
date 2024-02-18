/*

    Command: VERSION
    Parameters: [<server>]

    The VERSION message is used  to  query  the  version  of  the  server
    program.  An optional parameter <server> is used to query the version
    of the server program which a client is not directly connected to.

    Numeric Replies:

            ERR_NOSUCHSERVER                RPL_VERSION

    Examples:

    :Wiz VERSION *.se               ; message from Wiz to check the version
                                    of a server matching "*.se"

    VERSION tolsun.oulu.fi          ; check the version of server
                                    "tolsun.oulu.fi".
*/