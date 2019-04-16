TOP = ..

include $(TOP)/configure/CONFIG

STATIC_BUILD = YES

PROD_HOST = caClientMonitor

# This is needed for inetAddrID.h
USR_INCLUDES += -I$(EPICS_BASE)/src/ca/client

caClientMonitor_SRCS += caClientMonitor.cpp
caClientMonitor_SRCS += clientData.cpp
caClientMonitor_SRCS += cmServer.cpp

PROD_LIBS = cas gdd ca Com

include $(TOP)/configure/RULES
