#ifndef INVOICE_H
#define INVOICE_H

#include <string>
#include "Date.h"

class Invoice {
private:
    std::string invoiceID;
    std::string orderID;
    double totalAmount;
    Date createdDate;
    bool isPaid;
public:
    Invoice(std::string invId, std::string ordId, double amount, Date date, bool paid = false);
    // getters
    std::string getInvoiceID() const;
    std::string getOrderID() const;
    double getTotalAmount() const;
    Date getCreatedDate() const;
    bool getIsPaid() const;
    // setters
    void setPaid(bool paid);
};

#endif