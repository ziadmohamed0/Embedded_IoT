/**
 * @file Magazine.hpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#ifndef LIBRARY_MAGAZINE_HPP_
#define LIBRARY_MAGAZINE_HPP_

#include "../Items/item.hpp"

namespace Magazine {
    class Magazine : public items::LibraryItem {
        private:
            std::string title;
            int issueNumber;
            bool available;
        public:
            /**
             * @brief Construct a new Magazine object
             * 
             * @param copyTitle 
             * @param copyIssueNumber 
             */
            Magazine(const std::string& copyTitle, int copyIssueNumber) : 
                        title(copyTitle),
                        issueNumber(copyIssueNumber),
                        available(true){}
            void displayInfo() const override;
            bool isAvailable() const override;
            void borrowItem() override;
            void returnItem() override;
    };
}

#endif