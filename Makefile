all: xor_list
xor_list: list.c
	$(CC) list.c -O3 -o xor_list
clean:
	rm xor_list
