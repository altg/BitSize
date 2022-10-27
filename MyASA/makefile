CC=g++
AR=ar

ARFLAGS=crus

LIBNAME=libmyasa.a

DEBUGFLAG=

# -DUSER_ACCEPTANCE_TEST=TRUE

DEFINES = \
#  -DUSER_ASA_OUT=TRUE \
	-DOPTIONS_FILE=FALSE \
   $(DEBUGFLAG)

#CFLAGS=-pg -g -Wall   $(DEFINES)
CFLAGS=-g -Wall   $(DEFINES)


LFLAGS=-lm

DEFAULT_SLIB_PATH=.


all: testharness2

lib: 
	$(CC) $(CFLAGS) -c asa.c -o asa.o
	$(CC) $(CFLAGS) -c asa_usr.c -o asa_usr.o

testmain: asa_usr.o asa_usr_cst.o asa.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)


myasa:	myasa.o asa.o
	$(AR) $(ARFLAGS) $(LIBNAME) $^	

myasa_sh: myasa_sh.o asa_sh.o
	$(CC) -shared -Wl,-soname,libasash.so -o libasash.so $^

mytest2: mytest.o mycost_func.o asa.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)


testharness: test_harness.o mycost_func3.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) -L . -lmyasa

myasa_cost: mycost_func3.o 
	$(CC) -shared -Wl,-soname,libmycost.so -o libmycost.so $^

testharness2: test_harness.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS) -L . -lasash -ldl -Wl,-rpath,$(DEFAULT_SLIB_PATH)


testmain2: test_main5.o asa.o mycost_func.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

%.o: %.cpp
	$(CC) -c $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $^

%_sh.o: %.cpp
	$(CC) -fPIC -c $(CFLAGS) -o $@ $^

%_sh.o: %.c
	$(CC) -fPIC -c $(CFLAGS) -o $@ $^

clean:
	rm -f *.o testmain testharness

clean_lib:
	rm -f lib*.a lib*.so

clean_all: clean clean_lib

