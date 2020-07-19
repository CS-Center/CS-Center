BINDIR = build/bin
DEPDIR = build/deps
SOURCEDIRS = executors sandbox runtimes utils

SOURCES = $(shell find $(SOURCEDIRS) -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=$(BINDIR)/%.o)
DEPS = $(SOURCES:%.cpp=$(DEPDIR)/%.d)


CPP_FLAGS += -I. -std=c++11
LDFLAGS += -lpthread -lseccomp

.PHONY: all clean mkbuild
all: main

build/.hasbuild:
	mkdir -p build
	sudo mount -t tmpfs tmpfs build/
	touch build/.hasbuild

main: $(DEPS) $(OBJECTS)
	g++ $(LDFLAGS) -o $@ $(OBJECTS)
	

$(OBJECTS): $(BINDIR)/%.o: %.cpp build/.hasbuild
	@mkdir -p $(dir $@)
	g++ -c -o $@ $(CPP_FLAGS) $< 
	
	
$(DEPS): $(DEPDIR)/%.d: %.cpp build/.hasbuild
	@mkdir -p $(dir $@)
	g++ $(CPP_FLAGS) -MP -MM -MT $(BINDIR)/$(basename $<).o $< > $@
	
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
	
clean:
	sudo umount build
	rm -r build