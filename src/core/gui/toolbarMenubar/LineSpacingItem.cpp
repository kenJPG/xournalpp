#include "LineSpacingItem.h"

#include <utility>  // for move

#include <glib-object.h>  // for g_object_ref, g_signal_connect_object

#include "control/actions/ActionDatabase.h"  // for ActionDatabase
#include "util/GVariantTemplate.h"           // for makeGVariant, getGVariantValue
#include "util/gtk4_helper.h"                // for gtk_box_append
#include "util/i18n.h"                       // for _
#include "util/raii/GVariantSPtr.h"

static constexpr double MIN_LINE_SPACING = 0.5;
static constexpr double MAX_LINE_SPACING = 2.0;
static constexpr double LINE_SPACING_STEP = 0.1;

LineSpacingItem::LineSpacingItem(std::string id, ActionDatabase& db):
        AbstractToolItem(std::move(id), Category::TOOLS), gAction(db.getAction(Action::LINE_SPACING)) {}

static void lineSpacingValueChangedCb(GtkSpinButton* spin, gpointer gAction) {
    double value = gtk_spin_button_get_value(spin);
    g_action_change_state(G_ACTION(gAction), makeGVariant(value));
}

auto LineSpacingItem::createItem(bool horizontal) -> xoj::util::WidgetSPtr {
    GtkWidget* spin = gtk_spin_button_new_with_range(MIN_LINE_SPACING, MAX_LINE_SPACING, LINE_SPACING_STEP);
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(spin), 2);
    gtk_widget_set_tooltip_text(spin, getToolDisplayName().c_str());
    if (horizontal) {
        gtk_widget_set_size_request(spin, 70, -1);
    }

    // Initialize the spin button value from the current action state.
    xoj::util::GVariantSPtr state(g_action_get_state(G_ACTION(gAction.get())), xoj::util::adopt);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), getGVariantValue<double>(state.get()));

    // Spin button -> action (triggers the change callback).
    g_signal_connect_object(spin, "value-changed", G_CALLBACK(lineSpacingValueChangedCb), gAction.get(),
                            GConnectFlags(0));

    // Action -> spin button (block the handler to avoid a feedback loop).
    g_signal_connect_object(
            gAction.get(), "notify::state", G_CALLBACK(+[](GObject* action, GParamSpec*, gpointer spin) {
                xoj::util::GVariantSPtr state(g_action_get_state(G_ACTION(action)), xoj::util::adopt);
                g_signal_handlers_block_by_func(spin, (gpointer)lineSpacingValueChangedCb, action);
                gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin), getGVariantValue<double>(state.get()));
                g_signal_handlers_unblock_by_func(spin, (gpointer)lineSpacingValueChangedCb, action);
            }),
            spin, GConnectFlags(0));

    GtkToolItem* it = gtk_tool_item_new();
    gtk_container_add(GTK_CONTAINER(it), spin);

    // Makes a proxy item for the toolbar's overflow menu.
    auto createProxy = [this]() {
        GtkWidget* proxy = gtk_menu_item_new();
        auto* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
        gtk_container_add(GTK_CONTAINER(proxy), box);
        gtk_box_append(GTK_BOX(box), getNewToolIcon());
        gtk_box_append(GTK_BOX(box), gtk_label_new(getToolDisplayName().c_str()));
        return proxy;
    };
    gtk_tool_item_set_proxy_menu_item(it, "", createProxy());

    return xoj::util::WidgetSPtr(GTK_WIDGET(it), xoj::util::adopt);
}

auto LineSpacingItem::getToolDisplayName() const -> std::string { return _("Line spacing"); }

auto LineSpacingItem::getNewToolIcon() const -> GtkWidget* {
    return gtk_image_new_from_icon_name("format-justify-fill", GTK_ICON_SIZE_LARGE_TOOLBAR);
}
