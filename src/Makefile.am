AM_CFLAGS = -std=c99
AM_CXXFLAGS = -std=c++98

bin_PROGRAMS = ts
ts_SOURCES = main.cxx \
			 apirequest.cxx \
			 appconfig.cxx \
			 base64.cxx \
			 clicontroller.cxx \
			 commandfactory.cxx \
			 filehelper.cxx \
			 optionsparser.cxx \
			 servercommunicator.cxx

ts_SOURCES += commands/initrepo.cxx \
			  commands/status.cxx \
			  commands/addsecret.cxx \
			  commands/removesecret.cxx \
			  commands/serverfetch.cxx \
			  commands/serverpush.cxx \
			  commands/serversync.cxx
