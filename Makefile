#!/usr/bin/make -f

include configure.mk

CC=g++
INCLUDE=-I./ -I../shared
DEFINE=-D LOGDIR=\"$(INST_LOGDIR)\"
CFLAGS=
CXXFLAGS=-g -Wall -Wextra -MMD -O2 
LFLAGS=-L../shared
LIBS=

OBJECTS=GpsTracker.o
OUTPUT=GpsTracker

all: GpsTracker

GpsTracker.o: GpsTracker.cc
	$(CC) $(CFLAGS) $(CXXFLAGS) $(INCLUDE) $(DEFINE) -c GpsTracker.cc

GpsTracker: $(OBJECTS)
	$(CC) $(CXXFLAGS) $(LFLAGS) -O $(OBJECTS) $(LIBS) -o $(OUTPUT) 

clean:
	$(RM)  $(OBJECTS) $(OUTPUT) *.d

install:
	$(MKDIR) $(INST_LOGDIR)
	$(CP) $(OUTPUT) $(INST_SBINDIR)/
	chown $(INST_USER): $(INST_SBINDIR)/$(OUTPUT)
	chmod 0755 $(INST_SBINDIR)/$(OUTPUT)
uninstall:
	$(RM) $(INST_SBINDIR)/$(OUTPUT)
