#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure
DIRS += cacm

include $(TOP)/configure/RULES_TOP
