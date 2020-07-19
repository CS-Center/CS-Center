BINDIR = bin
DEPDIR = deps

SOURCES = $(shell find * -name '*.cpp')
OBJECTS = $(SOURCES:%.cpp=$(BINDIR)/%.o)
DEPS = $(SOURCES:%.cpp=$(DEPDIR)/%.d)


CPP_FLAGS += -I. -std=c++11
LDFLAGS += 

.PHONY: all clean
all: main

main: $(DEPS) $(OBJECTS)
	g++ $(LDFLAGS) -o $@ $(OBJECTS)
	

$(OBJECTS): $(BINDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	g++ -c -o $@ $(CPP_FLAGS) $< 
	
	
$(DEPS): $(DEPDIR)/%.d: %.cpp
	@mkdir -p $(dir $@)
	@touch $@
	g++ $(CPP_FLAGS) -MP -MM -MT $(BINDIR)/$(basename $<).o $< > $@
	
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
	
clean:
	rm -r $(BINDIR) $(DEPDIR)
