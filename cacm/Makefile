TOP = ..

include $(TOP)/configure/CONFIG

STATIC_BUILD = YES

PROD_HOST = caClientMonitor

# Needs access to the CA-private header inetAddrID.h
ifdef BASE_3_15
  ifdef BASE_7_0
    CA_SRC = $(EPICS_BASE)/modules/ca/src/client
  else
    CA_SRC = $(EPICS_BASE)/src/ca/client
  endif
else
  CA_SRC = $(EPICS_BASE)/src/ca
endif

ifeq ($(wildcard $(CA_SRC)/inetAddrID.h),)
  $(error "Can't find inetAddrID.h, not in $(CA_SRC)")
else
  USR_INCLUDES += -I$(CA_SRC)
endif

caClientMonitor_SRCS += caClientMonitor.cpp
caClientMonitor_SRCS += clientData.cpp
caClientMonitor_SRCS += cmServer.cpp

PROD_LIBS = cas gdd ca Com

include $(TOP)/configure/RULES
