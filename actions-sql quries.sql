/*Action1*/
SELECT *
FROM books
LEFT JOIN books_inventory ON books.book_id=books_inventory.book_id
WHERE books_inventory.amount > 0;

/*Action2*/
SELECT *
FROM book_orders
where (book_orders.status_id != 4 AND book_orders.status_id !=5);

/*Action3*/
SELECT *
FROM customers

/*Action4*/
SELECT *
FROM suppliers 

/*Action5*/
SELECT *
FROM purchases
WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate')

/*Action6*/
SELECT *
FROM global_discount

/*Action7*/
SELECT books_inventory.book_id, book_name, author_name, amount
FROM (
	SELECT book_id, book_name, author_name
	FROM books
	WHERE book_name = 'bookName' AND author_name = 'bookAuthor'
) AS search_book
LEFT JOIN books_inventory ON search_book.book_id = books_inventory.book_id;
       
       /*Action8*/
       SELECT suppliers.supp_id, supp_name, phone, book_id, supplier_price
FROM (
	SELECT book_suplliers_prices.book_id, supp_id, supplier_price
	FROM (
		SELECT book_id
		FROM books
		WHERE book_name = 'bookName' AND author_name = 'bookAuthor'
	) AS search_book
	LEFT JOIN book_suplliers_prices  ON search_book.book_id = book_suplliers_prices.book_id
) AS supplie_books
LEFT JOIN suppliers ON suppliers.supp_id = supplie_books.supp_id;

/*Action9*/
SELECT COUNT(book_id) AS books_sold, book_name, author_name
FROM (
	SELECT purch_id, purchases.book_id, book_name, author_name, purch_date, canceled
	FROM (
		SELECT book_id, book_name, author_name
		FROM books
		WHERE book_name = 'bookName' AND author_name = 'bookAuthor'
	) AS search_book
	LEFT JOIN purchases ON purchases.book_id = search_book.book_id
	WHERE purch_date >= 'fromDate' AND canceled = false
) AS books_purch;

/*Action10*/
SELECT COUNT(book_id) AS total_books, COUNT(DISTINCT book_id) AS differant_books
FROM (
	SELECT *
	FROM purchases
	WHERE cust_id ='custID' AND canceled = false AND purch_date >= 'fromDate'
) AS cust_purch;

/*Action11*/
SELECT best_customer.cust_id, book_amount, cust_name, phone
FROM (
	SELECT *
	FROM (
		SELECT cust_id, COUNT(cust_id) book_amount
		FROM (
			SELECT *
			FROM purchases
			WHERE canceled = false AND purch_date >= 'fromDate'
		) AS purch
		GROUP BY cust_id
	) AS custs_books
    ORDER BY book_amount DESC
    LIMIT 1
) AS best_customer
LEFT JOIN customers ON customers.cust_id = best_customer.cust_id;


/*Action12*/
SELECT amount, best_supp.supp_id, supp_name, phone 
FROM (
	SELECT *
	FROM (
		SELECT supp_id, SUM(amount) AS amount
		FROM book_orders
		WHERE order_date >= 'fromDate' AND  status_id != 5
		GROUP BY supp_id
	) AS supps_book_amounts
    ORDER BY amount DESC
    LIMIT 1
) AS best_supp
LEFT JOIN suppliers ON suppliers.supp_id = best_supp.supp_id;

/*Action13*/
SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount
FROM (
	SELECT *
	FROM book_orders
	WHERE (order_date BETWEEN 'fromDate' AND 'tilDate') AND status_id != 5
) AS orders_amount_by_book;

/*Action14*/
SELECT COUNT(order_id) orders_amount, SUM(amount) books_amount
FROM (
	SELECT *
	FROM book_orders
	WHERE (order_date BETWEEN 'fromDate' AND 'tilDate') AND status_id = 4
) AS orders_amount_by_book;


/*Action15*/
SELECT SUM(discount) AS total_disc
FROM (
	SELECT full_book_price -total_after_discount AS discount
	FROM (
		SELECT *
		FROM purchases
		WHERE ( purch_date BETWEEN 'fromDate' AND 'tilDate') AND cust_id = '1' AND canceled = false
	) AS cust_disc
) AS disc_sum;


/*Action16*/
SELECT *
FROM (
	SELECT *
	FROM (
		SELECT SUM(total_after_discount) AS total_Q1
		FROM purchases
		WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false
    ) AS Q1
    LEFT JOIN (
		SELECT *
        FROM (
			SELECT SUM(total_after_discount) AS total_Q2
			FROM purchases
			WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false
			) AS Q2
    ) Q1Q2 ON (total_Q1 OR total_Q1 IS NULL)
    LEFT JOIN (
		SELECT *
        FROM (
			SELECT SUM(total_after_discount) AS total_Q3
			FROM purchases
			WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false
			) AS Q3
    ) Q1Q2Q3 ON (total_Q1 OR total_Q1 IS NULL)
    LEFT JOIN (
		SELECT *
        FROM (
			SELECT SUM(total_after_discount) AS total_Q4
			FROM purchases
			WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false
			) AS Q4
    ) Q1Q2Q3Q4 ON (total_Q1 OR total_Q1 IS NULL)
) AS revenue;


/*Action17*/
SELECT COUNT(cust_id) AS new_custs 
FROM ( 
select * 
FROM customers 
) AS new_cust 
WHERE (join_date>= ' fromDate '); 


/*Action18*/
SELECT SUM(total_price) AS total_price
FROM (
	SELECT *
	FROM book_orders
	WHERE supp_id = 'suppID' AND (order_date BETWEEN 'fromDate' AND 'tilDate') AND status_id != 5
) AS supp_orders;


/*Action19*/
SELECT SUM(total_after_discount) AS total_for_seller
FROM (
	SELECT *
	FROM purchases
	WHERE seller_id = 'sellID' AND (purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false
) AS seller_purch;


/*Action20*/
SELECT *
FROM (
	SELECT books.book_id, book_name, author_name, amount
	FROM (
		SELECT book_id, COUNT(*) AS amount
		FROM purchases
		WHERE (purch_date BETWEEN 'fromDate' AND 'tilDate') AND canceled = false
		GROUP BY book_id
	) AS count_amount
    LEFT JOIN books ON books.book_id = count_amount.book_id
) AS top_10
ORDER BY amount DESC
LIMIT 10;