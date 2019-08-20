CC=gcc
CFLAGS=-I/usr/include/mysql -I/usr/include/mysql/mysql -L/usr/lib/ -lmariadb -ldl -lm -lpthread -lssl -lcrypto


main : sesh.o main.o util.o sql.o config.h sesh.h util.h sql.h
	$(CC) $(CFLAGS) -o main main.o sesh.o sql.o util.o config.h sesh.h util.h

util.o : util.c config.h
	$(CC) -c util.c

sql.o  : sql.c config.h util.h
	$(CC) $(CFLAGS) -c sql.c util.h

sesh.o : sesh.c config.h util.h
	$(CC) -c sesh.c util.h

main.o : main.c config.h sesh.h util.h sql.h
	$(CC) $(CFLAGS) -c main.c config.h sesh.h util.h sql.h
	$(CC) $(CFLAGS) -o wtf wtf.c

install :
	test -f main || make
	install -m 755 main /srv/www/cgi/

	ln -svf /srv/www/cgi/main /srv/www/cgi/newuser
	ln -svf /srv/www/cgi/main /srv/www/cgi/newevent
	ln -svf /srv/www/cgi/main /srv/www/cgi/newdbs
	ln -svf /srv/www/cgi/main /srv/www/cgi/cleardbs
	ln -svf /srv/www/cgi/main /srv/www/cgi/delevt
	ln -svf /srv/www/cgi/main /srv/www/cgi/setapiky

	touch /var/log/cgi.log
	chmod -v 666 /var/log/cgi.log
        chown apache:apache /var/log/cgi.log

uninstall :
	rm -rfv /srv/www/cgi/main
	rm -rfv /srv/www/cgi/{newuser,newevent,newdbs,cleardbs,delevt,setapiky}

clean :
	rm sql.o util.o sesh.o main.o main *.gch wtf
