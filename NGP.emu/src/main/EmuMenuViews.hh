#pragma once
#include "OptionView.hh"

class SystemOptionView : public OptionView
{
	BoolMenuItem ngpLanguage
	{
		"NGP Language", "Japanese", "English",
		[this](BoolMenuItem &item, const Input::Event &e)
		{
			item.toggle(*this);
			language_english = item.on;
		}
	};

public:
	SystemOptionView(Base::Window &win): OptionView(win) {}

	void loadSystemItems(MenuItem *item[], uint &items)
	{
		OptionView::loadSystemItems(item, items);
		ngpLanguage.init(language_english); item[items++] = &ngpLanguage;
	}
};

#include "MenuView.hh"

class SystemMenuView : public MenuView
{
public:
	SystemMenuView(Base::Window &win): MenuView(win) {}
};
