#include "stdafx.h"
#include "actions.h"

//check the date format, returns true if so and false otherwise
bool validateDateFormat(string date) {
	if (date.size() != 10) {
		return false;
	}
	for (int i = 0; i < 10; i++) {
		if ((i == 4 || i == 7) && date[i] != '-') {
			return false;
		}
		else if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) {
			return false;
		}
	}
	return true;
}

//1. Show all available books in inventory
int showBooks() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM books "
			"LEFT JOIN books_inventory ON books.book_id=books_inventory.book_id "
			"WHERE books_inventory.amount > 0; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no books in the inventory" << endl;
		}
		else {
			cout << "Book ID\tName\t\t\tAuthor\t\tPrice\t\t\tBook ID\t\t\tAmount"<< endl;
			while (rset->next()) {
				cout << rset->getUInt("book_id") << "\t" << rset->getString("book_name") << "\t\t\t" << rset->getString("author_name") << "\t\t" << rset->getString("book_price") << "\t\t" << rset->getString("book_id") << "\t\t" << rset->getString("amount") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//2. Show all open orders
int showOrders() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM book_orders "
			"where (book_orders.status_id != 4 AND book_orders.status_id !=5); ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no open orders" << endl;
		}
		else {
			cout << "Order ID\tOrder Date\tCustomer ID\tSupplier ID\tBook ID\tAmount\tTotal Price\tStatus" << endl;
			while (rset->next()) {
				cout << rset->getUInt("order_id") << "\t" << rset->getString("order_date") << "\t" << rset->getUInt("cust_id") << "\t" << rset->getUInt("supp_id") << "\t" << rset->getUInt("book_id") << "\t"  << "\t" << rset->getUInt("amount") << "\t" << rset->getDouble("total_price") << "\t" << rset->getString("status_id") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//3. List of all customers who made a purchase
int showCustomers() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM customers; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no customers who made a purchase" << endl;
		}
		else {
			cout << "Customer ID\Cust name\t\tPhone\t\tJoin Date" << endl;
			while (rset->next()) {
				cout << rset->getUInt("cust_id") << "\t" << rset->getString("cust_name") << "\t" << rset->getString("phone") << "\t" << rset->getString("join_date") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//4. Show all suppliers
int showSuppliers() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM suppliers; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no suppliers" << endl;
		}
		else {
			cout << "SuppID\tSupplier name\tPhone" << endl;
			while (rset->next()) {
				cout << rset->getUInt("supp_id") << "\t" << rset->getString("supp_name") << "\t\t" << rset->getString("phone")  << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//5. Show all purchases between given dates: fromDate & tilDate
int showPurchases(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM purchases "
			"WHERE (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "'); ");

		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no purchases in the given range of dates" << endl;
		}
		else {
			cout << "PurchID\tBookID\tSellerID\tCustomerID\tPurchDate\tCanceled\tPayment" << endl;
			while (rset->next()) {
				cout << rset->getUInt("purch_id") << "\t" << rset->getUInt("book_id") <<  "\t" << rset->getUInt("seller_id") << "\t" << rset->getUInt("cust_id") << "\t" << rset->getString("purch_date") << "\t" << rset->getString("canceled") << "\t" << rset->getDouble("total_after_discount") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//6. Show all books available for global sale
int showBooksOnSale() {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM global_discount; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no books on sale" << endl;
		}
		else {
			cout << "DisID\tDisID\tBookID\tStartDate\tEndDate\tPrice" << endl;
			while (rset->next()) {
				cout << rset->getUInt("discount_id") << "\t" << rset->getString("discount_name") << "\t" << rset->getString("book_id") << "\t" << rset->getDouble("start_date") << "\t" << rset->getDouble("end_date")<< "\t"<< rset->getDouble("price") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//7. Check if a given book: bookTitle + bookAuthor is available in the inventory
int searchBook(string bookName, string bookAuthor) {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT books_inventory.book_id, book_name, author_name, amount "
			"FROM ( "
			"SELECT book_id, book_name, author_name "
			"FROM books "
			"WHERE book_name = '" + bookName+ "' AND author_name = '" + bookAuthor + "' "
			") AS search_book "
			"LEFT JOIN books_inventory ON search_book.book_id = books_inventory.book_id; ");

		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There is no book with matching title and author in the database, therefore the amount in the inventory is 0" << endl;
		}
		else {
			rset->next();
			cout << rset->getString("book_name") << " by " << rset->getString("author_name") << " has " << rset->getUInt("amount") << " copies in the inventory" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//8. List of all suppliers of a given book: bookTitle + bookAuthor
int showSuppliersOfBook(string bookName, string bookAuthor) {
	Database &db = Database::getInstance();
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT suppliers.supp_id, supp_name, phone, book_id, supplier_price "
			"FROM ( "
			"SELECT book_suplliers_prices.book_id, supp_id, supplier_price "
			"FROM ( "
			"SELECT book_id "
			"FROM books "
			"WHERE book_name = '" + bookName + "' AND author_name = '" + bookAuthor + "' "
			") AS search_book "
			"LEFT JOIN book_suplliers_prices  ON search_book.book_id = book_suplliers_prices.book_id "
			") AS supplie_books "
			"LEFT JOIN suppliers ON suppliers.supp_id = supplie_books.supp_id; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There is no book with matching title and author in the database" << endl;
		}
		else {
			cout << "Supplier ID\tSupplier name\tPhone\t\tBook ID\tPrice" << endl;
		}
		while (rset->next()) {
			cout << rset->getUInt("supp_id") << "\t" << rset->getString("supp_name") << "\t" << rset->getString("phone") << "\t" << rset->getUInt("book_id") << "\t" << rset->getDouble("supplier_price") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//9. How many books from type: bookTitle + bookAuthor where sold from date fromDate
int booksSold(string bookName, string bookAuthor, string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(book_id) AS books_sold, book_name, author_name "
			"FROM ( "
			"SELECT purch_id, purchases.book_id, book_name, author_name, purch_date, canceled "
			"FROM ( "
			"SELECT book_id, book_name, author_name "
			"FROM books "
			"WHERE book_name = '" + bookName + "' AND author_name = '" + bookAuthor + "' "
			") AS search_book "
			"LEFT JOIN purchases ON purchases.book_id = search_book.book_id "
			"WHERE purch_date >= '2018-01-01' AND canceled = false "
			") AS books_purch; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("book_name") || rset->isNull("author_name")) {
			cout << "No such book has been sold" << endl;
		}
		else {
			cout << rset->getString("book_name") << " by " << rset->getString("author_name") << " sold " << rset->getInt("books_sold") << " copies since " << fromDate << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//10. Show How Many Books A Client Has Bought Since A Given Date
int booksPurchased(unsigned int custID, string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(book_id) AS total_books, COUNT(DISTINCT book_id) AS differant_books "
			"FROM ( "
			"SELECT * "
			"FROM purchases "
			"WHERE cust_id = '" + to_string(custID) + "' AND canceled = false AND purch_date >= '" + fromDate + "' "
		") AS cust_purch; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getInt("total_books") == 0) {
			cout << "There are no purchases made by the customer" << endl;
		}
		else {
			cout << "The customer made " << rset->getInt("total_books") << " purchases, and bought " << rset->getInt("differant_books") << " differant books" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//11. Show the customer details who bought the most since given date fromDate
int mostPurchasesCustomer(string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT best_customer.cust_id, book_amount, cust_name, phone "
			"FROM ( "
			"SELECT * "
			"FROM ( "
			"SELECT cust_id, COUNT(cust_id) book_amount "
			"FROM ( "
			"SELECT * "
			"FROM purchases "
			"WHERE canceled = false AND purch_date >= '" + fromDate + "' "
			") AS purch "
			"GROUP BY cust_id "
			") AS custs_books "
			"ORDER BY book_amount DESC "
			"LIMIT 1 "
			") AS best_customer "
			"LEFT JOIN customers ON customers.cust_id = best_customer.cust_id; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no customers who made a purchase" << endl;
		}
		else {
			cout << "Customer ID\tCust name\tPhone\tAmount" << endl;
			while (rset->next()) {
				cout << rset->getUInt("cust_id") << "\t" << rset->getString("cust_name") << "\t" << rset->getString("phone") << "\t"  << rset->getUInt("book_amount") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//12. show the supplier details who sold us the most books since given date fromDate
int mostOrdersSuplier(string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement(
			"SELECT amount, best_supp.supp_id, supp_name, phone "
			"FROM ( "
			"SELECT * "
			"FROM ( "
			"SELECT supp_id, SUM(amount) AS amount "
			"FROM book_orders "
			"WHERE order_date >= '" + fromDate + "' AND  status_id != 5 "
			"GROUP BY supp_id "
			") AS supps_book_amounts "
			"ORDER BY amount DESC "
			"LIMIT 1 "
			") AS best_supp "
			"LEFT JOIN suppliers ON suppliers.supp_id = best_supp.supp_id; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There are no orders from any supplier" << endl;
		}
		else {
			cout << "Supplier ID\tSupplier name\tPhone\t\tAmount" << endl;
			while (rset->next()) {
				cout << rset->getUInt("supp_id") << "\t" << rset->getString("supp_name") << "\t" << rset->getString("phone") << "\t" << rset->getUInt("amount") << endl;
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//13. amount of orders made (and how many books?) between given dates: fromDate & tilDate
int ordersMade(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount "
			"FROM ( "
			"SELECT * "
			"FROM book_orders "
			"	WHERE (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND status_id != 5 "
			") AS orders_amount_by_book; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getInt("orders_amount") == 0) {
			cout << "No orders were made" << endl;
		}
		else {
			cout << rset->getInt("orders_amount") << " orders were made, and " << rset->getInt("books_amount") << " books were ordered" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//14. amount of orders made between given dates that were made by customers and were sold
int ordersMadePurchases(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount "
			"FROM ( "
			"SELECT * "
			"FROM book_orders "
			"WHERE (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND status_id = 4 "
			") AS orders_amount_by_book; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getInt("orders_amount") == 0) {
			cout << "No orders were made by customers" << endl;
		}
		else {
			cout << rset->getInt("orders_amount") << " orders were made, and " << rset->getInt("books_amount") << " books were sold" << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//15. show the total discount a customer received since a certain date
int totalDiscountCustomer(unsigned int custID, string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT SUM(discount) AS total_disc "
			"FROM ( "
			"SELECT full_book_price -total_after_discount AS discount "
			"FROM ( "
			"SELECT * "
			"FROM purchases "
			"WHERE purch_date >= '" + fromDate + "' AND cust_id = '" + to_string(custID) + "' AND canceled = false "
			") AS cust_disc "
			") AS disc_sum; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("total_disc")) {
			cout << "No discount for the custmoer" << endl;
		}
		else {
			cout << "The total discount the customer received is " << rset->getInt("total_disc") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//16. sum revenue in Q1, Q2, Q3, Q4 in a given year: y
int sumRevenue(string y) {
	Database &db = Database::getInstance();
	if (y.size() != 4) {
		cout << "The year doesn't follow the right format - it should be 4 digits" << endl;
		return 1;
	}
	for (int i = 0; i < 4; i++) {
		if (y[i] < '0' || y[i] > '9') {
			cout << "The year doesn't follow the right format - it should be 4 digits" << endl;
			return 1;
		}
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM ( "
			"SELECT * "
			"FROM ( "
			"	SELECT SUM(total_after_discount) AS total_Q1 "
			"	FROM purchases "
			"WHERE (purch_date BETWEEN '" + y + "-01-01' AND '" + y + "-03-31') AND canceled = false "
			") AS Q1 "
			"LEFT JOIN ( "
			"SELECT * "
			"   FROM ( "
			"		SELECT SUM(total_after_discount) AS total_Q2 "
			"		FROM purchases "
			"		WHERE (purch_date BETWEEN '" + y + "-04-01' AND '" + y + "-06-30') AND canceled = false "
			"		) AS Q2 "
			") Q1Q2 ON (total_Q1 OR total_Q1 IS NULL) "
			"LEFT JOIN ( "
			"SELECT * "
			"FROM ( "
			"	SELECT SUM(total_after_discount) AS total_Q3 "
			"	FROM purchases "
			"	WHERE (purch_date BETWEEN '" + y + "-07-01' AND '" + y + "-09-30') AND canceled = false "
			"	) AS Q3 "
			") Q1Q2Q3 ON (total_Q1 OR total_Q1 IS NULL) "
			"LEFT JOIN ( "
			"SELECT * "
			"FROM ( "
			"SELECT SUM(total_after_discount) AS total_Q4 "
			"FROM purchases "
			"WHERE (purch_date BETWEEN '" + y + "-10-01' AND '" + y + "-12-31') AND canceled = false "
			") AS Q4 "
			" ) Q1Q2Q3Q4 ON (total_Q1 OR total_Q1 IS NULL) "
			") AS revenue; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("total_Q1") && rset->isNull("total_Q2") && rset->isNull("total_Q3") && rset->isNull("total_Q4")) {
			cout << "The store didn't operate in that year" << endl;
		}
		else {
			cout << "the total revenue is:" << endl;
			for (unsigned int i = 1; i <= 4; i++) {
				cout << "Q" << i << ": ";
				if (rset->isNull(i)) {
					cout << "0" << endl;
				}
				else {
					cout << rset->getUInt(i) << endl;
				}
			}
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//17. Show how many customers were added from given date: fromDate
int customersAdded(string fromDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT COUNT(cust_id) AS new_custs "
			"FROM ( "
			"select * "
			"FROM customers "
			") AS new_cust "
			"WHERE (join_date>= '" + fromDate + "'); ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->getUInt("new_custs") == 0) {
			cout << "There are no new customers" << endl;
		}
		else {
			cout << "The number of new customers since " + fromDate + " is " << rset->getInt("new_custs") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//18. total amount paid to a given supplier: suppID, between given dates: fromDate, tilDate
int totalPaidSupplier(unsigned int suppID, string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("select SUM(total_price) AS total_price "
			"FROM ( "
			"SELECT * "
			"FROM book_orders "
			"WHERE supp_id = '" + to_string(suppID) + "' AND (order_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND status_id != 5 "
			") AS supp_orders; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("total_price")) {
			cout << "There were no orders from the supplier" << endl;
		}
		else {
			cout << "The total amount paid to the supplier is " << rset->getInt("total_price") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//19. total amount a given seller: sellID earned between given dates: fromDate, tilDate
int totalSoldSeller(unsigned int sellID, string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT SUM(total_after_discount) AS total_for_seller "
			"FROM ( "
			"SELECT * "
			"FROM purchases "
			"WHERE seller_id = '" + to_string(sellID) + "' AND (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND canceled = false "
			") AS seller_purch; ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		rset->next();
		if (rset->isNull("total_for_seller")) {
			cout << "There are no purchases with that seller" << endl;
		}
		else {
			cout << "The total amount is " << rset->getInt("total_for_seller") << endl;
		}
		while (rset->next()) {
			cout << rset->getUInt("cust_id") << "\t" << rset->getString("first_name") << "\t" << rset->getString("last_name") << rset->getString("phone") << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}

//20. top 10 most sold books between given dates: fromDate, tilDate
int top10Books(string fromDate, string tilDate) {
	Database &db = Database::getInstance();
	if (!validateDateFormat(fromDate) || !validateDateFormat(tilDate)) {
		cout << "The date doesn't follow the right format - it should be yyyy-mm-tt" << endl;
		return 1;
	}
	try {
		Connection *con = db.getConnection();
		PreparedStatement *pstmt = con->prepareStatement("SELECT * "
			"FROM ( "
			"	SELECT books.book_id, book_name, author_name, amount "
			"	FROM ( "
			"		SELECT book_id, COUNT(*) AS amount "
			"		FROM purchases "
			"		WHERE (purch_date BETWEEN '" + fromDate + "' AND '" + tilDate + "') AND canceled = false  "
			"		GROUP BY book_id "
			"	) AS count_amount "
			"   LEFT JOIN books ON books.book_id = count_amount.book_id "
			") AS top_10 "
			"ORDER BY amount DESC "
			"LIMIT 10;   ");
		ResultSet *rset = pstmt->executeQuery();
		rset->beforeFirst();
		if (rset->rowsCount() == 0) {
			cout << "There were no purchase in that time" << endl;
		}
		else {
			cout << "Book ID\tBookName\tAuthor name" << endl;
		}
		while (rset->next()) {
			cout << rset->getUInt("book_id") << "\t" << rset->getString("book_name") << "\t" << rset->getString("author_name")  << endl;
		}
		delete pstmt;
		delete rset;
		delete con;
		return 0;
	}
	catch (SQLException &e) {
		cout << e.what();
	}
}