# Specify extensions of files to delete when cleaning
CLEANEXTS   = o so d

# Specify the source files, the target files, 
# and the install directory 
NAME = Line_Follower
SOURCES     = $(NAME).c
OUTPUTFILE  = lib$(NAME).so
INSTALLDIR  = .

.PHONY: all
all: $(OUTPUTFILE)

# Build libgeorgeringo.so from george.o, ringo.o, 
# and georgeringo.o; subst is the search-and-replace 
# function demonstrated in Recipe 1.16
$(OUTPUTFILE): $(subst .c,.o,$(SOURCES)) 
	$(CXX) -shared -fPIC $(LDFLAGS) -o $@ $^

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done

# Generate dependencies of .c files on .h files
include $(subst .c,.d,$(SOURCES))

%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
rm -f $@.$$$$
