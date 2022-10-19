run: app
	../bin/server.exe
	
app: server.o
	gcc -o ../bin/server.exe ../obj/server.o
	
server.o: ../src/server.c
	gcc -o ../obj/server.o ../src/server.c -c
	
clean:
	rm ../obj/*.o ../bin/*.exe

test: ../../../Tools_Report/CUnit/test_program.c  ../../../Tools_Report/CUnit/func.c
	gcc -o ../../../Tools_Report/CUnit/test.exe ../../../Tools_Report/CUnit/test_program.c ../../../Tools_Report/CUnit/func.c -lcunit
	../../../Tools_Report/CUnit/test.exe > ../../../Tools_Report/CUnit/Cunit_Report.txt