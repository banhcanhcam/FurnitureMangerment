#include "Invoice.h"

Invoice::Invoice(std::string invId, std::string ordId, double amount, Date date, bool paid)
    : invoiceID(invId), orderID(ordId), totalAmount(amount), createdDate(date), isPaid(paid) {}

std::string Invoice::getInvoiceID() const { return invoiceID; }
std::string Invoice::getOrderID() const { return orderID; }
double Invoice::getTotalAmount() const { return totalAmount; }
Date Invoice::getCreatedDate() const { return createdDate; }
bool Invoice::getIsPaid() const { return isPaid; }
void Invoice::setPaid(bool paid) { isPaid = paid; }