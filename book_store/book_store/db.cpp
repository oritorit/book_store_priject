#include "stdafx.h"
#include "db.h"

Database *Database::instance = 0;

Database::Database() : driver(get_driver_instance()) {
	connection_properties["hostName"] = DB_HOST;
	connection_properties["port"] = DB_PORT;
	connection_properties["userName"] = DB_USER;
	connection_properties["password"] = DB_PASS;
	connection_properties["OPT_RECONNECT"] = true;

	// use database, create tables and insert values.
	try {
		Connection *con = driver->connect(connection_properties);
		try {
			con->setSchema("book_store");
		}
		catch (SQLException &e) {
			Statement *stmt = con->createStatement();
			stmt->execute("CREATE DATABASE IF NOT EXISTS book_store;");
			// "USE book_store;");
			con->setSchema("book_store"); // switch database
			stmt->execute("CREATE TABLE IF NOT EXISTS books ("
				"book_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"book_name VARCHAR(100),"
				"author_name VARCHAR(100),"
				"book_price DOUBLE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS global_discount ("
				"discount_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"discount_name VARCHAR(100),"
				"book_id INT UNSIGNED,"
				"FOREIGN KEY (book_id) REFERENCES books(book_id),"
				"start_date DATE,"
				"end_date DATE,"
				"price_after_discount DOUBLE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS books_inventory ("
				"book_id INT UNSIGNED,"
				"FOREIGN KEY (book_id) REFERENCES books(book_id),"
				"amount INT UNSIGNED"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS suppliers ("
				"supp_id INT UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY,"
				"supp_name VARCHAR(100),"
				"phone VARCHAR(10)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS book_suplliers_prices ("
				"book_id INT UNSIGNED,"
				"supp_id INT UNSIGNED,"
				"FOREIGN KEY (supp_id) REFERENCES suppliers(supp_id),"
				"FOREIGN KEY (book_id) REFERENCES books(book_id),"
				"PRIMARY KEY (supp_id, book_id),"
				"supplier_price DOUBLE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS customers ("
				"cust_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"cust_name VARCHAR(100),"
				"phone VARCHAR(10),"
				"join_date DATE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS customers_10prec_discount ("
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY (cust_id) REFERENCES customers(cust_id),"
				"PRIMARY KEY (cust_id),"
				"start_date DATE,"
				"end_date DATE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS sellers ("
				"seller_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"seller_name VARCHAR(100)"
				"); ");
			stmt->execute("CREATE TABLE IF NOT EXISTS purchases ("
				"purch_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"book_id INT UNSIGNED,"
				"seller_id INT UNSIGNED,"
				"cust_id INT UNSIGNED,"
				"FOREIGN KEY (book_id) REFERENCES books(book_id),"
				"FOREIGN KEY (seller_id) REFERENCES sellers(seller_id),"
				"FOREIGN KEY (cust_id) REFERENCES customers(cust_id),"
				"purch_date DATE,"
				"canceled BOOL,"
				"full_book_price DOUBLE,"
				"total_after_discount DOUBLE"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS status_of_order ("
				"status_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"status_desc VARCHAR(100)"
				");");
			stmt->execute("CREATE TABLE IF NOT EXISTS book_orders ("
				"order_id INT UNSIGNED NOT NULL PRIMARY KEY,"
				"book_id INT UNSIGNED,"
				"supp_id INT UNSIGNED,"
				"cust_id INT UNSIGNED,"
				"status_id INT UNSIGNED,"
				"FOREIGN KEY (book_id) REFERENCES books(book_id),"
				"FOREIGN KEY (supp_id) REFERENCES suppliers(supp_id),"
				"FOREIGN KEY (cust_id) REFERENCES customers(cust_id),"
				"FOREIGN KEY (status_id) REFERENCES status_of_order(status_id),"
				"amount INT UNSIGNED,"
				"total_price DOUBLE,"
				"order_date DATE"
				");");

			stmt->execute("INSERT INTO books (book_id, book_name, author_name, book_price)"
				"VALUES"
				"(1,'In Search of Lost Time','Marcel Proust',1200),"
				"(2,'Don Quixote','Miguel de Cervantes',80),"
				"(3,'Ulysses','James Joyce',90),"
				"(4,'War and Peace','Leo Tolstoy',250),"
				"(5,'The Odyssey','Homer',120),"
				"(6,'Lolita','Vladimir Nabokov',100),"
				"(7,'Never Let Me Go','Kazuo Ishiguro',80),"
				"(8,'Wolf Hall','Hilary Mantel',120),"
				"(9,'War and Turpentine','Stefan Hertmans',100),"
				"(10,'In the Darkroom','Susan Faludi',78),"
				"(11,'The Impossible Fortress','Jason Rekulak',59),"
				"(12,'Hunger','Roxane Gay',1300),"
				"(13,'The Great Alone','Kristin Hannah',115),"
				"(14,'An American Marriage','Tayari Jones',130),"
				"(15,'The Wife Between Us','Greer Hendricks',120),"
				"(16,'The Outsider','Stephen King',95),"
				"(17,'Then She Was Gone','Lisa Jewell',120);");
			stmt->execute("INSERT INTO global_discount (discount_id,discount_name,book_id,start_date,end_date,price_after_discount)"
				"VALUES"
				"(1,'2018 sale-10%',15,'2018-01-01','2018-12-31',108),"
				"(2,'2018 summer sale-20%',5,'2018-06-01','2018-08-30',96),"
				"(3,'July-August 2018 sale-30%',3,'2018-07-01','2018-08-30',63);");
			stmt->execute("INSERT INTO books_inventory(book_id,amount)"
				"VALUES"
				"(1,8),"
				"(2,19),"
				"(3,20),"
				"(4,14),"
				"(5,30),"
				"(6,20),"
				"(7,3),"
				"(8,0),"
				"(9,22),"
				"(10,1),"
				"(11,33),"
				"(12,0),"
				"(13,20),"
				"(14,20),"
				"(15,0),"
				"(16,12),"
				"(17,0);");
			stmt->execute("INSERT INTO suppliers (supp_id,supp_name,phone)"
				"VALUES"
				"(1,'Booking','0546456822'),"
				"(2,'Zomet sfarim','0546456823'),"
				"(3,'Book4you','0546456844'),"
				"(4,'Best books','0548456988'),"
				"(5,'SFARIM','089658654'),"
				"(6,'Stimatski','035487452'),"
				"(7,'Book for life','086954526'),"
				"(8,'Karavitz','0391113377'),"
				"(9,'Books','038542512'),"
				"(10,'Sefer','0504568266');");
			stmt->execute("INSERT INTO book_suplliers_prices (book_id,supp_id,supplier_price)"
				"VALUES"
				"(1,1,900),"
				"(1,4,800),"
				"(1,3,950),"
				"(2,7,50),"
				"(2,5,35),"
				"(2,2,55),"
				"(2,8,30),"
				"(2,9,35),"
				"(3,6,50),"
				"(3,7,20),"
				"(3,8,60),"
				"(3,9,45),"
				"(4,10,150),"
				"(5,10,50),"
				"(6,3,35),"
				"(6,4,55),"
				"(6,5,35),"
				"(6,6,60),"
				"(7,1,30),"
				"(7,2,55),"
				"(7,3,60),"
				"(7,4,55),"
				"(7,5,30),"
				"(7,8,20),"
				"(8,6,90),"
				"(9,7,90),"
				"(9,9,35),"
				"(10,7,35),"
				"(10,8,30),"
				"(11,8,20),"
				"(12,9,30),"
				"(12,10,1000),"
				"(12,5,900),"
				"(12,2,1200),"
				"(13,6,50),"
				"(14,6,90),"
				"(14,2,30),"
				"(14,3,30),"
				"(15,6,30),"
				"(15,5,55),"
				"(15,2,60),"
				"(16,9,55),"
				"(17,10,50),"
				"(17,1,80);");

			stmt->execute("INSERT INTO customers (cust_id,cust_name,phone,join_date)"
				"VALUES"
				"(1,'Sarit Boter','0546456855','2017-01-01'),"
				"(2,'Eldar Levi','0548458211','2017-05-25'),"
				"(3,'Maya Gross','0532288374','2017-09-13'),"
				"(4,'Shani Cohen','0559999247','2018-01-01'),"
				"(5,'Shani levi','0524266667','2018-01-11'),"
				"(6,'Shon Grin','0549828323','2018-02-01'),"
				"(7,'Mor Fridman','0542559211','2018-03-12'),"
				"(8,'Eyal Michael','0562583588','2018-06-01'),"
				"(9,'Pnina Menashe','0534254854','2018-06-07'),"
				"(10,'Dalit Cohen','0534222262','2018-06-21');");
			stmt->execute("INSERT INTO customers_10prec_discount (cust_id,start_date,end_date)"
				"VALUES"
				"(1,'2017-01-01','2017-12-31'),"
				"(2,'2017-05-25','2017-12-31'),"
				"(9,'2018-06-07','2018-12-31');");
			stmt->execute("INSERT INTO sellers(seller_id,seller_name)"
				"VALUES"
				"(1,'Orit Binyamin'),"
				"(2,'Or Hadad'),"
				"(3,'Edi Binyamin'),"
				"(4,'Natali Chen');");
			stmt->execute("INSERT INTO purchases (purch_id,book_id,seller_id,cust_id,purch_date,canceled,full_book_price,total_after_discount)"
				"VALUES"
				"(1,1,1,1,'2017-01-01',false,1200,1200),"
				"(2,3,1,1,'2017-03-01',false,90,81),"
				"(3,12,3,2,'2017-05-25',false,1300,1300),"
				"(4,3,1,2,'2017-05-30',true,90,81),"
				"(5,4,3,2,'2017-06-01',false,250,225),"
				"(6,13,2,3,'2017-09-13',false,115,115),"
				"(7,8,2,3,'2017-09-27',false,120,120),"
				"(8,9,4,4,'2018-01-01',false,100,100),"
				"(9,3,1,5,'2018-01-11',false,90,90),"
				"(10,2,2,6,'2018-02-01',false,80,80),"
				"(11,15,1,7,'2018-03-12',false,120,108),"
				"(12,5,1,8,'2018-06-01',false,120,96),"
				"(13,1,3,9,'2018-06-07',false,1200,1200),"
				"(14,14,1,9,'2018-06-08',false,130,117),"
				"(15,15,1,10,'2018-06-21',false,120,108),"
				"(16,3,1,2,'2018-07-05',false,90,63);");
			stmt->execute("INSERT INTO status_of_order (status_id,status_desc)"
				"VALUES"
				"(1,'order'),"
				"(2,'In store'),"
				"(3,'Sent SMS to customer'),"
				"(4,'Sold'),"
				"(5,'canceld');");
			stmt->execute("INSERT INTO book_orders (order_id,book_id,supp_id,cust_id,status_id,amount,total_price,order_date)"
				"VALUES"
				"(1,3,6,1,4,1,50,'2017-01-01'),"
				"(2,5,10,2,4,1,50,'2017-05-30'),"
				"(3,8,6,3,4,1,90,'2017-09-13'),"
				"(4,10,7,2,5,1,35,'2017-10-10'),"
				"(5,14,6,9,4,1,90,'2018-06-07'),"
				"(6,10,8,10,2,2,30,'2018-07-30'),"
				"(7,15,6,1,1,1,30,'2018-08-01'),"
				"(8,17,10,5,1,1,50,'2018-08-02');");
	
			delete stmt;
		}
		delete con;
	}
	catch (SQLException &e) {
		cout << e.getErrorCode() << " " << e.what() << " " << e.getSQLState();
	}
}

Database & Database::getInstance() {
	if (Database::instance == 0) {
		instance = new Database();
	}
	return *instance;
}

Connection * Database::getConnection() {
	try {
		Connection *con = driver->connect(connection_properties);
		con->setSchema(DB_NAME);
		return con;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
	return 0;
}