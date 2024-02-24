#include "Client.hpp"

#include "Exception.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "Room.hpp"
#include "Utils.hpp"
#include "TextEngine.hpp"
#include "Define.hpp"
#include "ErrorRPL.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

/*
4.2.1 Join message

      Command: JOIN
   Parameters: <channel>{,<channel>} [<key>{,<key>}]

   The JOIN command is used by client to start listening a specific
   channel. Whether or not a client is allowed to join a channel is
   checked only by the server the client is connected to; all other
   servers automatically add the user to the channel when it is received
   from other servers.  The conditions which affect this are as follows:

           1.  the user must be invited if the channel is invite-only;

           2.  the user's nick/username/hostname must not match any
               active bans;

           3.  the correct key (password) must be given if it is set.

   These are discussed in more detail under the MODE command (see
   section 4.2.3 for more details).

   Once a user has joined a channel, they receive notice about all
   commands their server receives which affect the channel.  This
   includes MODE, KICK, PART, QUIT and of course PRIVMSG/NOTICE.  The
   JOIN command needs to be broadcast to all servers so that each server
   knows where to find the users who are on the channel.  This allows
   optimal delivery of PRIVMSG/NOTICE messages to the channel.

   If a JOIN is successful, the user is then sent the channel's topic
   (using RPL_TOPIC) and the list of users who are on the channel (using
   RPL_NAMREPLY), which must include the user joining.

   Numeric Replies:

           ERR_NEEDMOREPARAMS (done)              ERR_BANNEDFROMCHAN
           ERR_INVITEONLYCHAN (done)             ERR_BADCHANNELKEY
           ERR_CHANNELISFULL (done)              ERR_BADCHANMASK
           ERR_NOSUCHCHANNEL (done)              ERR_TOOMANYCHANNELS
           RPL_TOPIC

   Examples:

   JOIN #foobar                    ; join channel #foobar.

   JOIN &foo fubar                 ; join channel &foo using key "fubar".

   JOIN #foo,&bar fubar            ; join channel #foo using key "fubar"
                                   and &bar using no key.

   JOIN #foo,#bar fubar,foobar     ; join channel #foo using key "fubar".
                                   and channel #bar using key "foobar".

   JOIN #foo,#bar                  ; join channels #foo and #bar.

   :WiZ JOIN #Twilight_zone        ; JOIN message from WiZ
*/

#define FLAG_KEY 1
#define FLAG_INV 2
#define FLAG_TOPIC 4
#define FLAG_NOOUTSIDE 8
#define FLAG_LIMIT 16

#define ERR_ALREADYJOINED(source, channel) ": 443 " + source + " " + channel + " :You are already in that channel\n\r"

