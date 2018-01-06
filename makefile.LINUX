all:ask EXT
	-rm ask
EXT : mainLinux.o aesLinux.o md5Linux.o basicIOLinux.o systemLinux.o readkeyLinux.o abouttimeLinux.o extLinux.o operationLinux.o usersLinux.o
	g++ -o EXT mainLinux.o aesLinux.o md5Linux.o basicIOLinux.o systemLinux.o readkeyLinux.o abouttimeLinux.o extLinux.o operationLinux.o usersLinux.o -lcurses
	-rm mainLinux.o aesLinux.o md5Linux.o basicIOLinux.o systemLinux.o readkeyLinux.o abouttimeLinux.o extLinux.o operationLinux.o usersLinux.o
	-rm *.h.gch
ask : askLinux.o
	g++ -o ask askLinux.o -lcurses
	-rm askLinux.o
	./ask
askLinux.o :   ask.cpp
	iconv -f "gb2312" -t "utf-8" ask.cpp -o askLinux.cpp
	g++ -c askLinux.cpp
	rm askLinux.cpp
mainLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" main.cpp -o mainLinux.cpp
	g++ -c mainLinux.cpp switches.h
	rm mainLinux.cpp
aesLinux.o : main.cpp aes.h disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" aes.cpp -o aesLinux.cpp
	g++ -c aesLinux.cpp switches.h
	rm aesLinux.cpp
md5Linux.o : main.cpp md5.h disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" md5.cpp -o md5Linux.cpp
	g++ -c md5Linux.cpp switches.h
	rm md5Linux.cpp
basicIOLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" basicIO.cpp -o basicIOLinux.cpp
	g++ -c basicIOLinux.cpp switches.h
	rm basicIOLinux.cpp
systemLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" system.cpp -o systemLinux.cpp
	g++ -c systemLinux.cpp switches.h
	rm systemLinux.cpp
readkeyLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" readkey.cpp -o readkeyLinux.cpp
	g++ -c readkeyLinux.cpp switches.h
	rm readkeyLinux.cpp
abouttimeLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" abouttime.cpp -o abouttimeLinux.cpp
	g++ -c abouttimeLinux.cpp switches.h
	rm abouttimeLinux.cpp
extLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" ext.cpp -o extLinux.cpp
	g++ -c extLinux.cpp switches.h
	rm extLinux.cpp
operationLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" operation.cpp -o operationLinux.cpp
	g++ -c operationLinux.cpp switches.h
	rm operationLinux.cpp
usersLinux.o : main.cpp disk.h errorconstances.h switches.h
	iconv -f "gb2312" -t "utf-8" users.cpp -o usersLinux.cpp
	g++ -c usersLinux.cpp switches.h
	rm usersLinux.cpp
clean : 
	rm main.o aes.o md5.o basicIO.o system.o readkey.o abouttime.o ext.o operation.o users.o
