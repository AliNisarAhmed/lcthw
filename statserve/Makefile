CFLAGS=-g -O2 -Wall -Wextra -Isrc -I/usr/include -rdynamic -DBDEBUG $(OPTFLAGS)
LIBS=-ldl -llcthw $(OPTLIBS)
LDFLAGS=-L/usr/include/liblcthw -llcthw
PREFIX?=/usr/include

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

TARGET=build/libstatserve.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

# The Target build 
all: $(TARGET) tests bin/statserve

dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all 

so: $(SO_TARGET)

bin/statserve: $(TARGET)

$(TESTS): $(TARGET) 

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET): $(TARGET) $(OBJECTS)
		$(CC) -shared -o $@ $(LDFLAGS) $(LIBS) $(OBJECTS)

build:
	@mkdir -p build 
	@mkdir -p bin

# THe Unit tests
.PHONY: tests 
tests: CFLAGS += $(TARGET)
tests: $(TESTS)
	sh ./tests/runtests.sh

# valgrind
valgrind:
	VALGRIND="valgrind --log-file=/tmp/valgrind-%p.log" $(MAKE)

# The cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

# The install
install: all
	install -d $(PREFIX)/install/
	install $(TARGET) $(PREFIX)/install/

# The checker 
check:
	@echo Files with potentially dangerous functions.
	@egrep '[^_.a>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|_)\
		|stpn?cpy|a?sn?printf|byte_)' $(SOURCES) || true
