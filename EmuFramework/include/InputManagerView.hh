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

#include <input/Input.hh>
#include <MenuView.hh>
#include <EmuInput.hh>

class IdentInputDeviceView : public View
{
	IG::WindowRect viewFrame;
	Gfx::Text text;

public:
	typedef DelegateFunc<void (const Input::Event &e)> OnIdentInputDelegate;
	OnIdentInputDelegate onIdentInput;

	constexpr IdentInputDeviceView(Base::Window &win): View(win) {}
	IG::WindowRect &viewRect() override { return viewFrame; }
	void init();
	void deinit() override;
	void place() override;
	void inputEvent(const Input::Event &e) override;
	void draw(Base::FrameTimeBase frameTime) override;
};

class InputManagerView : public BaseMenuView
{
private:
	char deviceConfigStr[MAX_SAVED_INPUT_DEVICES][MAX_INPUT_DEVICE_NAME_SIZE] {{0}};
	TextMenuItem deleteDeviceConfig;
	const char *profileStr[MAX_CUSTOM_KEY_CONFIGS] {nullptr};
	TextMenuItem deleteProfile;
	#ifdef CONFIG_INPUT_ANDROID_MOGA
	BoolMenuItem mogaInputSystem;
	#endif
	#ifdef INPUT_HAS_SYSTEM_DEVICE_HOTSWAP
	BoolMenuItem notifyDeviceChange;
	#endif
	#ifdef CONFIG_BASE_ANDROID
	TextMenuItem rescanOSDevices;
	#endif
	TextMenuItem identDevice;
	TextMenuItem inputDevName[Input::MAX_DEVS];
	MenuItem *item[sizeofArrayConst(inputDevName) + 6] = {nullptr};

public:
	char inputDevNameStr[Input::MAX_DEVS][80] {{0}};

	InputManagerView(Base::Window &win);
	void init(bool highlightFirst);
	void deinit() override;
	void onShow() override;
};

class InputManagerDeviceView : public BaseMenuView
{
private:
	MultiChoiceSelectMenuItem player;
	char profileStr[128] {0};
	TextMenuItem loadProfile;
	TextMenuItem renameProfile;
	TextMenuItem newProfile;
	TextMenuItem deleteProfile;
	#if defined CONFIG_INPUT_ICADE
	BoolMenuItem iCadeMode;
	#endif
	BoolMenuItem joystickAxis1DPad;
	BoolMenuItem joystickAxis2DPad;
	BoolMenuItem joystickAxisHatDPad;
	//TextMenuItem disconnect {"Disconnect"}; // TODO
	TextMenuItem inputCategory[EmuControls::categories];
	MenuItem *item[EmuControls::categories + 11] = {nullptr};
	InputDeviceConfig *devConf = nullptr;

	void confirmICadeMode(const Input::Event &e);

public:
	InputManagerDeviceView(Base::Window &win);

	void init(bool highlightFirst, InputDeviceConfig &devConf);
	void onShow() override;
};
