all:
	g++ udf-bitmap.cpp -shared -o mysql-udf-bitmap.so -I /Users/hadoop/git/EWAHBoolArray/headers -I /usr/local/mysql/include -fPIC

clean:
	rm -rf *.so

install:
	cp -f mysql-udf-bitmap.so /usr/local/mysql/lib/plugin/

uninstall:
	rm -rf /usr/local/mysql/lib/plugin/mysql-udf-bitmap.so
