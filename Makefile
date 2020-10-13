# Specify extensions of files to delete when cleaning
CLEANEXTS   = o so d

# Specify the source files, the target files, 
# and the install directory 
NAME = Line_Follower
SOURCES     = $(NAME).c
OUTPUTFILE  = lib$(NAME).so
INSTALLDIR  = .

.PHONY: all
all: $(OUTPUTFILE) $(NAME)_Test

# Build .so from .o, subst is the search-and-replace 
$(OUTPUTFILE): $(subst .c,.o,$(SOURCES)) 
	$(CXX) -shared -fPIC $(LDFLAGS) -o $@ $^

test: $(NAME)_Test
	LD_LIBRARY_PATH=. ./$(NAME)_Test

$(NAME)_Test: lib$(NAME).so
	$(CC) $(NAME)_Test.c -o $@ -L. -l$(NAME)

.PHONY: install
install:
	mkdir -p $(INSTALLDIR)
	cp -p $(OUTPUTFILE) $(INSTALLDIR)

.PHONY: clean 
clean:
	for file in $(CLEANEXTS); do rm -f *.$$file; done
	rm $(NAME)_Test

# Generate dependencies of .c files on .h files
include $(subst .c,.d,$(SOURCES))

%.d: %.c
	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
rm -f $@.$$$$
