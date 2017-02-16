ifndef MY_SWIG_PACKAGE
    $(error MY_SWIG_PACKAGE is not defined.)
endif

MY_SWIG_OUTDIR:= src/main/java/$(subst .,/,$(MY_SWIG_PACKAGE))

ifndef MY_SWIG_TYPE
    MY_SWIG_TYPE:=c
endif


ifeq ($(MY_SWIG_TYPE),cxx)
    MY_SWIG_MODE:= -c++
else
    MY_SWIG_MODE:=
endif

LOCAL_SRC_FILES += $(foreach MY_SWIG_INTERFACE,\
    $(MY_SWIG_INTERFACES), \
    $(basename $(MY_SWIG_INTERFACE))_wrap.$(MY_SWIG_TYPE))

LOCAL_CPP_EXTENSION += .cxx

%_wrap.$(MY_SWIG_TYPE) : %.i
	$(call host-mkdir,$(MY_SWIG_OUTDIR))
	swig -java \
	$(MY_SWIG_MODE) \
	-package $(MY_SWIG_PACKAGE) \
	-outdir $(MY_SWIG_OUTDIR) \
	$<
