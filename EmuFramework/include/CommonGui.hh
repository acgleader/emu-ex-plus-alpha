#pragma once

/*  This file is part of EmuFramework.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with EmuFramework.  If not, see <http://www.gnu.org/licenses/> */

#include "EmuApp.hh"
#include <FileUtils.hh>
#include <main/EmuMenuViews.hh>

static SystemMenuView mMenu(mainWin);

void initMainMenu(Base::Window &win)
{
	mMenu.win = &win;
	mMenu.init(Input::keyInputIsPresent());
}

View &mainMenu()
{
	return mMenu;
}

View &allocAndGetOptionCategoryMenu(Base::Window &win, const Input::Event &e, StackAllocator &allocator, uint idx)
{
	auto oCategoryMenu = allocator.allocNew<SystemOptionView>(win);
	oCategoryMenu->init(idx, !e.isPointer());
	return *oCategoryMenu;
}
