#include <base/Base.hh>
#include <base/EventLoopFileSource.hh>
#include <base/common/windowPrivate.hh>
#include <glib-unix.h>

namespace Base
{

#ifdef CONFIG_BASE_X11
extern void x11FDHandler();
extern bool x11FDPending();
#endif

void EventLoopFileSource::init(int fd, PollEventDelegate callback, uint events)
{
	logMsg("adding fd %d to glib context", fd);
	static GSourceFuncs fdSourceFuncs
	{
		nullptr,
		nullptr,
		[](GSource *source, GSourceFunc, gpointer user_data)
		{
			auto &e = *((EventLoopFileSource*)user_data);
			//logMsg("events for fd: %d", e.fd());
			return (gboolean)e.callback(e.fd_, g_source_query_unix_fd(source, e.tag));
		},
		nullptr
	};
	fd_ = fd;
	var_selfs(callback);
	source = g_source_new(&fdSourceFuncs, sizeof(GSource));
	tag = g_source_add_unix_fd(source, fd, (GIOCondition)events);
	g_source_set_callback(source, nullptr, this, nullptr);
	g_source_attach(source, nullptr);
	g_source_unref(source);
}

#ifdef CONFIG_BASE_X11
void EventLoopFileSource::initX(int fd)
{
	logMsg("adding X fd %d to glib context", fd);
	static GSourceFuncs fdSourceFuncs
	{
		[](GSource *, gint *timeout)
		{
			*timeout = -1;
			return (gboolean)x11FDPending();
		},
		[](GSource *)
		{
			return (gboolean)x11FDPending();
		},
		[](GSource *, GSourceFunc, gpointer)
		{
			//logMsg("events for X fd");
			x11FDHandler();
			return (gboolean)TRUE;
		},
		nullptr
	};
	fd_ = fd;
	source = g_source_new(&fdSourceFuncs, sizeof(GSource));
	tag = g_source_add_unix_fd(source, fd, G_IO_IN);
	g_source_attach(source, nullptr);
	g_source_unref(source);
}
#endif

void EventLoopFileSource::setEvents(uint events)
{
	if(!fd_)
	{
		bug_exit("tried to set events on uninitialized source");
	}
	g_source_modify_unix_fd(source, tag, (GIOCondition)events);
}

int EventLoopFileSource::fd() const
{
	return fd_;
}

void EventLoopFileSource::deinit()
{
	logMsg("removing fd %d from glib context", fd_);
	if(!fd_)
	{
		bug_exit("tried to destroy uninitialized source");
	}
	g_source_destroy(source);
	fd_ = -1;
}

void initMainEventLoop() {}

void runMainEventLoop()
{
	static GSourceFuncs onFrameSourceFuncs
	{
		[](GSource *, gint *timeout)
		{
			bool ready = Base::mainScreen().frameIsPosted();
			*timeout = ready ? 0 : -1;
			return (gboolean)ready;
		},
		[](GSource *)
		{
			return (gboolean)Base::mainScreen().frameIsPosted();
		},
		[](GSource *, GSourceFunc, gpointer)
		{
			//logMsg("events for frame handler");
			frameUpdate(0);
			return (gboolean)TRUE;
		},
		nullptr
	};
	auto onFrameSource = g_source_new(&onFrameSourceFuncs, sizeof(GSource));
	g_source_set_priority(onFrameSource, G_PRIORITY_HIGH_IDLE);
	g_source_attach(onFrameSource, nullptr);
	g_source_unref(onFrameSource);

	logMsg("entering glib event loop");
	auto mainLoop = g_main_loop_new(nullptr, false);
	g_main_loop_run(mainLoop);
}

}
