vpath %.c src
vpath %.h headers

CPPFLAGS = -I headers -Wall

objects := main.o utils.o tuntap.o

tcp-ip: $(objects)
	$(CC) $(objects) -o tcp-ip

clean:
	rm *.o tcp-ip
