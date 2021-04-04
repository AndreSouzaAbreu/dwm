/* Constants */
#define TERMINAL "termite"
#define TERMCLASS "Termite"
#define BROWSER "qutebrowser"

/* appearance */
static unsigned int borderpx  = 3;  /* border pixel of windows */
static unsigned int snap      = 32; /* snap pixel */
static unsigned int gappih    = 0;  /* horiz inner gap between windows */
static unsigned int gappiv    = 0;  /* vert inner gap between windows */
static unsigned int gappoh    = 0;  /* horiz outer gap between windows and screen edge */
static unsigned int gappov    = 0;  /* vert outer gap between windows and screen edge */
static int swallowfloating    = 0;  /* 1 means swallow floating windows by default */
static int smartgaps          = 1;  /* 1 means no outer gap when there is only one window */
static int showbar            = 1;  /* 0 means no bar */
static int topbar             = 1;  /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=14" };
static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#770000";
static char selbgcolor[]      = "#005577";
static char *colors[][3] = {
  /*               fg           bg           border   */
  [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
  [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

typedef struct {
  const char *name;
  const void *cmd;
} Sp;
const char *spcmd1[] = {TERMINAL, "-n", "spterm", "-g", "120x34", NULL };
const char *spcmd2[] = {TERMINAL, "-n", "spcalc", "-f", "monospace:size=16", "-g", "50x20", "-e", "bc", "-lq", NULL };
static Sp scratchpads[] = {
  /* name    cmd  */
  {"spterm", spcmd1},
  {"spcalc", spcmd2},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
  */
  /* class      instance    title           tags mask  isfloating   isterminal  noswallow  monitor */
  { "Gimp",     NULL,       NULL,           1 << 8,     0,           0,         0,         -1 },
  { TERMCLASS,  NULL,       NULL,           0,          0,           1,         0,         -1 },
  { NULL,       NULL,       "Event Tester", 0,          0,           0,         1,         -1 },
  { NULL,      "spterm",    NULL,           SPTAG(0),   1,           1,         0,         -1 },
  { NULL,      "spcalc",    NULL,           SPTAG(1),   1,           1,         0,         -1 },
};

/* layout(s) */
static float mfact     = 0.55;  /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;     /* number of clients in master area */
static int resizehints = 1;     /* 1 means respect size hints in tiled resizals */
#define FORCE_VSPLIT 1          /* nrowgrid layout: force two clients to always split vertically */

#include "vanitygaps.c"

static const Layout layouts[] = {
  /* symbol arrange                   function */
  { "[]=",  tile },                   /* Default: Master on left, slaves on right */
  { "TTT",  bstack },                 /* Master on top, slaves on bottom */
  { "[@]",  spiral },                 /* Fibonacci spiral */
  { "[\\]", dwindle },                /* Decreasing in size right and leftward */
  { "[D]",  deck },                   /* Master on left, slaves in monocle-like mode on right */
  { "[M]",  monocle },                /* All windows on top of eachother */
  { "|M|",  centeredmaster },         /* Master in middle, slaves on sides */
  { ">M>",  centeredfloatingmaster }, /* Same but master floats */
  { "><>",  NULL },                   /* no layout function means floating behavior */
  { NULL,   NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY, view,       {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY, toggleview, {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY, tag,        {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY, toggletag,  {.ui = 1 << TAG} },
#define STACKKEYS(MOD,ACTION) \
  { MOD,  XK_j, ACTION##stack,  {.i = INC(+1) } }, \
  { MOD,  XK_k, ACTION##stack,  {.i = INC(-1) } }, \
  /* { MOD, XK_grave, ACTION##stack, {.i = PREVSEL } }, \ */
  /* { MOD, XK_a,     ACTION##stack, {.i = 1 } }, \ */
  /* { MOD, XK_z,     ACTION##stack, {.i = 2 } }, \ */
  /* { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static const char *termcmd[]  = { TERMINAL, NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
  { "color0",         STRING,   &normbordercolor },
  { "color8",         STRING,   &selbordercolor },
  { "color0",         STRING,   &normbgcolor },
  { "color4",         STRING,   &normfgcolor },
  { "color0",         STRING,   &selfgcolor },
  { "color4",         STRING,   &selbgcolor },
  { "borderpx",       INTEGER,  &borderpx },
  { "snap",           INTEGER,  &snap },
  { "showbar",        INTEGER,  &showbar },
  { "topbar",         INTEGER,  &topbar },
  { "nmaster",        INTEGER,  &nmaster },
  { "resizehints",    INTEGER,  &resizehints },
  { "mfact",          FLOAT,    &mfact },
  { "gappih",         INTEGER,  &gappih },
  { "gappiv",         INTEGER,  &gappiv },
  { "gappoh",         INTEGER,  &gappoh },
  { "gappov",         INTEGER,  &gappov },
  { "swallowfloating",INTEGER,  &swallowfloating },
  { "smartgaps",      INTEGER,  &smartgaps },
};

#include <X11/XF86keysym.h>
#include "shiftview.c"

static Key keys[] = {
  /* { MODKEY|ShiftMask,    XK_Escape,  spawn,          SHCMD("") }, */
  /* { MODKEY,              XK_grave,   spawn,          SHCMD("dmenuunicode") }, */
  /* { MODKEY|ShiftMask,    XK_grave,   togglescratch,  SHCMD("") }, */

  TAGKEYS(XK_1, 0)
  TAGKEYS(XK_2, 1)
  TAGKEYS(XK_3, 2)
  TAGKEYS(XK_4, 3)
  TAGKEYS(XK_5, 4)
  TAGKEYS(XK_6, 5)
  TAGKEYS(XK_7, 6)
  TAGKEYS(XK_8, 7)
  TAGKEYS(XK_9, 8)

  /* default bindings */
  { MODKEY,           XK_0,   view,       {.ui = ~0 } },
  { MODKEY|ShiftMask, XK_0,   tag,        {.ui = ~0 } },
  { MODKEY,           XK_Tab, view,       {0} },
  { MODKEY,           XK_q,   killclient, {0} },

  /* volume */
  { MODKEY|ShiftMask,   XK_minus,  spawn,  SHCMD("pamixer --allow-boost -d 5; pkill --signal 10 dwmblocks") },
  { MODKEY|ShiftMask,   XK_equal,  spawn,  SHCMD("pamixer --allow-boost -i 5; pkill --signal 10 dwmblocks") },
  { 0,          XF86XK_AudioMute,  spawn,  SHCMD("pamixer -t; pkill --signal 10 dwmblocks") },
  { 0,   XF86XK_AudioRaiseVolume,  spawn,  SHCMD("pamixer --allow-boost -i 5; pkill --signal 10 dwmblocks") },
  { 0,   XF86XK_AudioLowerVolume,  spawn,  SHCMD("pamixer --allow-boost -d 5; pkill --signal 10 dwmblocks") },

  /* screen brightness */
  { 0,      XF86XK_MonBrightnessUp,   spawn, SHCMD("light -A 5") },
  { 0,      XF86XK_MonBrightnessDown, spawn, SHCMD("light -U 5") },
  { MODKEY, XK_minus,                 spawn, SHCMD("light -U 5") },
  { MODKEY, XK_equal,                 spawn, SHCMD("light -A 5") },

  /* commands */
  /* { MODKEY,            XK_v,  spawn,  SHCMD(TERMINAL " -e vim") }, */
  { MODKEY,            XK_e,  spawn,  SHCMD(TERMINAL " -e neomutt") },
  { MODKEY,            XK_w,  spawn,  SHCMD(BROWSER) },
  { MODKEY,            XK_d,  spawn,  SHCMD("menu-applications") },
  { MODKEY,            XK_c,  spawn,  SHCMD("menu-clipboard") },
  { MODKEY|ControlMask,XK_l,  spawn,  SHCMD("menu-lorem") },
  { MODKEY|ShiftMask,  XK_l,  spawn,  SHCMD("screen-lock") },
  { MODKEY,            XK_p,  spawn,  SHCMD("menu-password") },
  { MODKEY|ShiftMask,  XK_p,  spawn,  SHCMD("menu-otp") },
  { MODKEY,            XK_m,  spawn,  SHCMD("pamixer --toggle-mute; pkill --signal 10 dwmblocks") },

  /* screen temperature color */
  { MODKEY,           XK_F1,  spawn,  SHCMD("screen-color dec 500") },
  { MODKEY,           XK_F2,  spawn,  SHCMD("screen-color inc 500") },

  /* layouts */
  { MODKEY,           XK_t,  setlayout,      {.v = &layouts[0]} }, /* tile */
  { MODKEY|ShiftMask, XK_t,  setlayout,      {.v = &layouts[1]} }, /* bstack */
  { MODKEY,           XK_y,  setlayout,      {.v = &layouts[2]} }, /* spiral */
  { MODKEY|ShiftMask, XK_y,  setlayout,      {.v = &layouts[3]} }, /* dwindle */
  { MODKEY,           XK_u,  setlayout,      {.v = &layouts[4]} }, /* deck */
  { MODKEY|ShiftMask, XK_u,  setlayout,      {.v = &layouts[5]} }, /* monocle */
  { MODKEY,           XK_i,  setlayout,      {.v = &layouts[6]} }, /* centeredmaster */
  { MODKEY|ShiftMask, XK_i,  setlayout,      {.v = &layouts[7]} }, /* centeredfloatingmaster */
  { MODKEY,           XK_o,  incnmaster,     {.i = +1 } },
  { MODKEY|ShiftMask, XK_o,  incnmaster,     {.i = -1 } },
  { MODKEY,           XK_g,  togglegaps,     {0} },
  { MODKEY|ShiftMask, XK_g,  defaultgaps,    {0} },
  { MODKEY,           XK_s,  togglesticky,   {0} },
  { MODKEY,           XK_f,  togglefullscr,  {0} },
  { MODKEY|ShiftMask, XK_f,  setlayout,      {.v = &layouts[8]} },

  /* navigation */
  { MODKEY,           XK_j,           focusstack,     {.i = INC(+1) } },
  { MODKEY,           XK_k,           focusstack,     {.i = INC(-1) } },
  { MODKEY|ShiftMask, XK_j,           pushstack,      {.i = INC(+1) } },
  { MODKEY|ShiftMask, XK_k,           pushstack,      {.i = INC(-1) } },
  { MODKEY,           XK_h,           setmfact,       {.f = -0.05} },
  { MODKEY,           XK_l,           setmfact,       {.f = +0.05} },
  { MODKEY,           XK_semicolon,   shiftview,      { .i = 1 } },
  { MODKEY|ShiftMask, XK_semicolon,   shifttag,       { .i = 1 } },
  { MODKEY,           XK_apostrophe,  togglescratch,  {.ui = 1} },
  { MODKEY,           XK_Return,      spawn,          {.v = termcmd } },
  { MODKEY|ShiftMask, XK_Return,      togglescratch,  {.ui = 0} },

  /* gaps */
  { MODKEY,           XK_z,           incrgaps,       {.i = +3 } },
  { MODKEY,           XK_x,           incrgaps,       {.i = -3 } },

  /* bar */
  { MODKEY,           XK_b,           togglebar,      {0} },

  /* arrow keys */
  { MODKEY,           XK_Left,      focusstack, {.i = INC(-1) } },
  { MODKEY|ShiftMask, XK_Left,      shiftview,  {.i = -1 } },
  { MODKEY,           XK_Right,     focusstack, {.i = INC(+1) } },
  { MODKEY|ShiftMask, XK_Right,     shiftview,  {.i = +1 } },

  /* page keys */
  { MODKEY,           XK_Page_Up,   shiftview,  { .i = -1 } },
  { MODKEY,           XK_Page_Down, shiftview,  { .i = +1 } },
  /* { MODKEY|ShiftMask,   XK_Page_Up,   shifttag,   { .i = -1 } }, */
  /* { MODKEY|ShiftMask,   XK_Page_Down, shifttag,   { .i = +1 } }, */

  { MODKEY,           XK_space, zoom,           {0} },
  { MODKEY|ShiftMask, XK_space, togglefloating, {0} },

  /* print */
  { 0,         XK_Print, spawn, SHCMD("flameshot gui") },
  { ShiftMask, XK_Print, spawn, SHCMD("flameshot full -p ~/pictures/screenshots") },

  /* other stuff */
  { MODKEY,    XK_F11,   spawn, SHCMD("record-webcam") },


  { 0,  XF86XK_WWW,          spawn, SHCMD(BROWSER) },
  { 0,  XF86XK_DOS,          spawn, SHCMD(TERMINAL) },
  { 0,  XF86XK_Launch1,      spawn, SHCMD("xset dpms force off") },
  { 0,  XF86XK_AudioMicMute, spawn, SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle") },

  /* { MODKEY,              XK_Print,       spawn,  SHCMD("dmenurecord") }, */
  /* { MODKEY|ShiftMask,    XK_Print,       spawn,  SHCMD("dmenurecord kill") }, */
  /* { MODKEY,              XK_Delete,      spawn,  SHCMD("dmenurecord kill") }, */
  /* { MODKEY,              XK_Scroll_Lock, spawn,  SHCMD("killall screenkey || screenkey &") }, */
  /* { 0, XF86XK_AudioPrev,     spawn,    SHCMD("mpc prev") }, */
  /* { 0, XF86XK_AudioNext,     spawn,    SHCMD("mpc next") }, */
  /* { 0, XF86XK_AudioPause,    spawn,    SHCMD("mpc pause") }, */
  /* { 0, XF86XK_AudioPlay,     spawn,    SHCMD("mpc play") }, */
  /* { 0, XF86XK_AudioStop,     spawn,    SHCMD("mpc stop") }, */
  /* { 0, XF86XK_AudioRewind,   spawn,    SHCMD("mpc seek -10") }, */
  /* { 0, XF86XK_AudioForward,  spawn,    SHCMD("mpc seek +10") }, */
  /* { 0, XF86XK_AudioMedia,    spawn,    SHCMD(TERMINAL " -e ncmpcpp") }, */
  /* { 0, XF86XK_PowerOff,      spawn,    SHCMD("sysact") }, */
  /* { 0, XF86XK_Calculator,    spawn,    SHCMD(TERMINAL " -e bc -l") }, */
  /* { 0, XF86XK_Sleep,         spawn,    SHCMD("sudo -A zzz") }, */
  /* { 0, XF86XK_ScreenSaver,   spawn,    SHCMD("slock & xset dpms force off; mpc pause; pauseallmpv") }, */
  /* { 0, XF86XK_Mail,          spawn,    SHCMD(TERMINAL " -e neomutt ; pkill -RTMIN+12 dwmblocks") }, */
  /* { 0, XF86XK_MyComputer,    spawn,    SHCMD(TERMINAL " -e ranger /") }, */
  /* { 0, XF86XK_Battery,       spawn,    SHCMD("") }, */
  /* { 0, XF86XK_TouchpadOff,   spawn,    SHCMD("synclient TouchpadOff=1") }, */
  /* { 0, XF86XK_TouchpadOn,    spawn,    SHCMD("synclient TouchpadOff=0") }, */
  /* { 0, XF86XK_TouchpadToggle,spawn,    SHCMD("(synclient | grep 'TouchpadOff.*1' && synclient TouchpadOff=0) || synclient TouchpadOff=1") }, */
  /* { MODKEY|Mod4Mask,              XK_h,      incrgaps,       {.i = +1 } }, */
  /* { MODKEY|Mod4Mask,              XK_l,      incrgaps,       {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } }, */
  /* { MODKEY|Mod4Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } }, */
  /* { MODKEY|Mod4Mask|ShiftMask,    XK_0,      defaultgaps,    {0} }, */
  /* { MODKEY,                       XK_y,      incrihgaps,     {.i = +1 } }, */
  /* { MODKEY,                       XK_o,      incrihgaps,     {.i = -1 } }, */
  /* { MODKEY|ControlMask,           XK_y,      incrivgaps,     {.i = +1 } }, */
  /* { MODKEY|ControlMask,           XK_o,      incrivgaps,     {.i = -1 } }, */
  /* { MODKEY|Mod4Mask,              XK_y,      incrohgaps,     {.i = +1 } }, */
  /* { MODKEY|Mod4Mask,              XK_o,      incrohgaps,     {.i = -1 } }, */
  /* { MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } }, */
  /* { MODKEY|ShiftMask,             XK_o,      incrovgaps,     {.i = -1 } }, */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */

static Button buttons[] = {
  /* click          event mask  button    function        argument */
#ifndef __OpenBSD__
  { ClkWinTitle,    0,          Button2,  zoom,         {0} },
  { ClkStatusText,  0,          Button1,  sigdwmblocks, {.i = 1} },
  { ClkStatusText,  0,          Button2,  sigdwmblocks, {.i = 2} },
  { ClkStatusText,  0,          Button3,  sigdwmblocks, {.i = 3} },
  { ClkStatusText,  ShiftMask,  Button1,  sigdwmblocks, {.i = 6} },
#endif
  { ClkClientWin,   MODKEY,     Button1,  movemouse,    {0} },
  { ClkClientWin,   MODKEY,     Button2,  defaultgaps,  {0} },
  { ClkClientWin,   MODKEY,     Button3,  resizemouse,  {0} },
  { ClkClientWin,   MODKEY,     Button4,  incrgaps,     {.i = +1} },
  { ClkClientWin,   MODKEY,     Button5,  incrgaps,     {.i = -1} },
  { ClkTagBar,      0,          Button1,  view,         {0} },
  { ClkTagBar,      0,          Button3,  toggleview,   {0} },
  { ClkTagBar,      MODKEY,     Button1,  tag,          {0} },
  { ClkTagBar,      MODKEY,     Button3,  toggletag,    {0} },
  { ClkTagBar,      0,          Button4,  shiftview,    {.i = -1} },
  { ClkTagBar,      0,          Button5,  shiftview,    {.i = 1} },
  { ClkRootWin,     0,          Button2,  togglebar,    {0} },
};
