#!/bin/bash
mysql -uroot -proot3306 -e "DROP FUNCTION IF EXISTS bm;"
mysql -uroot -proot3306 -e "DROP FUNCTION IF EXISTS bm_count"
#mysql -uroot -proot3306 -e "DROP FUNCTION IF EXISTS bm_and_count"
mysql -uroot -proot3306 -e "DROP FUNCTION IF EXISTS bm_detail"
make clean
sudo make uninstall

make && sudo make install
mysql -uroot -proot3306 -e "CREATE AGGREGATE FUNCTION bm RETURNS STRING SONAME 'mysql-udf-bitmap.so'"
mysql -uroot -proot3306 -e "CREATE FUNCTION bm_count RETURNS INT SONAME 'mysql-udf-bitmap.so'"
#mysql -uroot -proot3306 -e "CREATE AGGREGATE FUNCTION bm_and_count RETURNS INT SONAME 'mysql-udf-bitmap.so'"
mysql -uroot -proot3306 -e "CREATE FUNCTION bm_detail RETURNS STRING SONAME 'mysql-udf-bitmap.so'"
