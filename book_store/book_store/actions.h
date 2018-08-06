#ifndef ACTIONS_H
#define ACTIONS_H

#include "db.h"
#include <string>

//check the date format
bool validateDateFormat(string date);

//1. Show all available books in inventory
int showBooks();

//2. Show all open orders
int showOrders();

//3. List of all customers who made a purchase
int showCustomers();

//4. Show all suppliers
int showSuppliers();

//5. Show all purchases between given dates: fromDate & tilDate
int showPurchases(string fromDate, string tilDate);

//6. Show all books available for global sale
int showBooksOnSale();

//7. Check if a given book: bookTitle + bookAuthor is available in the inventory
int searchBook(string bookName, string bookAuthor);

//8. List of all suppliers of a given book: bookTitle + bookAuthor
int showSuppliersOfBook(string bookName, string bookAuthor);

//9. How many books from type: bookTitle + bookAuthor where sold from date fromDate
int booksSold(string bookName, string bookAuthor, string fromDate);

//10. Show How Many Books A Client Has Bought Since A Given Date
int booksPurchased(unsigned int custID, string fromDate);

//11. Show the customer details who bought the most since given date fromDate
int mostPurchasesCustomer(string fromDate);

//12. show the supplier details who sold us the most books since given date fromDate
int mostOrdersSuplier(string fromDate);

//13. amount of orders made (and how many books?) between given dates: fromDate & tilDate
int ordersMade(string fromDate, string tilDate);

//14. Amount of orders made between given dates that were made by customers and were sold
int ordersMadePurchases(string fromDate, string tilDate);

//15. show the total discount a customer received since a certain date
int totalDiscountCustomer(unsigned int custID, string fromDate);

//16. sum revenue in Q1, Q2, Q3, Q4 in a given year: y
int sumRevenue(string y);

//17. Show how many customers were added from given date: fromDate
int customersAdded(string fromDate);

//18. total amount paid to a given supplier: suppID, between given dates: fromDate, tilDate
int totalPaidSupplier(unsigned int suppID, string fromDate, string tilDate);

//19. total amount a given seller: sellID earned between given dates: fromDate, tilDate
int totalSoldSeller(unsigned int sellID, string fromDate, string tilDate);

//20. top 10 most sold books between given dates: fromDate, tilDate
int top10Books(string fromDate, string tilDate);

#endif
