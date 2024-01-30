# irc_commands.txt

# IRC Sunucusu Başlatma
./ircserv 6667 mypassword

# Bağlantı Kurma
nc localhost 6667

# Kullanıcı Bilgilerini Ayarlama
NICK assg00k
USER assg00k 0 * :Real Name

# Kanala Katılma
JOIN #kanal_adı

# Özel Mesaj Gönderme
/PRIVMSG hedef_kullanici :Merhaba, nasılsın?

# Kanaldan Ayrılma
/PART #kanal_adı

# Sunucudan Çıkma
/QUIT [neden]

# Kullanıcı Hakkında Bilgi Alma
/WHOIS kullanici_adi

# Kanal Modları
/MODE #kanal_adı +o kullanici_adi

# Aktif Kanalları Listeleme
/LIST

# Kanal Konusunu Ayarlama
/TOPIC #kanal_adı [yeni_konu]

# Kullanıcıya Not Gönderme
/NOTICE kullanici_adi :Bu bir nottur.

# Kullanıcıyı Kanaldan Atma
/KICK #kanal_adı kullanici_adi [neden]
