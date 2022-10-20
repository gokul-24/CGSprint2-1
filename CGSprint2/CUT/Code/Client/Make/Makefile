run: app
	../bin/client.exe
	
app: client.o
	gcc -o ../bin/client.exe ../obj/client.o
	
client.o: ../src/client.c
	gcc -o ../obj/client.o ../src/client.c -c
	
clean:
	rm ../obj/*.o ../bin/*.exe
	
