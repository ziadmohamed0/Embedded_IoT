/**
 * @file item.hpp
 * @author Ziad Mohammed Fathy
 * @brief 
 * @version 0.1
 * @date 2025-04-15
 */

#ifndef _LIBRARY_ITEMS_ITEMS_HPP_
#define _LIBRARY_ITEMS_ITEMS_HPP_

#include <iostream>

namespace items{

    class LibraryItem{
        public:
            virtual void displayInfo(void) const = 0;
            virtual bool isAvailable(void) const = 0;
            virtual void borrowItem(void) = 0;
            virtual void returnItem(void) = 0;
            virtual ~LibraryItem() = default;
    };
}

#endif