BINDIR = build/bin
DEPDIR = build/deps
SOURCEDIRS = executors sandbox runtimes utils

COMPILER = g++

SOURCES = $(shell find $(SOURCEDIRS) -name '*.cpp')
HEADERS = $(shell find $(SOURCEDIRS) -name '*.hpp')

OBJECTS = $(SOURCES:%.cpp=$(BINDIR)/%.o)
DEPS = $(SOURCES:%.cpp=$(DEPDIR)/%.d)

CPP_FLAGS += -I. -Wextra -Wpedantic -Wall -g # -O
LDFLAGS += -lpthread -lseccomp

.PHONY: all clean mkbuild
all: build/main

build/.hasbuild:
	mkdir -p build
	sudo mount -t tmpfs tmpfs build/
	touch build/.hasbuild

build/main: $(DEPS) $(OBJECTS)
	$(COMPILER) -o $@ $(OBJECTS) main.cpp $(LDFLAGS)


$(OBJECTS): $(BINDIR)/%.o: %.cpp build/.hasbuild
	@mkdir -p $(dir $@)
	$(COMPILER) -c -o $@ $(CPP_FLAGS) $<


$(DEPS): $(DEPDIR)/%.d: %.cpp build/.hasbuild
	@mkdir -p $(dir $@)
	$(COMPILER) $(CPP_FLAGS) -MP -MM -MT $(BINDIR)/$(basename $<).o $< > $@

	$(COMPILER) $(CPP_FLAGS) -MM -MT $@ $< >> $@

# ^^^^^^^^^^^
# In the future, if this file's dependencies are updated, re-update the dependency file as well
# This means that if
# a.cpp depends on b.hpp
# b.hpp depends on c.hpp
# if c.hpp is deleted, a.cpp is initially recompiled because of a indirect dependency on c.hpp
# a new dependency file is also created indicating that a.cpp no longer depends on c.hpp
# This also means that dependency files aren't re-created unless they need to be


ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

clean:
	sudo umount build
	rm -r build
