#include "Client.hpp"
#include "Room.hpp"
#include "Executor.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include "Define.hpp"

#include "Define.hpp"
#include <iostream>
#include <string>

#include <vector>
#include <cstdlib>

/*

	4.2.3 Mode message

	Command: MODE

	The MODE command is a dual-purpose command in IRC.  It allows both
	usernames and channels to have their mode changed.  The rationale for
	this choice is that one day nicknames will be obsolete and the
	equivalent property will be the channel.

	When parsing MODE messages, it is recommended that the entire message
	be parsed first and then the changes which resulted then passed on.

	4.2.3.1 Channel modes

	Parameters: <channel> {[+|-]|o|p|s|i|t|n|b|v} [<limit>] [<user>]
	            [<ban mask>]

	The MODE command is provided so that channel operators may change the
	characteristics of `their' channel.  It is also required that servers
	be able to change channel modes so that channel operators may be
	created.

	The various modes available for channels are as follows:

	        o - give/take channel operator privileges;
	        p - private channel flag;
	        s - secret channel flag;
	        i - invite-only channel flag;
	        t - topic settable by channel operator only flag;
	        n - no messages to channel from clients on the outside;
	        m - moderated channel;
	        l - set the user limit to channel;



	arinen & Reed                                               [Page 21]

	 1459              Internet Relay Chat Protocol              May 1993


	        b - set a ban mask to keep users out;
	        v - give/take the ability to speak on a moderated channel;
	        k - set a channel key (password).

	When using the 'o' and 'b' options, a restriction on a total of three
	per mode command has been imposed.  That is, any combination of 'o'
	and

	.3.2 User modes

	Parameters: <nickname> {[+|-]|i|w|s|o}

	The user MODEs are typically changes which affect either how the
	client is seen by others or what 'extra' messages the client is sent.
	A user MODE command may only be accepted if both the sender of the
	message and the nickname given as a parameter are both the same.

	The available modes are as follows:

	        i - marks a users as invisible;
	        s - marks a user for receipt of server notices;
	        w - user receives wallops;
	        o - operator flag.

	Additional modes may be available later on.

	If a user attempts to make themselves an operator using the "+o"
	flag, the attempt should be ignored.  There is no restriction,
	however, on anyone `deopping' themselves (using "-o").  Numeric
	Replies:

	        ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
	        ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
	        ERR_NOTONCHANNEL                ERR_KEYSET
	        RPL_BANLIST                     RPL_ENDOFBANLIST
	        ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

	        ERR_USERSDONTMATCH              RPL_UMODEIS
	        ERR_UMODEUNKNOWNFLAG

	Use of Channel Modes:

	MODE #Finnish +im           ; Makes #Finnish channel moderated and
	                            'invite-only'.

	MODE #Finnish +o Kilroy     ; Gives 'chanop' privileges to Kilroy on
	                            channel #Finnish.

	MODE #Finnish +v Wiz        ; Allow WiZ to speak on #Finnish.

	MODE #Fins -s               ; Removes 'secret' flag from channel
	                            #Fins.

	MODE #42 +k oulu            ; Set the channel key to "oulu".

	MODE #eu-opers +l 10        ; Set the limit for the number of users
	                            on channel to 10.

	MODE &oulu +b               ; list ban masks set for channel.

	MODE &oulu +b *!*@*         ; prevent all users from joining.

	MODE &oulu +b *!*@*.edu     ; prevent any user from a hostname
	                            matching *.edu from joining.

	        Use of user Modes:

	:MODE WiZ -w                ; turns reception of WALLOPS messages
	                            off for WiZ.

	:Angel MODE Angel +i        ; Message from Angel to make themselves
	                            invisible.

	MODE WiZ -o                 ; WiZ 'deopping' (removing operator
	                            status).  The plain reverse of this
	                            command ("MODE WiZ +o") must not be
	                            allowed from users since would bypass
	                            the OPER command.

*/

void	Server::mode(C_STR_REF input, Client &client){
	VECT_STR params = Utils::ft_split(input, " ");
	if (params.size() < 3){
		Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
		return;
	}
	if (params.size() == 1){
		return;
	}
	// odayı buluyom
	VECT_ITER_CHA it = this->channels.begin();
	for (; it != this->channels.end(); ++it){
		if (it->getName() == params[0]){
			break;
		}
	}
	if (it == this->channels.end()){ // oda yoksa
		Utils::instaWrite(client.getFd(), ERR_NOSUCHCHANNEL(client.getNick(), params[0]));
		return;
	}
	// operatör mü
	if (it->getOperator().getNick() != client.getNick()){
		Utils::instaWrite(client.getFd(), ERR_CHANOPRIVSNEEDED(client.getNick(), params[0]));
		return;
	}
	if (params[1] == "+k"){ // sets a key to the channel
		if (params.size() == 3){
			it->setKey(params[2]);
			it->setKeycode(it->getKeycode() | KEY_CODE); // yada (0 | 1) == 1
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "+k", params[2]));
		}
	}
	if (params[1] == "-k"){
		if (params.size() == 2){
			it->setKey("");
			it->setKeycode(it->getKeycode() ^ KEY_CODE); // (1i 0 0ı 1 yapar)
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "-k", ""));
		}
	}
	if (params[1] == "+l"){ // limits the number of clients in the channel (working properly)
		if (params.size() == 3){
			if (it->getClients().size() < static_cast<size_t>(atoi(params[2].c_str()))){
				Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getNick(), "MODE"));
				return;
			}
			it->setChanelLimit(atoi(params[2].c_str()));
			it->setKeycode(it->getKeycode() | LIMIT_CODE); 
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "+l", params[2]));
		}
	}
	if (params[1] == "-l"){ // removes the limit of clients in the channel (working properly)
		if (params.size() == 2){
			it->setKeycode(it->getKeycode() ^ LIMIT_CODE);
			Utils::instaWrite(client.getFd(), RPL_MODE(client.getNick(), it->getName(), "-l", ""));
		}
	}
	if (params[1] == "+o"){ // gives operator status to a client (working properly)
		if (params.size() == 3){
			std::string joined = Utils::ft_join(params, " ", 2);
			op(params[0] +  " " + joined, client);
		}
	}
}
