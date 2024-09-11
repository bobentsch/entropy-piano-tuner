#pragma once

#include <algorithm>
#include <QLayout>


namespace common::core::qtlegacy {

    int getMaxMarginOfLayout(const QLayout& layout) {
        int margins[4];
        layout.getContentsMargins(margins, margins+1, margins+2, margins+3);
        return std::ranges::max(margins);
    }

} // namespace common::core::qtlegacy