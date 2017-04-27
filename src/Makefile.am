AM_CFLAGS = -std=c99
AM_CXXFLAGS = -std=c++98

bin_PROGRAMS = ts
ts_SOURCES = main.cxx \
			 clicontroller.cxx \
			 filehelper.cxx \
			 optionsparser.cxx \
			 servercommunicator.cxx

ts_SOURCES += commands/addsecret.cxx \
			  commands/removesecret.cxx \
			  commands/serverfetch.cxx \
			  commands/serverpush.cxx
