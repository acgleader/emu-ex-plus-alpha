#pragma once

/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#include <engine-globals.h>
#include <util/operators.hh>
#include <base/iphone/private.hh>

#ifdef __OBJC__
#import <base/iphone/EAGLView.h>
#endif

namespace Base
{

#ifdef CONFIG_BASE_IOS_RETINA_SCALE
extern uint screenPointScale;
#else
static constexpr uint screenPointScale = 1;
#endif

class IOSWindow : public NotEquals<IOSWindow>
{
public:
	void *uiWin_ = nullptr; // UIWindow in ObjC
	void *glView_ = nullptr; // EAGLView in ObjC
	IG::WindowRect contentRect; // active window content
	#ifdef CONFIG_BASE_IOS_RETINA_SCALE
	uint pointScale = 1;
	#else
	static constexpr uint pointScale = 1;
	#endif

	constexpr IOSWindow() {}
	#ifdef __OBJC__
	void updateContentRect(int width, int height, uint rotateView, UIApplication *sharedApp);
	UIWindow *uiWin() { return (__bridge UIWindow*)uiWin_; }
	EAGLView *glView() { return (__bridge EAGLView*)glView_; }
	#endif

	bool operator ==(IOSWindow const &rhs) const
	{
		return uiWin_ == rhs.uiWin_;
	}

	operator bool() const
	{
		return uiWin_;
	}
};

using WindowImpl = IOSWindow;

}
