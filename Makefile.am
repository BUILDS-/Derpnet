AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = src/derpnet

src_derpnet_CPPFLAGS = -std=gnu++0x -pthread $(CFLAGS) -I$(srcdir)/src
src_derpnet_LDFLAGS = -pthread $(LDFLAGS)

src_derpnet_SOURCES = src/ConnectionWrapper.h \
	src/server.h \
	src/ircserv/Channel.h \
	src/ircserv/IrcConn.h \
	src/ircserv/irc.h \
	src/ircserv/IrcServer.h \
	src/ircserv/User.h \
	src/multicast/MultiConn.h \
	src/multicast/MultiServer.h \
	src/sockserv/Connection.h \
	src/sockserv/SockServ.h \
	src/main.cc \
	src/errors.cc \
	src/globals.cc \
	src/ircserv/Channel.cc \
	src/ircserv/IrcConn.cc \
	src/ircserv/IrcServer.cc \
	src/ircserv/User.cc \
	src/multicast/MultiConn.cc \
	src/multicast/MultiServer.cc \
	src/sockserv/Connection.cc \
	src/sockserv/SockServ.cc
