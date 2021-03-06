/* See LICENSE file for copyright and license details. */
#include "movestack.c"

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Dank Mono:size=11:antialias=true:autoHint=true", "JoyPixels:size=11:antialias=true", "monospace:size=11" };
static const char dmenufont[]       = "Dank Mono:size=11:antialias=true";
static const char dmenuprompt[]     = ">";
static const char col_black[]       = "#000000";
static const char col_gray1[]       = "#111111";
static const char col_gray2[]       = "#333333";
static const char col_dim[]         = "#888888";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_bright[]      = "#f8f8f8";
static const char col_cyan[]        = "#005555";
static const char col_bright_cyan[] = "#00BBBB";
static const char col_red[]         = "#ff4444";
static const char *colors[][3]      = {
	/*                 fg          bg               border   */
	[SchemeNorm]   = { col_gray3,  col_black,       col_gray2 },
	[SchemeSel]    = { col_bright, col_black,       col_bright_cyan  },
	[SchemeTagSel] = { col_bright, col_gray1,       col_gray2 },
	[SchemeDim]    = { col_dim,    col_black,       col_gray2 },
	[SchemeUrg]    = { col_black,  col_red,         col_gray2 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title       tags mask     isfloating   monitor */
	{ "Gimp",               NULL,       NULL,       0,            0,           -1 },
	{ "Firefox",            NULL,       NULL,       0,            0,           -1 },
	{ "code",               NULL,       NULL,       0,            0,           -1 },
	{ "mpv",                NULL,       NULL,       0,            1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-p", dmenuprompt, "-fn", dmenufont, "-nb", col_black, "-nf", col_dim, "-sb", col_cyan, "-sf", col_bright, NULL };
static const char *termcmd[]  = { "urxvt", NULL };
static const char *browser[] = { "firefox", NULL };
static const char *shutdown[] = { "shutdown", "now" };

static Key keys[] = {
  	/* modifier                     key           function        argument */
	/*** Launchers ***/
	/* dmenu */
  	{ MODKEY,                       XK_p,         spawn,          {.v = dmenucmd } },
	/* terminal */
	{ MODKEY,                       XK_semicolon, spawn,          {.v = termcmd } },
	/* browser */
	{ MODKEY,                       XK_i,         spawn,          {.v = browser } },
	/* shutdown */
	{ MODKEY|ShiftMask,             XK_s,         spawn,          {.v = shutdown } },
	/* quit */
	{ MODKEY|ShiftMask,             XK_q,         quit,           {0} },
	/*** Client ***/
	/* close */
	{ MODKEY,                       XK_c,         killclient,     {0} },
	/* focus next */
	{ MODKEY,                       XK_j,         focusstack,     {.i = +1 } },
	/* focus previous */
	{ MODKEY,                       XK_k,         focusstack,     {.i = -1 } },
	/* swap with next */
	{ MODKEY|ShiftMask,             XK_j,         movestack,      {.i = +1 } },
	/* swap with previous */
	{ MODKEY|ShiftMask,             XK_k,         movestack,      {.i = -1 } },
	/* swap with master */
	{ MODKEY,                       XK_Return,    zoom,           {0} },
	/* send to next tag */
	{ MODKEY,                       XK_n,         inctag,         {.i = +1 } },
	/* send to previous tag */
	{ MODKEY|ShiftMask,             XK_n,         inctag,         {.i = -1 } },
	/* toggle floating */
	{ MODKEY|ShiftMask,             XK_space,     togglefloating, {0} },
	/* Show focused client in all tags */
	{ MODKEY|ShiftMask,             XK_0,         tag,            {.ui = ~0 } },
	/*** Layout ***/
	/* toggle bar */
	{ MODKEY,                       XK_b,         togglebar,      {0} },
	/* inc master window count */
	{ MODKEY|ShiftMask,             XK_h,         incnmaster,     {.i = +1 } },
	/* dec master window count */
	{ MODKEY|ShiftMask,             XK_l,         incnmaster,     {.i = -1 } },
	/* inc master scale */
	{ MODKEY,                       XK_l,         setmfact,       {.f = +0.025} },
	/* dec master scale */
	{ MODKEY,                       XK_h,         setmfact,       {.f = -0.025} },
	/* set tile */
	{ MODKEY,                       XK_t,         setlayout,      {.v = &layouts[0]} },
	/* set floating */
	{ MODKEY,                       XK_f,         setlayout,      {.v = &layouts[1]} },
	/* set monocle */
	{ MODKEY,                       XK_m,         setlayout,      {.v = &layouts[2]} },
	/* switch to previous layout */
	{ MODKEY,                       XK_space,     setlayout,      {0} },
	/* show all clients */
	{ MODKEY,                       XK_0,         view,           {.ui = ~0 } },
	/*** Navigation ***/
	TAGKEYS(                        XK_1,                         0)
	TAGKEYS(                        XK_2,                         1)
	TAGKEYS(                        XK_3,                         2)
	TAGKEYS(                        XK_4,                         3)
	TAGKEYS(                        XK_5,                         4)
	TAGKEYS(                        XK_6,                         5)
	TAGKEYS(                        XK_7,                         6)
	TAGKEYS(                        XK_8,                         7)
	TAGKEYS(                        XK_9,                         8)
	/* switch to previous tag */
	{ MODKEY,                       XK_Tab,       view,           {0} },
        /*** Multi Monitor ***/
        /* focus previous monitor */
	{ MODKEY,                       XK_comma,     focusmon,       {.i = -1 } },
        /* focus next monitor */
	{ MODKEY,                       XK_period,    focusmon,       {.i = +1 } },
        /* send focused window to previous */
	{ MODKEY|ShiftMask,             XK_comma,     tagmon,         {.i = -1 } },
        /* send focused window to next */
	{ MODKEY|ShiftMask,             XK_period,    tagmon,         {.i = +1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

