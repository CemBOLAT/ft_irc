#pragma once

#include <vector>
#include <string>

#define USAGE "./ircserv [port] [password]"
#define BOT_USAGE "./turco [port] [password]"
#define VECT_STR std::vector<std::string>
#define VECT_ITER_CLI std::vector<Client>::iterator
#define VECT_ITER_CHA std::vector<Room>::iterator
#define C_VECT_STR const std::vector<std::string>
#define C_VECT_STR_R C_VECT_STR&
#define C_STR const std::string
#define C_STR_REF const std::string &

#define INV_PARAM "Invalid parameters\n"

#define hexchat 1
#define bot 2

#define KEY_CODE 1
#define LIMIT_CODE 2


// HEXCHAT COMMANDS
# define RPL_MODE(source, channel, modes, args)		":" + source + " MODE " + channel + " " + modes + " " + args + "\r\n"
# define JOIN_RESPONSE(nick, ip, channel) ":" + nick + "!" + nick + "@" + ip + " JOIN " + channel + "\r\n"
# define RPL_TOPIC_JOIN(nick, ip, channel, topic) ":" + nick + "!" + nick + "@" + ip + " TOPIC " + channel + " :" + topic + "\r\n"
# define KICK_RESPONSE(nick, ip, channel, message) ":" + nick + "!" + nick + "@" + ip + " KICK " + channel + " " + message + "\r\n"
# define RPL_NICK(nick, user, ip, newnick) ":" + nick + "!" + user + "@" + ip + " NICK :" + newnick + "\r\n"
# define RPL_PART(source, channel)					":" + source + " PART :" + channel + "\r\n"
# define RPL_PART_REASON(source, channel, reason)	":" + source + " PART " + channel + " :" + reason + "\r\n"
# define RPL_PING(source, nick, second)				":" + source + " PONG " + nick + " :"+ second + "\r\n"
# define MSG_GROUP(nick,user, host, room, message) ":" + nick + "!" + user + "@" + host + " PRIVMSG  " + room + " :" + message + "\r\n"
# define RPL_NOTICE(source, target, message) ":" + source + " NOTICE " + target + " :" + message + "\r\n"
# define RPL_PRIVMSG(source, target, message)		":" + source + " PRIVMSG " + target + " :" + message + "\r\n"
#define RPL_QUIT(source, message)                   (std::string(":") + source + " QUIT :" + message + "\r\n")
#define RPL_TOPIC(nick, channel, topic) ": 332 " + nick + " " + channel + " :" + topic + "\r\n"
#define RPL_NOTOPIC(nick, channel) ": 331 " + nick + " " + channel + " :No topic is set\r\n"
#define RPL_TOPICSET(nick, channel, topic, time) ": 333 " + nick + " " + channel + " " + time + "\r\n"

# define RPD_NAMESSTART(nick, channel)			    ": 353 " + nick + " = " + channel + " :"
# define RPL_NAMES(nick, channel, names)             ": 353 " + nick + " = " + channel + " : " + names + "\r\n"
# define RPL_NAMESEND(nick, channel)				    ": 366 " + nick + " " + channel + " :End of /NAMES list\r\n"

#define RPL_WHOISUSER(nick, user, ip)				": 311 " + nick + " " + nick + " " + nick + " " + ip + " * :" + user + "\r\n"
#define RPL_WHOISSERVER(source, server)				": 312 " + source + " * :" + server + "\r\n"
#define RPL_ENDOFWHOIS(source, nick)				": 318 " + source + " " + nick + " :End of /WHOIS list" + "\r\n"

# define ERR_NEEDMOREPARAMS(source, command)			": 461 " + source + " " + command + " :Not enough parameters" + "\r\n"			  
# define ERR_CHANNELISFULL(source, channel)			": 471 " + source + " " + channel + " :Cannot join channel (+l)" + "\r\n"
# define ERR_BADCHANNELKEY(source, channel)			": 475 " + source + " " + channel + " :Cannot join channel (+k)" + "\r\n"
# define ERR_NOSUCHCHANNEL(source, channel)			": 403 " + source + " " + channel + " :No such channel" + "\r\n"				
# define ERR_CHANOPRIVSNEEDED(source, channel)		": 482 " + source + " " + channel + " :You're not the channel operator" + "\r\n"
# define ERR_NOSUCHNICK(source, nick) ": 401 " + source + " " + nick + " :No such nick/channel" + "\r\n"					
# define ERR_NOTONCHANNEL(source, channel) ": 442 " + source + " " + channel + " :You're not on that channel" + "\r\n"	
# define ERR_OWNKICK(source, channel) ": 443 " + source + " " + channel + " :You can't kick yourself" + "\r\n"			
# define ERR_NICKNAMEEMPTY(source) ": 433 " + source + " " + source + " :Nickname cannot empty" + "\r\n"
# define ERR_NICKNAMEINUSE(source) ": 433 " + source + " " + source + " :Nickname is already in use" + "\r\n"
#define ERR_ERRONEUSNICKNAME(nick) ": 432 " + nick + " :Erroneus nickname\r\n"
