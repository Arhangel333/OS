all:main1 main2


main1: libfirst.so main1.c
	gcc main1.c -L. -lfirst -o main1 -Wl,-rpath,.

main2: libsecond.so main2.c
	gcc main2.c -ldl -o main2

libfirst.so: lib1.o
	gcc -shared lib1.o -o libfirst.so

libsecond.so: lib2.o
	gcc -shared lib2.o -o libsecond.so

lib1.o:
	gcc -c lib1.c -o lib1.o

lib2.o: 
	gcc -fPIC -c lib2.c -o lib2.o

clean: 
	rm -f *.o *.so main1 main2