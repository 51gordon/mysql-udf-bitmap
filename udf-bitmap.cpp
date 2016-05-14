#include <string>
#include <string.h>
#include <my_global.h>
#include <my_sys.h>
#include <mysql_com.h>
#include <stdlib.h>
#include "ewah.h"
#include <fstream>
//#include <sstream>
//#include <roaring.h>
//#include <bitset.h>
extern "C" {
	// get bitmap cardinality
	my_bool bitmap_count_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	//void count2_add(UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error);
	void bitmap_count_deinit(UDF_INIT *initid);
	long long bitmap_count(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

	// generate bitmap
	my_bool bitmap_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	void bitmap_clear(UDF_INIT* initid, char* is_null, char *error);
	void bitmap_add(UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error);
	void bitmap_deinit(UDF_INIT *initid);
	char * bitmap(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error);

	// bitmap and operate
	//my_bool bitmap_and_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	//long long bitmap_and(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);

	// bitmap or operate
	//my_bool bitmap_or_init(UDF_INIT *initid, UDF_ARGS *args, char *message);
	//long long bitmap_or(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error);
}

void debug_log(int i)
{
	std::ofstream f;
	f.open("/tmp/mysql-udf.log",std::ios::app);
	f << i << std::endl;
	f.close();
}
void debug_log(std::string str)
{
	std::ofstream f;
	f.open("/tmp/mysql-udf.log",std::ios::app);
	f << str << std::endl;
	f.close();
}
void tofile(EWAHBoolArray<uint32_t> * bitmap)
{
	std::ofstream f;
	f.open("/tmp/bitmap-demo", std::ios::binary);
	bitmap->write(f);
	f.close();
}

my_bool bitmap_count_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
	if (args->arg_count != 1) {
		strcpy(message, "bitmap_count must have exaclty one argument");
		return 1;
	}
  args->arg_type[0] = STRING_RESULT;
	initid->maybe_null = 0;
	initid->ptr = (char *) new EWAHBoolArray<uint32_t>;
	return 0;
}

long long bitmap_count(UDF_INIT *initid, UDF_ARGS *args, char *is_null, char *error)
{
	EWAHBoolArray<uint32_t> * bitmap = (EWAHBoolArray<uint32_t> *)initid->ptr;
	std::string str(args->args[0], args->lengths[0]);
	std::stringstream ss(str);
	bitmap->read(ss);
	return bitmap->numberOfOnes();
}

void bitmap_count_deinit(UDF_INIT *initid)
{
	EWAHBoolArray<uint32_t> * bitmap = (EWAHBoolArray<uint32_t> *)initid->ptr;
	delete bitmap;
}

// ==================================================================

my_bool bitmap_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
	if (args->arg_count != 1) {
    strcpy(message, "most must have exaclty one argument");
    return 1;
  }
  args->arg_type[0] = INT_RESULT;
  initid->maybe_null = 1;
	initid->ptr = (char *) new EWAHBoolArray<uint32_t>;
	return 0; 
}

void bitmap_clear(UDF_INIT* initid, char* is_null, char *error)
{
	EWAHBoolArray<uint32_t> * bitmap = (EWAHBoolArray<uint32_t> *)initid->ptr;
	bitmap->reset();
}

void bitmap_add(UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error)
{
	if (args->args[0])
	{
		int t = *((int*) args->args[0]);
		EWAHBoolArray<uint32_t> * bitmap = (EWAHBoolArray<uint32_t> *)initid->ptr;
		bitmap->set(t);
	}
}

char * bitmap(UDF_INIT *initid, UDF_ARGS *args, char *result, unsigned long *length, char *is_null, char *error)
{
	EWAHBoolArray<uint32_t> * bitmap = (EWAHBoolArray<uint32_t> *)initid->ptr;
	std::stringstream ss;
	bitmap->write(ss);
	std::string str = ss.str();
	char* ca = new char[str.size()+1];
	std::copy(str.begin(), str.end(), ca);
	ca[str.size()] = '\0';
	return ca;	
}

void bitmap_deinit(UDF_INIT *initid)
{
	//EWAHBoolArray<uint32_t> * bitmap = (EWAHBoolArray<uint32_t> *)initid->ptr;
	//delete bitmap;
	delete initid->ptr;
}
