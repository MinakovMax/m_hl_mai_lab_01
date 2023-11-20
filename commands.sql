use archdb; 
show tables;
CREATE TABLE IF NOT EXISTS `Author` (`id` INT NOT NULL AUTO_INCREMENT,`first_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`last_name` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NOT NULL,`email` VARCHAR(256) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,`title` VARCHAR(1024) CHARACTER SET utf8 COLLATE utf8_unicode_ci NULL,PRIMARY KEY (`id`),KEY `fn` (`first_name`),KEY `ln` (`last_name`));
describe Author;
select * from Author where id<10;
select id,first_name,last_name from Author where id>=10 and id<20;
explain select * from Author where id=10;
explain select * from Author where first_name='Rhianna';
explain select * from Author where title='Cashaier';
explain select * from Author where id>10 and id<20;
explain select * from Author where first_name='Elle%';
insert into Author (first_name,last_name,email,title) values ('Иван','Иванов','ivanov@yandex.ru','господин');
SELECT LAST_INSERT_ID();
select * from Author where id=LAST_INSERT_ID();
delete from Author where id= 100001;
show index from Author;
drop index email_hash on Author;
create index email_hash USING HASH  on Author(email);
explain select * from Author where email='Chad_Cavanagh1998@nickia.com';
drop index fn_ln on Author;
create index fn_ln using btree on Author(first_name,last_name);
drop index ln_fn on Author;
create index ln_fn using btree on Author(last_name,first_name);
explain format=json select * from Author where first_name='Elle%' and last_name='A%';

DROP Table `Author`

/* Реализация домашнео задания */
use archdb; 
show tables;

CREATE TABLE Visit (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,user_id INT,datetime DATETIME);
CREATE TABLE Temperature (id INT NOT NULL AUTO_INCREMENT PRIMARY KEY,user_id INT,value DECIMAL(6,2),datetime DATETIME);

DESCRIBE User;
insert into User (last_name,first_name,,email,login,password,title) values ('Ivanov','Ivan','ivanov@yandex.ru','ivanov','mypass','Mr');
insert into User (last_name,first_name,,email,login,password,title) values ('Petrov','Petr','petrovv@yandex.ru','petrovv','pass1','Ser');

insert into Visit (user_id,datetime) values (1,CURRENT_DATE());
insert into Visit (user_id,datetime) values (2,CURRENT_DATE());

insert into Temperature (user_id,value,datetime) values (2,20.2,CURRENT_DATE());

insert into Temperature (user_id,value,datetime) values (1,18.2,CURRENT_DATE());

UPDATE User SET last_name = 'Ivanov', first_name = 'Ivan', email = 'ivanov@yandex.ru', login = 'ivanov', password = 'mypass', title = 'Mr' WHERE id = 1;
UPDATE User SET last_name = 'Petrov', first_name = 'Petr', email = 'petrovv@yandex.ru', login = 'petrovv', password = 'pass1', title = 'Ser' WHERE id = 2;

SELECT datetime, first_name, last_name, login, password, email, title FROM `Visit` as v JOIN `User` as u ON v.user_id = u.id 

SELECT datetime, user_id, first_name, last_name, login, password, email, title FROM `Visit` AS v JOIN `User` AS u ON v.user_id = u.id

SELECT id, user_id, datetime FROM Visit