void Server::join(C_STR_REF params, Client &client)
{
        if (client.getIsRegistered() == false){
	        Utils::instaWrite(client.getFd(), ERR_NOTREGISTERED(client.getUserByHexChat()));
                return;
	}
        stringstream ss(params);
        string roomName, key, message;
        if (params.empty()){
                Utils::instaWrite(client.getFd(), ERR_NEEDMOREPARAMS(client.getUserByHexChat(), "JOIN"));
                return;
        }
        ss >> roomName;
        ss >> key;
        if (roomName[0] != '#')
        {
                roomName = "#" + roomName;
        }
        if (isRoom(roomName)){
                if (getRoom(roomName).isClientInChannel(client.getFd())){
                        Utils::instaWrite(client.getFd(), ERR_ALREADYJOINED(client.getUserByHexChat(), roomName));
                        //Utils::instaWrite(client.getFd(), ERR_TOOMANYCHANNELS(client.getUserByHexChat(), roomName)); // my own implement
                        return;
                }
                Room &room = getRoom(roomName);
                if ((room.getKeycode() & FLAG_INV) && !room.isInvite(client)){
                        Utils::instaWrite(client.getFd(), ERR_INVITEONLYCHAN(client.getUserByHexChat(), roomName));
                        return;
                }
                if (room.getKeycode() & FLAG_KEY){
                        if (key.empty() || room.getKey() != key){
                                Utils::instaWrite(client.getFd(), ERR_BADCHANNELKEY(client.getUserByHexChat(), roomName));
                                return;
                        }
                }
                if (room.getKeycode() & FLAG_LIMIT){
                        if (room.getChanelLimit() <= (int) room.getClients().size()){
                                Utils::instaWrite(client.getFd(), ERR_CHANNELISFULL(client.getUserByHexChat(), roomName));
                                return;
                        }
                }
                if (room.isInvite(client)){
                        room.removeInvite(client);
                }
                room.addClient(client);
                Utils::instaWriteAll(room.getClients(), RPL_JOIN(client.getUserByHexChat(), roomName));
                if (!room.getTopic().empty()){
                        Utils::instaWrite(client.getFd(), RPL_TOPIC(client.getUserByHexChat(), roomName, room.getTopic()));
                }
        }
        else {
                TextEngine::green("Room " + roomName + " has been created by " + client.getUserByHexChat(), TextEngine::printTime(std::cout)) << std::endl;
                Room room;
                room.setName(roomName);
                room.addOperator(client);
                room.addClient(client);
                channels.push_back(room);
                Utils::instaWriteAll(room.getClients(), RPL_JOIN(client.getUserByHexChat(), roomName));
        }
        responseAllClientResponseToGui(client, getRoom(roomName));
}
//stringstream ss(params);
//string roomName, key, message;
//if (!params.empty()){
//	ss >> roomName;
//	ss >> key;
//	if (key.empty())
//		key = "";
//	if (roomName.empty()){
//		client.getmesagesFromServer().push_back("JOIN :Not enough parameters\n\r");
//		FD_SET(client.getFd(), &writefds);
//		return;
//	}
//	if (roomName[0] != '#'){
//		roomName = "#" + roomName;
//	}
//	if (isRoom(roomName)){
//		for (VECT_ITER_CHA it = channels.begin(); it != channels.end(); it++){
//			if (roomName == (*it).getName()){
//				if (!it->isClientInChannel(client.getFd())){
//					if ((it->getKeycode() & KEY_CODE) && (it->getKeycode() & LIMIT_CODE))
//					{ // odamda hem limit var hem şifre var
//						if (it->getChanelLimit() <= (int) it->getClients().size()) {
//							Utils::instaWrite(client.getFd(), ERR_CHANNELISFULL(client.getNick(), roomName));
//						}
//						else if (key == "" && it->getKey() != key) {
//							Utils::instaWrite(client.getFd(), ERR_BADCHANNELKEY(client.getNick(), roomName));
//						}
//						else {
//							(*it).getClients().push_back(client);
//							Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
//							if (!(*it).getTopic().empty()){
//								Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
//							}
//						}
//					}
//					else if ((it->getKeycode() & KEY_CODE)) { // şifre var
//						if (key == "" && it->getKey() != key) {
//							Utils::instaWrite(client.getFd(), ERR_BADCHANNELKEY(client.getNick(), roomName));
//						}
//						else {
//							(*it).getClients().push_back(client);
//							Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
//							if (!(*it).getTopic().empty()){
//								Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
//							}
//						}
//					}
//					else if ((it->getKeycode() & LIMIT_CODE)) { // limit var
//						if (it->getChanelLimit() <= (int) it->getClients().size()) {
//							Utils::instaWrite(client.getFd(), ERR_CHANNELISFULL(client.getNick(), roomName));
//						}
//						else {
//							(*it).getClients().push_back(client);
//							Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
//							if (!(*it).getTopic().empty()){
//								Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
//							}
//						}
//					}
//					else { // ikiside yok
//						(*it).getClients().push_back(client);
//						Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
//						if (!(*it).getTopic().empty()){
//							Utils::instaWrite(client.getFd(), RPL_TOPIC_JOIN(client.getNick(), client._ip , roomName, (*it).getTopic()));
//						}
//					}
//				} else {
//					client.getmesagesFromServer().push_back("You already in the chanel\n\r");
//					FD_SET(client.getFd(), &writefds);
//				}
//				break;
//			}
//		}
//	} else {
//		TextEngine::green("Room " + roomName + " has been created by " + client.getNick(), TextEngine::printTime(std::cout)) << std::endl;
//		Room room;
//		room.setName(roomName);
//		room.setOperator(client);
//		room.addClient(client);
//		channels.push_back(room);
//		Utils::instaWrite(client.getFd(), JOIN_RESPONSE(client.getNick(), client._ip , roomName));
//	}
//	responseAllClientResponseToGui(client, getRoom(roomName));
//} else {
//	client.getmesagesFromServer().push_back("JOIN :Not enough parameters");
//	FD_SET(client.getFd(), &writefds);
//}
