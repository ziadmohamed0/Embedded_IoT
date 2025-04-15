/**
 * @file books.hpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#ifndef _LIBRARY_BOOKS_BOOKS_HPP_
#define _LIBRARY_BOOKS_BOOKS_HPP_

#include "../Items/item.hpp"

namespace Books {
    class Books : public items::LibraryItem{
        private:
            std::string title, author;
            bool available{false};
        public:
        Books(std::string copyTitle, std::string copyAuthor) : 
                title(copyTitle), 
                author(copyAuthor) {}
        void displayInfo(void) const override;
        bool isAvailable(void) const override;
        void borrowItem(void) override;
        void returnItem(void) override;
    };
}

#endif