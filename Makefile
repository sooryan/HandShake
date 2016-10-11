CPPFLAGS = -I headers -Wall

src = $(wildcard src/*.c)
obj = $(patsubst src/%.c, build/%.o, $(src))

tcp-ip: $(obj)
	$(CC) $(obj) -o tcp-ip

build/%.o: src/%.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm build/*.o tcp-ip
