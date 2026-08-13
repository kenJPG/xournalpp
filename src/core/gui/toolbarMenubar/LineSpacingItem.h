/*
 * Xournal++
 *
 * Part of the customizable toolbars
 *
 * A spin button to adjust the line spacing of note text (Pango line spacing factor).
 *
 * @author Xournal++ Team
 * https://github.com/xournalpp/xournalpp
 *
 * @license GNU GPLv2 or later
 */

#pragma once

#include <string>  // for string

#include <gtk/gtk.h>  // for GtkWidget

#include "control/actions/ActionRef.h"

#include "AbstractToolItem.h"

class ActionDatabase;

class LineSpacingItem: public AbstractToolItem {
public:
    LineSpacingItem(std::string id, ActionDatabase& db);
    ~LineSpacingItem() override = default;

public:
    GtkWidget* getNewToolIcon() const override;
    std::string getToolDisplayName() const override;

    xoj::util::WidgetSPtr createItem(bool horizontal) override;

private:
    ActionRef gAction;  ///< Points to the GAction corresponding to Action::LINE_SPACING
};
