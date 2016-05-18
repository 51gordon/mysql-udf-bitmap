#!/bin/bash
function execute(){
	sql="$1"
	mysql3307 -uroot -proot3307 -P3307 -e "$sql"
}
execute "DROP FUNCTION IF EXISTS bm_count"
execute "DROP FUNCTION IF EXISTS bm_detail"
execute "DROP FUNCTION IF EXISTS bm;"
execute "DROP FUNCTION IF EXISTS bm_and"
execute "DROP FUNCTION IF EXISTS bm_or"
execute "DROP FUNCTION IF EXISTS bm_and_count"
execute "DROP FUNCTION IF EXISTS bm_or_count"

make clean
sudo make uninstall

make && sudo make install
execute "CREATE FUNCTION bm_count RETURNS INT SONAME 'mysql-udf-bitmap.so'"
execute "CREATE FUNCTION bm_detail RETURNS STRING SONAME 'mysql-udf-bitmap.so'"
execute "CREATE AGGREGATE FUNCTION bm RETURNS STRING SONAME 'mysql-udf-bitmap.so'"
execute "CREATE AGGREGATE FUNCTION bm_and RETURNS INT SONAME 'mysql-udf-bitmap.so'"
execute "CREATE AGGREGATE FUNCTION bm_or RETURNS INT SONAME 'mysql-udf-bitmap.so'"
execute "CREATE AGGREGATE FUNCTION bm_and_count RETURNS INT SONAME 'mysql-udf-bitmap.so'"
execute "CREATE AGGREGATE FUNCTION bm_or_count RETURNS INT SONAME 'mysql-udf-bitmap.so'"
