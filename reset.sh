#!/bin/bash
mysql -uroot -proot3306 -e "DROP FUNCTION IF EXISTS bitmap;"
mysql -uroot -proot3306 -e "DROP FUNCTION IF EXISTS bitmap_count"
make clean
sudo make uninstall

make && sudo make install
mysql -uroot -proot3306 -e "CREATE AGGREGATE FUNCTION bitmap RETURNS STRING SONAME 'mysql-udf-bitmap.so'"
mysql -uroot -proot3306 -e "CREATE FUNCTION bitmap_count RETURNS INT SONAME 'mysql-udf-bitmap.so'"
