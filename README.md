#当前状态
由于刚刚开发完成，尚未进行严格的测试，请不要用在生产环境！
***
#什么是Bitmap
所谓bitmap就是用一个bit位来标记某个元素对应的value，而key即是这个元素。由于采用bit为单位来存储数据，因此在可以大大的节省存储空间，通过对bitmap的and/or等操作，实现数据的快速交并等运算。
***
#为什么要使用Bitmap
在数据统计工作中，我们经常遇到这样的问题，在一张统计表中，有部分指标列是可以简单求和的，但是有些列不可以，看下面的例子(假设从5月1日开放注册和充值)：

| 日期    | 注册用户数 | 充值用户数|
| -------- |:----------:|:-----:|
| 20160501 | 2        | 1     |
| 20160502 | 3        | 2     |
| 20160503 | 2        | 3     |

需求：计算5月2~4日总的注册用户数和充值用户数。
总的注册用户数：对注册用户数做sum即可
总的充值用户数：这时我们不能做sum了，因为这种指标是需要排重的，用户在3天内可能发生多次充值，但是在计算时是能算1此，怎么解决呢，接着往下看。

| 日期     | 注册用户数 | 充值用户bitmap|
|----------|:----------:|:--------|
| 20160501 | 2        | 01         |
| 20160502 | 3        | 01010      |
| 20160503 | 3        | 10100010   |

20160501：01中每一位表示一个用户，那么其表示第2位注册用户有充值

20160502：01010表示第2和4位注册用户有充值

20160503：1010001101表示第1、3、7位注册用户有充值

这时我们对充值用户bitmap做or运算（有一位是1，当前位置即为1），即可得出11110010，求出其中1的个数为5，即这三天总的充值用户数。

注：mysql的原生类型bit也可以实现类似的功能，但是由于bit最大只支持64位，意味着01串最大只能到64位，无法满足我们的需求。
***
#使用教程
1.	克隆EWAHBoolArray，这里的bitmap使用的是lemire的EWAHBoolArray

	传送门：https://github.com/lemire/EWAHBoolArray

2.	编辑makefile,指定你自己的EWAHBoolArray目录和mysql目录

3.	编译安装so库，make && make install

4.	将自定义的函数加入到mysql（需要有mysql.func的insert权限）

		CREATE FUNCTION bm_count RETURNS INT SONAME 'mysql-udf-bitmap.so'
	
		CREATE FUNCTION bm_detail RETURNS STRING SONAME 'mysql-udf-bitmap.so'
	
		CREATE AGGREGATE FUNCTION bm RETURNS STRING SONAME 'mysql-udf-bitmap.so'
	
		CREATE AGGREGATE FUNCTION bm_and RETURNS STRING SONAME 'mysql-udf-bitmap.so'
	
		CREATE AGGREGATE FUNCTION bm_or RETURNS STRING SONAME 'mysql-udf-bitmap.so'
	
		CREATE AGGREGATE FUNCTION bm_and_count RETURNS INT SONAME 'mysql-udf-bitmap.so'
	
		CREATE AGGREGATE FUNCTION bm_or_count RETURNS INT SONAME 'mysql-udf-bitmap.so'
5.	函数介绍

	| 函数名 | 功能 | 返回值 |
	|--------------|:------------------------------------|:--------:|
	| bm_count     |计算bitmap中1的个数                    |     INT     |
	| bm_detail    |展示bitmap详情,将位置为1的索引按逗号分隔  |   STRING   |
	| bm           |对原始数据生成bitmap                   |  STRING  |
	| bm_and       |对多个bitmap做与运算                   |  STRING  |
	| bm_or        |对多个bitmap做或运算                   |  STRING  |
	| bm_and_count |对多个bitmap做与运算后求出1的个数        |  INT  |
	| bm_or_count  |对多个bitmap做或运算后求出1的个数        |  INT  |

***
#举例说明
明天再写
			



