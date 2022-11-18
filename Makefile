help:
	@echo "make help - this message"
	@echo "make run - build and run the project and clean up afterwards"
	@echo "make build - build the project"
	@echo "make play - play the project"
	@echo "make clean - remove all generated files"

run:
	@echo "Compiling game ..."
	@gcc Main.c Functions.c AboutItem.c -o main -w
	@echo "Done."
	@./main
	@echo "Cleaning up ..."
	@rm main
	@echo "Done."

build:
	@echo "Compiling game ..."
	@gcc Main.c Functions.c AboutItem.c -o main $(sdl2-config --cflags --libs)
	@echo "Done."
	@echo "Run with 'make play'"

play:
	@echo "Running game ..."
	@./main
	@echo "Hope you enjoyed it!"
	@echo "clean up with 'make clean'"

clean:
	@echo "Cleaning up ..."
	@rm -f main Functions AboutItem
	@echo "Done."
