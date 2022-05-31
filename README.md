# Small Scale DBMS 

## Commands implemented
1. Create table
2. Insert values
3. Select
4. Update
5. Delete
6. Drop table
7. Help tables
8. Help commands
9. Describe table
10. 'where' clause in insert, select, update, delete


## Example Commands
create table student ( id INT , name STR , college STR , branch STR )


insert into student values ( 33 , Saurabh , VIT , Comp )

insert into student values ( 46 , Gitesh , VIT , Entc )

insert into student values ( 83 , Rahul , VIT , Mech )


describe student


select * from student

select name , branch from student

select * from student where id = 33


help tables


help createtable

help select


update student set branch = Comp where id = 46

update student set branch = Mech


delete from student where id = 83

delete from student


drop table student
