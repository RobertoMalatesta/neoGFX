// menu_bar.cpp
/*
neogfx C++ GUI Library
Copyright(C) 2016 Leigh Johnston

This program is free software: you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "neogfx.hpp"
#include "app.hpp"
#include "menu_bar.hpp"
#include "flow_layout.hpp"
#include "text_widget.hpp"

namespace neogfx
{
	menu_bar::menu_bar() : menu(MenuBar)
	{
		init();
	}

	menu_bar::menu_bar(i_widget& aParent) : widget(aParent), menu(MenuBar)
	{
		init();
	}

	menu_bar::menu_bar(i_layout& aLayout) : widget(aLayout), menu(MenuBar)
	{
		init();
	}

	menu_bar::~menu_bar()
	{
		item_added.unsubscribe(this);
		item_removed.unsubscribe(this);
	}

	size_policy menu_bar::size_policy() const
	{
		if (widget::has_size_policy())
			return widget::size_policy();
		return neogfx::size_policy::Minimum;
	}

	bool menu_bar::visible() const
	{
		if (app::instance().basic_services().has_shared_menu_bar())
			return false;
		return widget::visible();
	}

	void menu_bar::init()
	{
		set_margins(neogfx::margins{});
		set_layout(std::make_shared<flow_layout>(*this));
		item_added([this](item_index aItemIndex)
		{
			switch (item(aItemIndex).type())
			{
			case i_menu_item::Action:
				layout().add_item(aItemIndex, std::make_shared<text_widget>(item(aItemIndex).action().menu_text()));
				break;
			case i_menu_item::SubMenu:
				layout().add_item(aItemIndex, std::make_shared<text_widget>(item(aItemIndex).sub_menu().title()));
				break;
			}
		}, this);
		item_removed([this](item_index aItemIndex)
		{
			auto& item = layout().get_item(aItemIndex);
			if (layout().is_widget(aItemIndex))
			{
				i_widget& w = layout().get_widget(aItemIndex);
				w.parent().remove_widget(w);
			}
			layout().remove_item(aItemIndex);
		}, this);
	}
}