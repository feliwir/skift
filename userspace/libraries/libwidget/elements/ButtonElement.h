#pragma once

#include <libgraphic/Icon.h>
#include <libwidget/Element.h>

namespace Widget
{

class ButtonElement : public Element
{
public:
    enum Style
    {
        TEXT,
        OUTLINE,
        FILLED,
    };

private:
    bool _mouse_over = false;
    bool _mouse_press = false;
    Style _style = TEXT;

public:
    ButtonElement(Style style);

    ButtonElement(Style style, RefPtr<Graphic::Icon> icon);

    ButtonElement(Style style, String text);

    ButtonElement(Style style, RefPtr<Graphic::Icon> icon, String text);

    ButtonElement(Style style, RefPtr<Graphic::Bitmap> image, String text);

    void paint(Graphic::Painter &painter, const Math::Recti &rectangle) override;

    void event(Event *event) override;
};

RefPtr<ButtonElement> button(ButtonElement::Style style, RefPtr<Element> child, Callback<void(void)> on_click);

/* --- Basic Button --------------------------------------------------------- */

RefPtr<ButtonElement> basic_button(RefPtr<Element> child, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> basic_button(RefPtr<Graphic::Icon> icon, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> basic_button(String text, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> basic_button(RefPtr<Graphic::Icon> icon, String text, Callback<void(void)> on_click = nullptr);

/* --- Outlined Button ------------------------------------------------------ */

RefPtr<ButtonElement> outlined_button(RefPtr<Element> child, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> outlined_button(RefPtr<Graphic::Icon> icon, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> outlined_button(String text, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> outlined_button(RefPtr<Graphic::Icon> icon, String text, Callback<void(void)> on_click = nullptr);

/* --- Filled Button -------------------------------------------------------- */

RefPtr<ButtonElement> filled_button(RefPtr<Element> child, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> filled_button(RefPtr<Graphic::Icon> icon, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> filled_button(String text, Callback<void(void)> on_click = nullptr);

RefPtr<ButtonElement> filled_button(RefPtr<Graphic::Icon> icon, String text, Callback<void(void)> on_click = nullptr);

} // namespace Widget
