#pragma once

#include <libgraphic/Icon.h>
#include <libwidget/Application.h>
#include <libwidget/Button.h>

namespace Widget
{

enum DialogButton
{
    YES = (1 << 0),
    NO = (1 << 1),
    OK = (1 << 2),
    CANCEL = (1 << 3)
};

enum class DialogResult
{
    NONE,
    YES,
    NO,
    OK,
    CANCEL,
    CLOSE,
};

class Dialog
{
private:
    DialogResult _result = DialogResult::NONE;

    int _buttons = 0;
    String _title = "";
    Window *_window = nullptr;

public:
    String title() { return _title; }

    void title(String title) { _title = title; }

    void buttons(int buttons) { _buttons = buttons; }

    Dialog()
    {
    }

    virtual ~Dialog()
    {
    }

    DialogResult show()
    {
        Assert::is_true(_window == nullptr);

        _window = new Window(WINDOW_NONE);

        _window->type(WINDOW_TYPE_POPOVER);

        _window->on(Event::WINDOW_CLOSING, [&](auto) {
            close(DialogResult::CLOSE);
            _window->hide();
        });

        render(_window);

        _window->show();

        Application::run_nested();

        delete _window;
        _window = nullptr;

        return _result;
    }

    void close(DialogResult result)
    {
        Application::exit_nested(0);
        _result = result;
    }

    virtual void render(Window *) {}

    virtual void on_button(DialogButton) {}

    void create_buttons(Component *parent)
    {
        if (_buttons & DialogButton::YES)
        {
            auto button = new Button(parent, Button::OUTLINE, "Yes");
            button->on(Event::ACTION, [&](auto) {
                on_button(DialogButton::YES);
                close(DialogResult::YES);
                _window->hide();
            });
        }

        if (_buttons & DialogButton::NO)
        {
            auto button = new Button(parent, Button::OUTLINE, "No");
            button->on(Event::ACTION, [&](auto) {
                on_button(DialogButton::NO);
                close(DialogResult::NO);
                _window->hide();
            });
        }

        if (_buttons & DialogButton::OK)
        {
            auto button = new Button(parent, Button::OUTLINE, "Ok");
            button->on(Event::ACTION, [&](auto) {
                on_button(DialogButton::OK);
                close(DialogResult::OK);
                _window->hide();
            });
        }

        if (_buttons & DialogButton::CANCEL)
        {
            auto button = new Button(parent, Button::OUTLINE, "Cancel");
            button->on(Event::ACTION, [&](auto) {
                on_button(DialogButton::CANCEL);
                close(DialogResult::CANCEL);
                _window->hide();
            });
        }
    }
};

} // namespace Widget
