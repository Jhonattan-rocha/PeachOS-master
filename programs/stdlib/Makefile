FILES=./build/start.asm.o ./build/start.o ./build/peachos.asm.o ./build/peachos.o ./build/stdlib/stdlib.o ./build/stdio/stdio.o ./build/string/string.o ./build/memory/memory.o
INCLUDES=-I./src
FLAGS= -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ${FILES}
	i686-elf-ld -m elf_i386 -relocatable ${FILES} -o ./stdlib.elf

./build/start.asm.o: ./src/start.asm
	nasm -f elf ./src/start.asm -o ./build/start.asm.o

./build/peachos.asm.o: ./src/peachos.asm
	nasm -f elf ./src/peachos.asm -o ./build/peachos.asm.o

./build/peachos.o: ./src/peachos.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./src/peachos.c -o ./build/peachos.o

./build/stdlib/stdlib.o: ./src/stdlib/stdlib.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./src/stdlib/stdlib.c -o ./build/stdlib/stdlib.o

./build/stdio/stdio.o: ./src/stdio/stdio.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./src/stdio/stdio.c -o ./build/stdio/stdio.o


./build/string/string.o: ./src/string/string.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o

./build/memory/memory.o: ./src/memory/memory.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/start.o: ./src/start.c
	i686-elf-gcc ${INCLUDES} $(FLAGS) -std=gnu99 -c ./src/start.c -o ./build/start.o

clean:
	rm -rf ${FILES}
	rm ./stdlib.elf