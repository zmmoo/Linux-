# SQLite
- .help
- .quit .exit
- sqlite3 student.db  创建一个数据库
- create table stu(id Integer, name char, socre Integer); 创建一张表格
- 基本的sql命令不以.开头，但以;结尾
- .schema 打印表格结构图
- insert into stu values(1001, 'zhang', 80);  插入数据
- insert into stu values(1002, "li", 90);
- select * from stu; 查看表stu中所有内容
- insert into stu (name, socre)values(1003,"wang"); 插入部分字段
- select name from stu 查询name字段 select name,socre from stu
- select * from stu where socre=80;
- select * from stu where socre=80 and（or） name='zhangsan';
- delete from stu where id=1003 or（and）.....; 删除id为1003的数据 不加where会删除所有数据
- update stu set name='wangwu', socre = 80 where id 1001  设置id为1001的name和score
- .datebase 查看带看的数据库
- .table 查看表格
- alter table stu add column address char; 添加address列
- 删除列  
- 1 创建一个表（create table stu1 as select id,name,score from stu） 
- 2  删除原有表格 drop table stu;
- 3  改名字  alter table stu1 rename to stu
# SQLite编程接口
- `int sqlite3_open(const char *filename,sqlite3 **ppDb)`; 成功返回SQLITE_OK 出错错误码
- filename 数据库路径名 
- ppDb 代表的是数据库的操作句柄 
- `int sqlite3_close(sqlite3*);` 关闭数据库
- `const char* sqlite3_errmsg(sqlite3*)`
- `int sqlite3_exec(sqlite3*, /* An open database */`
    `const char *sql,   /* SQL to be evaluated */`
  `int (*callback)(void*,int,char**,char**),  /* Callback function */`
  `void *,                                    /* 1st argument to callback */`
  `char **errmsg                              /* Error msg written here */`
`);`
- db:数据库句柄 sql 一条sql语句  callback 只有为查询时，才会执行此语句
- arg 给回调函数的参数 errmsg 错误信息
- `int (*callback)(void * （参数），argm int(记录中字段的数目) , char** （包含每个字段值得指针数组）,char**(包含每个字段名称的指针数组))` 回调函数
- 查询结果是一个指针类型，传递函数名即可
- 不使用回调函数查询  `int sqlites_get_table(sqlite3 *db, const char *sql,char ***resultp, int *nrow, int ncolumn, )`
- create table usr(name text primary key):primary key:主键　　相同的name 只能出现一次
