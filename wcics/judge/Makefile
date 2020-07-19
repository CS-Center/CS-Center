BINDIR = build/bin
DEPDIR = build/deps
SOURCEDIRS = executors sandbox runtimes utils

COMPILER = g++

SOURCES = $(shell find $(SOURCEDIRS) -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=$(BINDIR)/%.o)
DEPS = $(SOURCES:%.cpp=$(DEPDIR)/%.d)


CPP_FLAGS += -I. -O
LDFLAGS += -lpthread -lseccomp

.PHONY: all clean mkbuild
all: main

build/.hasbuild:
	mkdir -p build
	sudo mount -t tmpfs tmpfs build/
	touch build/.hasbuild

main: $(DEPS) $(OBJECTS)
	$(COMPILER) $(LDFLAGS) -o $@ $(OBJECTS)
	

$(OBJECTS): $(BINDIR)/%.o: %.cpp build/.hasbuild
	@mkdir -p $(dir $@)
	$(COMPILER) -c -o $@ $(CPP_FLAGS) $< 
	
	
$(DEPS): $(DEPDIR)/%.d: %.cpp build/.hasbuild
	@mkdir -p $(dir $@)
	$(COMPILER) $(CPP_FLAGS) -MP -MM -MT $(BINDIR)/$(basename $<).o $< > $@
	
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
	
clean:
	sudo umount build
	rm -r build