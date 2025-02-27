#include <libgraphic/Painter.h>
#include <libwidget/Elements.h>
#include <libwidget/Layouts.h>

namespace Widget
{

void ButtonElement::paint(Graphic::Painter &painter, const Math::Recti &rectangle)
{
    UNUSED(rectangle);

    if (enabled())
    {
        if (_style == OUTLINE)
        {
            painter.draw_rectangle_rounded(bound(), 4, 1, color(THEME_BORDER));
        }
        else if (_style == FILLED)
        {
            painter.fill_rectangle_rounded(bound(), 4, color(THEME_ACCENT));
        }

        if (_mouse_over)
        {
            painter.fill_rectangle_rounded(bound(), 4, color(THEME_FOREGROUND).with_alpha(0.1));
        }

        if (_mouse_press)
        {
            painter.fill_rectangle_rounded(bound(), 4, color(THEME_FOREGROUND).with_alpha(0.1));
        }
    }
}

void ButtonElement::event(Event *event)
{
    if (event->type == Event::MOUSE_ENTER)
    {
        _mouse_over = true;

        should_repaint();
        event->accepted = true;
    }
    else if (event->type == Event::MOUSE_LEAVE)
    {
        _mouse_over = false;

        should_repaint();
        event->accepted = true;
    }
    else if (event->type == Event::MOUSE_BUTTON_PRESS)
    {
        _mouse_press = true;

        should_repaint();
        event->accepted = true;
    }
    else if (event->type == Event::MOUSE_BUTTON_RELEASE)
    {
        _mouse_press = false;

        Event action_event = {};
        action_event.type = Event::ACTION;
        dispatch_event(&action_event);

        should_repaint();
        event->accepted = true;
    }
    else if (event->type == Event::WIDGET_DISABLE)
    {
        _mouse_over = false;
        _mouse_press = false;
    }
}

ButtonElement::ButtonElement(Style style) : _style{style}
{
    layout(HFLOW(0));
    //insets(Insetsi(0, 16));
    min_height(36);
    flags(Element::GREEDY);
}

ButtonElement::ButtonElement(Style style, RefPtr<Graphic::Icon> icon) : ButtonElement{style}
{
    layout(STACK());

    min_width(64);
    min_height(36);
    flags(Element::GREEDY | Element::SQUARE);

    auto icon_panel = add(Widget::icon(icon));

    if (style == FILLED)
    {
        icon_panel->color(THEME_FOREGROUND, Graphic::Colors::WHITE);
    }
}

ButtonElement::ButtonElement(Style style, String text) : ButtonElement{style}
{
    layout(STACK());
    insets(Insetsi(0, 16));
    min_width(64);

    auto lbl = add(label(text, Anchor::CENTER));
    if (style == FILLED)
    {
        lbl->color(THEME_FOREGROUND, Graphic::Colors::WHITE);
    }
}

ButtonElement::ButtonElement(Style style, RefPtr<Graphic::Icon> icon, String text) : ButtonElement{style}
{
    insets(Insetsi(0, 0, 12, 16));
    min_width(64);

    auto icon_panel = add(Widget::icon(icon));
    icon_panel->insets(Insetsi(0, 0, 0, 8));

    auto lbl = add(label(text));

    if (style == FILLED)
    {
        lbl->color(THEME_FOREGROUND, Graphic::Colors::WHITE);
        icon_panel->color(THEME_FOREGROUND, Graphic::Colors::WHITE);
    }
}

ButtonElement::ButtonElement(Style style, RefPtr<Graphic::Bitmap> image, String text) : ButtonElement{style}
{
    insets(Insetsi(4, 4, 12, 16));
    min_width(64);

    auto image_panel = add(Widget::image(image));
    image_panel->outsets(Insetsi(0, 0, 0, 8));
    image_panel->min_width(36);
    image_panel->min_height(36);

    auto lbl = add(label(text));

    if (style == FILLED)
    {
        lbl->color(THEME_FOREGROUND, Graphic::Colors::WHITE);
    }
}

RefPtr<ButtonElement> button(ButtonElement::Style style, RefPtr<Element> child, Callback<void(void)> on_click)
{
    auto button = make<ButtonElement>(style);

    button->add(fill(child));
    button->min_width(36);
    button->min_height(36);

    if (on_click)
    {
        button->on(Event::ACTION, [on_click](auto) { on_click(); });
    }

    return button;
}

/* --- Basic Button --------------------------------------------------------- */

RefPtr<ButtonElement> basic_button(RefPtr<Element> child, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::TEXT,
        child,
        on_click);
}

RefPtr<ButtonElement> basic_button(RefPtr<Graphic::Icon> icon, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::TEXT,
        spacing({0, 16},
                Widget::icon(icon)),
        on_click);
}

RefPtr<ButtonElement> basic_button(String text, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::TEXT,
        spacing({0, 16},
                Widget::label(text, Anchor::CENTER)),
        on_click);
}

RefPtr<ButtonElement> basic_button(RefPtr<Graphic::Icon> icon, String text, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::TEXT,
        spacing({0, 0, 12, 16},
                Widget::hflow({
                    spacing({0, 0, 0, 8}, Widget::icon(icon)),
                    Widget::label(text, Anchor::CENTER),
                })),
        on_click);
}

/* --- Outlined Button ------------------------------------------------------ */

RefPtr<ButtonElement> outlined_button(RefPtr<Element> child, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::OUTLINE,
        child,
        on_click);
}

RefPtr<ButtonElement> outlined_button(RefPtr<Graphic::Icon> icon, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::OUTLINE,
        spacing({0, 16},
                Widget::icon(icon)),
        on_click);
}

RefPtr<ButtonElement> outlined_button(String text, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::OUTLINE,
        spacing({0, 16},
                Widget::label(text)),
        on_click);
}

RefPtr<ButtonElement> outlined_button(RefPtr<Graphic::Icon> icon, String text, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::OUTLINE,
        spacing({0, 0, 12, 16},
                Widget::hflow({
                    spacing({0, 0, 0, 8}, Widget::icon(icon)),
                    Widget::label(text, Anchor::CENTER),
                })),
        on_click);
}

/* --- Filled Button -------------------------------------------------------- */

RefPtr<ButtonElement> filled_button(RefPtr<Element> child, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::FILLED,
        child,
        on_click);
}

RefPtr<ButtonElement> filled_button(RefPtr<Graphic::Icon> icon, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::FILLED,
        spacing({0, 16},
                Widget::icon(icon)),
        on_click);
}

RefPtr<ButtonElement> filled_button(String text, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::FILLED,
        spacing({0, 16},
                Widget::label(text)),
        on_click);
}

RefPtr<ButtonElement> filled_button(RefPtr<Graphic::Icon> icon, String text, Callback<void(void)> on_click)
{
    return button(
        ButtonElement::FILLED,
        spacing({0, 0, 12, 16},
                Widget::hflow({
                    spacing({0, 0, 0, 8}, Widget::icon(icon)),
                    Widget::label(text, Anchor::CENTER),
                })),
        on_click);
}

} // namespace Widget
