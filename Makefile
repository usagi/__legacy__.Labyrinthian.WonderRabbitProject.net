THIS_MAKEFILE:=$(abspath $(lastword $(MAKEFILE_LIST)))
NACL_SDK_ROOT?=$(abspath $(dir $(THIS_MAKEFILE))../..)
OSNAME       :=$(shell uname --kernel-name | tr "[:upper:]" "[:lower:]")
TC_PATH      :=$(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_newlib)

CXXPPAPI    := -lppapi -lppapi_cpp -lppapi_gles2
CXXWARNINGS := -Wall
CXXSTD      := -std=gnu++98
CXXOPTIMIZE := -O2
CXXTHREAD   := -pthread

CXXFLAGS:=$(CXXTHREAD) $(CXXSTD) $(CXXOPTIMIZE) $(CXXWARNINGS) $(CXXPPAPI)

CXX:=$(TC_PATH)/bin/i686-nacl-g++

all: Labyrinthian_i686.nexe Labyrinthian_x86_64.nexe

Labyrinthian_i686.nexe: Labyrinthian.cxx $(THIS_MAKE)
	$(CXX) -o $@ $< -m32 $(CXXFLAGS)

Labyrinthian_x86_64.nexe: Labyrinthian.cxx $(THIS_MAKE)
	$(CXX) -o $@ $< -m64 $(CXXFLAGS)

clean:
	@-if [ -d _site ]; then rm -rfv _site/*; fi;
	@-rmdir -v _site
	@-rm -v *.o
	@-rm -v *.nexe

.PHONY: _site
_site:
	@if [ -d _site ]; then rm -rf _site/*; else mkdir _site; fi;
	@-ln -v *.nexe _site
	@-ln -v *.html _site
	@-ln -v *.nmf  _site
	@-ln -v *.js   _site
	@-ln -v *.css  _site

.PHONY: test
test: all _site
	darkhttpd _site --index main.html

