
#pragma once

#include <memory>
#include <string>
#include <QMetaType>

namespace jp
{

    struct Author
    {
        int id;
        std::string name;

        friend bool operator==(const std::shared_ptr<jp::Author> lhs, const int rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Author> lhs, const std::string& rhs)
        {
            return lhs->name == rhs;
        }
    };


    struct Genre
    {
        int id;
        std::string name;

        friend bool operator==(const std::shared_ptr<jp::Genre> lhs, const int rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Genre> lhs, const std::string& rhs)
        {
            return lhs->name == rhs;
        }
    };


    struct Publisher
    {
        int id;
        std::string name;

        friend bool operator==(const std::shared_ptr<jp::Publisher> lhs, const int rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Publisher> lhs, const std::string& rhs)
        {
            return lhs->name == rhs;
        }
    };


    struct Book
    {
        int id;
        std::string title;
        std::shared_ptr<jp::Author> author;
        std::shared_ptr<jp::Genre> genre;
        std::shared_ptr<jp::Publisher> publisher;
        std::string year;
        float price;
        int count;

        friend bool operator==(const std::shared_ptr<jp::Book> lhs, const int& rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Book> lhs, const std::string& rhs)
        {
            return lhs->title == rhs;
        }
    };


    struct Client
    {
        int id;
        std::string name;
        std::string phoneNumber;

        friend bool operator==(const std::shared_ptr<jp::Client> lhs, const int& rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Client> lhs, const std::string& rhs)
        {
            return lhs->phoneNumber == rhs;
        }
    };


    struct Status
    {
        int id;
        std::string name;

        friend bool operator==(const std::shared_ptr<jp::Status> lhs, const int& rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Status> lhs, const std::string& rhs)
        {
            return lhs->name == rhs;
        }
    };


    struct Employee
    {
        int id;
        std::string firstName;
        std::string surname;
        std::string patronymic;
        std::string login;
        std::string password;
        bool role;
        bool isActive;

        friend bool operator==(const std::shared_ptr<jp::Employee> lhs, const int& rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Employee> lhs, const std::string& rhs)
        {
            return lhs->login == rhs;
        }
    };


    struct Order
    {
        int id;
        std::list<Book> books;
        int employee;
        std::shared_ptr<jp::Client> client;
        std::shared_ptr<jp::Status> status;
        std::string address;
        std::string date;
        float sum;

        friend bool operator==(const std::shared_ptr<jp::Order> lhs, const int& rhs)
        {
            return lhs->id == rhs;
        }

        friend bool operator==(const std::shared_ptr<jp::Order> lhs, const std::string& rhs)
        {
            return lhs->client->name == rhs;
        }
    };
}

Q_DECLARE_METATYPE(std::shared_ptr<jp::Book>);
Q_DECLARE_METATYPE(std::shared_ptr<jp::Order>);
Q_DECLARE_METATYPE(std::shared_ptr<jp::Employee>);
Q_DECLARE_METATYPE(std::shared_ptr<jp::Publisher>);
Q_DECLARE_METATYPE(std::shared_ptr<jp::Author>);
Q_DECLARE_METATYPE(std::shared_ptr<jp::Genre>);
Q_DECLARE_METATYPE(jp::Book);
Q_DECLARE_METATYPE(jp::Publisher);
Q_DECLARE_METATYPE(jp::Author);
Q_DECLARE_METATYPE(jp::Genre);
