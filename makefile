make notbash:
	gcc -o ./target/notbash ./source/notbash.c ./source/utils.c ./source/builtin.c -I./source

make clean:
	rm ./target/notbash

make run:
	./target/notbash