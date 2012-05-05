THIS_MAKEFILE:=$(abspath $(lastword $(MAKEFILE_LIST)))
NACL_SDK_ROOT?=$(abspath $(dir $(THIS_MAKEFILE))../..)
OSNAME       :=$(shell uname --kernel-name | tr "[:upper:]" "[:lower:]")
TC_PATH      :=$(abspath $(NACL_SDK_ROOT)/toolchain/$(OSNAME)_x86_glibc)

CXXPPAPI    := -lppapi -lppapi_cpp -lppapi_gles2
CXXINCLUDES := -Iinclude
CXXWARNINGS := -Wall
CXXSTD      := -std=c++0x
CXXOPTIMIZE := -O2
CXXTHREAD   := -pthread

CXXFLAGS:=$(CXXTHREAD) $(CXXSTD) $(CXXOPTIMIZE) $(CXXWARNINGS) $(CXXPPAPI) $(CXXINCLUDES)

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
	@-rm -v *.nmf

.PHONY: _site_dir
_site_dir:
	@if [ -d _site ]; then rm -rf _site/*; else mkdir _site; fi;

.PHONY: glibc_so_64
glibc_so_64: _site_dir Labyrinthian_x86_64.nexe
	@if [ -d _site/lib64 ]; then rm -rf _site/lib64/*; else mkdir _site/lib64; fi;
	@ln -v $(TC_PATH)/x86_64-nacl/lib/runnable-ld.so _site/lib64/
	@for a in `$(TC_PATH)/bin/x86_64-nacl-objdump -p Labyrinthian_x86_64.nexe | grep NEEDED | tr NEEDED " " | sed "s/^[ ]*//" | tr "\n" " "`; do ln -v $(TC_PATH)/x86_64-nacl/lib/$${a} _site/lib64/$${a}; done;
	@chmod 644 _site/lib64/*

.PHONY: glibc_so_32
glibc_so_32: _site_dir Labyrinthian_i686.nexe
	@if [ -d _site/lib32 ]; then rm -rf _site/lib32/*; else mkdir _site/lib32; fi;
	@ln -v $(TC_PATH)/x86_64-nacl/lib32/runnable-ld.so _site/lib32/
	@for a in `$(TC_PATH)/bin/i686-nacl-objdump -p Labyrinthian_i686.nexe | grep NEEDED | tr NEEDED " " | sed "s/^[ ]*//" | tr "\n" " "`; do ln -v $(TC_PATH)/x86_64-nacl/lib32/$${a} _site/lib32/$${a}; done;
	@chmod 644 _site/lib32/*

.PHONY: Labyrinthian.nmf
Labyrinthian.nmf: _site_dir Labyrinthian_x86_64.nexe
	@echo '{' >> Labyrinthian.nmf
	@echo '  "program": {' >> Labyrinthian.nmf
	@echo '     "x86-64": { "url": "lib64/runnable-ld.so" },' >> Labyrinthian.nmf
	@echo '     "x86-32": { "url": "lib32/runnable-ld.so" }' >> Labyrinthian.nmf
	@echo '  },' >> Labyrinthian.nmf
	@echo '  "files": {' >> Labyrinthian.nmf
	@for a in `$(TC_PATH)/bin/i686-nacl-objdump -p Labyrinthian_i686.nexe | grep NEEDED | tr NEEDED " " | sed "s/^[ ]*//" | tr "\n" " "`;\
		do\
		echo "    \"$${a}\": {" >> Labyrinthian.nmf;\
		echo "      \"x86-64\": { \"url\": \"lib64/$${a}\"}," >> Labyrinthian.nmf;\
		echo "      \"x86-32\": { \"url\": \"lib32/$${a}\"}" >> Labyrinthian.nmf;\
		echo '    },' >> Labyrinthian.nmf;\
		done;
	@echo '    "main.nexe": {' >> Labyrinthian.nmf
	@echo '      "x86-64": { "url": "Labyrinthian_x86_64.nexe" },' >> Labyrinthian.nmf
	@echo '      "x86-32": { "url": "Labyrinthian_i686.nexe" }' >> Labyrinthian.nmf
	@echo '    }' >> Labyrinthian.nmf
	@echo '  }' >> Labyrinthian.nmf
	@echo "}" >> Labyrinthian.nmf

.PHONY: _site
_site: _site_dir Labyrinthian.nmf glibc_so_64 glibc_so_32 Labyrinthian_x86_64.nexe Labyrinthian_i686.nexe
	@-ln -v *.nexe _site
	@-ln -v *.html _site
	@-ln -v *.nmf  _site
	@-ln -v *.js   _site
	@-ln -v *.css  _site
	@-ln -v favicon.ico _site

.PHONY: test-server
test-server: all _site
	darkhttpd _site --index main.html --port 12345

.PHONY: test-client
test-client: 
	@ if [ -f test-client.log ]; then rm -v test-client.log; fi;
	chromium-dev "http://localhost:12345" | tee test-client.log

.PHONY: deploy-test
deploy-test: all _site
	rsync -av --delete _site/ Labyrinthian.WonderRabbitProject.net:/srv/http/WonderRabbitProject.net/Labyrinthian/test

.PHONY: deploy-current
deploy-current: all _site
	rsync -av --delete _site/ Labyrinthian.WonderRabbitProject.net:/srv/http/WonderRabbitProject.net/Labyrinthian/current

