#pragma once

#include <libwidget/Container.h>
#include <libwidget/TitleBar.h>
#include <libwidget/Window.h>

#include <libfilepicker/model/Navigation.h>
#include <libfilepicker/widgets/FileBrowser.h>
#include <libfilepicker/widgets/JumpList.h>
#include <libfilepicker/widgets/ToolBar.h>

class MainWindow :
    public Widget::Window
{
private:
public:
    MainWindow(RefPtr<FilePicker::Navigation> navigation, RefPtr<FilePicker::Bookmarks> bookmarks)
        : Widget::Window(WINDOW_RESIZABLE)
    {
        size(Math::Vec2i(700, 500));

        root()->layout(VFLOW(0));

        root()->add<Widget::TitleBar>(
            Graphic::Icon::get("folder"),
            "File Manager");

        root()->add<FilePicker::ToolBar>(navigation, bookmarks);

        auto bookmarks_and_browser = root()->add<Widget::Container>();

        bookmarks_and_browser->flags(Widget::Element::FILL);
        bookmarks_and_browser->layout(HFLOW(1));

        auto jump_list = bookmarks_and_browser->add<FilePicker::JumpList>(navigation, bookmarks);

        jump_list->min_width(192);

        auto browser = bookmarks_and_browser->add<FilePicker::FileBrowser>(navigation);

        browser->on_element_selected = [&](String &path) {
            auto l = launchpad_create("open", "/System/Utilities/open");
            launchpad_argument(l, path.cstring());
            launchpad_launch(l, nullptr);
        };
    }
};
